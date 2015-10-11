//------------------------------------------------------------------------------
//  SquarePusher.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "Core/App.h"
#include "Gfx/Gfx.h"
#include "Assets/Gfx/MeshBuilder.h"
#include "shaders.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Oryol;

using namespace Oryol;

class SquarePusher : public App {
public:
    /// on init frame method
    virtual AppState::Code OnInit();
    /// on running frame method
    virtual AppState::Code OnRunning();
    /// on cleanup frame method
    virtual AppState::Code OnCleanup();

    /// update shader params
    void updateShaderParams();
    /// initialize square states
    void initSquares();
    /// update square states
    void updateSquares();

    int32 frameIndex = 0;
    Id squareStateMesh;
    Id drawState;
    Shaders::Main::VSParams vsParams;
    glm::mat4 proj;

    static const int NumSquaresPerStrip = 10;
    static const int NumStrips = 1;
    static const int NumSquares = NumStrips * NumSquaresPerStrip;
    struct square {
        glm::vec2 orig;     // origin
        glm::vec2 dir;      // direction
        float startTime;    //
        float tileIndex;    // 0.0 is head
        glm::vec2 _pad;
    } squares[NumStrips][NumSquaresPerStrip];
};
OryolMain(SquarePusher);

//------------------------------------------------------------------------------
AppState::Code
SquarePusher::OnInit() {
    auto gfxSetup = GfxSetup::WindowMSAA4(640, 480, "SquarePusher");
    Gfx::Setup(gfxSetup);

    // create the instance mesh with square states
    auto instSetup = MeshSetup::Empty(NumSquares, Usage::Stream);
    instSetup.NumVertices = NumSquares;
    instSetup.Layout
        .Add(VertexAttr::Instance0, VertexFormat::Float4)
        .Add(VertexAttr::Instance1, VertexFormat::Float4);
    instSetup.StepFunction = VertexStepFunction::PerInstance;
    instSetup.StepRate = 1;
    this->squareStateMesh = Gfx::CreateResource(instSetup);
    
    // create a quad mesh
    MeshBuilder mb;
    mb.NumVertices = 4;
    mb.NumIndices = 6;
    mb.PrimitiveGroups.Add(0, 6);
    mb.Layout
        .Add(VertexAttr::Position, VertexFormat::Float2)
        .Add(VertexAttr::Normal, VertexFormat::Byte4N);
    mb.Begin();
    const float quad[4][2] = {
        { -0.5f, -0.5f }, { +0.5f, -0.5f }, { +0.5f, +0.5f }, { -0.5f, +0.5f }
    };
    for (int i = 0; i < 4; i++) {
        mb.Vertex(i, VertexAttr::Position, quad[i][0], quad[i][1]);
        mb.Vertex(i, VertexAttr::Normal, 0.0f, 1.0f, 0.0f, 0.0f);
    }
    mb.Triangle(0, 0, 1, 2);
    mb.Triangle(1, 0, 2, 3);
    mb.End();
    const Id quadMesh = Gfx::CreateResource(mb.Result());

    // shader and draw-state
    const Id quadShd = Gfx::CreateResource(Shaders::Main::Setup());
    auto dss = DrawStateSetup::FromMeshAndShader(quadMesh, quadShd);
    dss.DepthStencilState.DepthWriteEnabled = true;
    dss.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
    dss.RasterizerState.CullFaceEnabled = false;
    dss.RasterizerState.SampleCount = gfxSetup.SampleCount;
    dss.Meshes[1] = this->squareStateMesh;
    this->drawState = Gfx::CreateResource(dss);

    // projection matrix
    const float width = Gfx::DisplayAttrs().FramebufferWidth;
    const float height = Gfx::DisplayAttrs().FramebufferHeight;
    this->proj = glm::perspectiveFov(glm::radians(60.0f), width, height, 0.1f, 100.0f);

    // one-time init of square strips
    this->initSquares();

    return App::OnInit();
}

//------------------------------------------------------------------------------
void
SquarePusher::updateShaderParams() {
    const glm::vec3 eye(5.0f, 10.0f, 10.0f);
    const glm::vec3 center(0.0f, 0.0f, 0.0f);
    const glm::vec3 up(0.0f, 1.0f, 0.0f);
    const glm::mat4 view = glm::lookAt(eye, center, up);
    const glm::mat4 mvp = this->proj * view;
    this->vsParams.ModelViewProj = mvp;
    this->vsParams.ModelView = view;
    this->vsParams.Time = this->frameIndex * (1.0f/60.0f);
}

//------------------------------------------------------------------------------
void
SquarePusher::initSquares() {
    for (int stripIndex = 0; stripIndex < NumStrips; stripIndex++) {
        for (int squareIndex = 0; squareIndex < NumSquaresPerStrip; squareIndex++) {
            square& s = this->squares[stripIndex][squareIndex];
            s.orig = glm::vec2(0.0f, 0.0f);
            s.dir  = glm::vec2(0.0f, 1.0f);
            s.startTime = 0.0f;
            s.tileIndex = squareIndex;
        }
    }
}

//------------------------------------------------------------------------------
void
SquarePusher::updateSquares() {
    // FIXME
}

//------------------------------------------------------------------------------
AppState::Code
SquarePusher::OnRunning() {

    this->updateShaderParams();

    Gfx::ApplyDefaultRenderTarget();
    Gfx::UpdateVertices(this->squareStateMesh, this->squares, sizeof(this->squares));
    Gfx::ApplyDrawState(this->drawState);
    Gfx::ApplyUniformBlock(this->vsParams);
    Gfx::DrawInstanced(0, NumSquares);
    Gfx::CommitFrame();

    this->frameIndex++;
    return Gfx::QuitRequested() ? AppState::Cleanup : AppState::Running;
}

//------------------------------------------------------------------------------
AppState::Code
SquarePusher::OnCleanup() {
    Gfx::Discard();
    return App::OnCleanup();
}
