#pragma once

namespace nexo::renderer {

    enum class TextureType {
        ALBEDO,      // sRGB, linear filtering, mipmaps
        NORMAL,      // Linear, linear filtering, mipmaps
        METALLIC,    // Linear, linear filtering, mipmaps
        ROUGHNESS,   // Linear, linear filtering, mipmaps
        AO,          // Linear, linear filtering, mipmaps
        EMISSIVE,    // sRGB, linear filtering, mipmaps
        HEIGHT,       // Linear, linear filtering, mipmaps
        COMBINED_MR,
        COMBINED_ORM,
    };

} // namespace nexo::renderer
