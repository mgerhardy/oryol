#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::_priv::texture
    @ingroup _priv
    @brief texture frontend class
    
    A texture object can be a normal 2D, 3D or cube texture, as well
    as a render target with optional depth buffer.
*/
#if ORYOL_OPENGL
#include "Gfx/gl/glTexture.h"
namespace Oryol {
namespace _priv {
class texture : public glTexture { };
} }
#elif ORYOL_D3D11
#include "Gfx/d3d11/d3d11Texture.h"
namespace Oryol {
namespace _priv {
class texture : public d3d11Texture { };
} }
#elif ORYOL_D3D12
#include "Gfx/d3d12/d3d12texture.h"
namespace Oryol {
namespace _priv {
class texture : public d3d12Texture { };
} }
#elif ORYOL_METAL
#include "Gfx/mtl/mtlTexture.h"
namespace Oryol {
namespace _priv {
class texture : public mtlTexture { };
} }
#else
#error "Target platform not yet supported!"
#endif



