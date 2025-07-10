//// DrawCommand.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/05/2025
//  Description: Header file for the draw command
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Shader.hpp"
#include "UniformCache.hpp"
#include "VertexArray.hpp"

namespace nexo::renderer {

    // Function to get the quad, initializing it on first use
    inline std::shared_ptr<NxVertexArray> getFullscreenQuad()
    {
        static std::shared_ptr<NxVertexArray> s_fullscreenQuad = nullptr;
        if (!s_fullscreenQuad) {
            // Create a simple 2-triangle quad covering NDC [-1..1]
            s_fullscreenQuad = createVertexArray();
            static float quadVertices[] = {
                // positions   // tex coords
                -1.0f,  1.0f,  0.0f, 1.0f,
                -1.0f, -1.0f,  0.0f, 0.0f,
                 1.0f, -1.0f,  1.0f, 0.0f,
                -1.0f,  1.0f,  0.0f, 1.0f,
                 1.0f, -1.0f,  1.0f, 0.0f,
                 1.0f,  1.0f,  1.0f, 1.0f
            };
            auto vb = createVertexBuffer(sizeof(quadVertices));
            vb->setData(quadVertices, sizeof(quadVertices));
            vb->setLayout({
                {NxShaderDataType::FLOAT2, "aPosition"},
                {NxShaderDataType::FLOAT2, "aTexCoord"}
            });
            s_fullscreenQuad->addVertexBuffer(vb);
        }
        return s_fullscreenQuad;
    }

    enum class CommandType {
        MESH,
        FULL_SCREEN,
    };

    struct DrawCommand {
        CommandType type = CommandType::MESH;

        std::shared_ptr<NxVertexArray> vao;
        std::shared_ptr<NxShader> shader;
        std::unordered_map<std::string, UniformValue> uniforms;

        uint32_t filterMask = 0xFFFFFFFF;
        bool isOpaque = true;

        void execute() const;
    };
}
