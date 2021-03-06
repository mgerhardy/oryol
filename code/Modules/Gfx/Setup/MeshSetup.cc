//------------------------------------------------------------------------------
//  MeshSetup.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "MeshSetup.h"

namespace Oryol {

//------------------------------------------------------------------------------
MeshSetup::MeshSetup() :
VertexUsage(Usage::InvalidUsage),
IndexUsage(Usage::InvalidUsage),
NumVertices(0),
NumIndices(0),
IndicesType(IndexType::None),
FullScreenQuadFlipV(false),
Locator(Locator::NonShared()),
DataVertexOffset(0),
DataIndexOffset(InvalidIndex),
numPrimGroups(0),
setupFromFile(false),
setupFromData(false),
setupEmpty(false),
setupFullScreenQuad(false) {
    // empty
}

//------------------------------------------------------------------------------
MeshSetup
MeshSetup::FromFile(const class Locator& loc, Id placeholder) {
    MeshSetup setup;
    setup.VertexUsage = Usage::Immutable;
    setup.IndexUsage = Usage::Immutable;
    setup.Locator = loc;
    setup.Placeholder = placeholder;
    setup.setupFromFile = true;
    return setup;
}

//------------------------------------------------------------------------------
MeshSetup
MeshSetup::FromData(Usage::Code vbUsage, Usage::Code ibUsage) {
    MeshSetup setup;
    setup.VertexUsage = vbUsage;
    setup.IndexUsage = ibUsage;
    setup.setupFromData = true;
    return setup;
}

//------------------------------------------------------------------------------
MeshSetup
MeshSetup::FromData(const MeshSetup& blueprint) {
    MeshSetup setup(blueprint);
    setup.setupFromData = true;
    return setup;
}

//------------------------------------------------------------------------------
MeshSetup
MeshSetup::Empty(int32 numVertices,
                 Usage::Code vertexUsage,
                 IndexType::Code indexType,
                 int32 numIndices,
                 Usage::Code indexUsage) {
    
    o_assert_dbg(numVertices > 0);
    
    MeshSetup setup;
    setup.setupEmpty = true;
    setup.VertexUsage = vertexUsage;
    setup.IndexUsage = indexUsage;
    setup.NumVertices = numVertices;
    setup.NumIndices = numIndices;
    setup.IndicesType = indexType;
    setup.DataVertexOffset = InvalidIndex;
    setup.DataIndexOffset = InvalidIndex;
    return setup;
}

//------------------------------------------------------------------------------
MeshSetup
MeshSetup::FullScreenQuad(bool flipV) {
    MeshSetup setup;
    setup.setupFullScreenQuad = true;
    setup.FullScreenQuadFlipV = flipV;
    setup.Layout.Add(VertexAttr::Position, VertexFormat::Float3);
    setup.Layout.Add(VertexAttr::TexCoord0, VertexFormat::Float2);
    return setup;
}

//------------------------------------------------------------------------------
bool
MeshSetup::ShouldSetupFromFile() const {
    return this->setupFromFile;
}

//------------------------------------------------------------------------------
bool
MeshSetup::ShouldSetupFromData() const {
    return this->setupFromData;
}

//------------------------------------------------------------------------------
bool
MeshSetup::ShouldSetupEmpty() const {
    return this->setupEmpty;
}

//------------------------------------------------------------------------------
bool
MeshSetup::ShouldSetupFullScreenQuad() const {
    return this->setupFullScreenQuad;
}

//------------------------------------------------------------------------------
void
MeshSetup::AddPrimitiveGroup(const class PrimitiveGroup& primGroup) {
    o_assert(this->setupEmpty || this->setupFromData);
    o_assert(this->numPrimGroups < GfxConfig::MaxNumPrimGroups);
    this->primGroups[this->numPrimGroups++] = primGroup;
}

//------------------------------------------------------------------------------
int32
MeshSetup::NumPrimitiveGroups() const {
    return this->numPrimGroups;
}

//------------------------------------------------------------------------------
const class PrimitiveGroup&
MeshSetup::PrimitiveGroup(int32 index) const {
    o_assert_range(index, GfxConfig::MaxNumPrimGroups);
    return this->primGroups[index];
}

} // namespace Oryol