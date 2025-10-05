//// DrawCommand.hpp //////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        07/05/2025
//  Description: Header file for the draw command
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <array>
#include "Shader.hpp"
#include "UniformCache.hpp"
#include "VertexArray.hpp"

namespace nexo::renderer {
    /**
     * @brief Retrieves a shared pointer to a fullscreen quad vertex array.
     *
     * This function initializes a fullscreen quad vertex array on its first invocation,
     * creating a simple 2-triangle quad that covers the normalized device coordinates (NDC)
     * from -1 to 1. The quad is defined with position and texture coordinate attributes.
     *
     * @return A shared pointer to the NxVertexArray representing the fullscreen quad.
     *
     * Example usage:
     * ```cpp
     * auto fullscreenQuad = getFullscreenQuad();
     * fullscreenQuad->bind();
     * ```
     */
    inline std::shared_ptr<NxVertexArray> getFullscreenQuad()
    {
        static std::shared_ptr<NxVertexArray> s_fullscreenQuad = nullptr;
        if (!s_fullscreenQuad) {
            s_fullscreenQuad               = createVertexArray();
            // Create a simple 2-triangle quad covering NDC [-1..1]
            static std::array quadVertices = {-1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // positions   // tex coords
                                              1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,
                                              1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f};
            const auto vb = createVertexBuffer(static_cast<unsigned int>(sizeof(float) * quadVertices.size()));
            vb->setData(quadVertices.data(), sizeof(float) * quadVertices.size());
            vb->setLayout({{NxShaderDataType::FLOAT2, "aPosition"}, {NxShaderDataType::FLOAT2, "aTexCoord"}});
            s_fullscreenQuad->addVertexBuffer(vb);
        }
        return s_fullscreenQuad;
    }

    enum class CommandType {
        MESH,
        LINE,
        FULL_SCREEN,
    };

    struct DrawCommand {
        CommandType type = CommandType::MESH;

        std::shared_ptr<NxVertexArray> vao;
        std::shared_ptr<NxShader> shader;
        std::unordered_map<std::string, UniformValue> uniforms;

        uint32_t filterMask = 0xFFFFFFFF;
        bool isOpaque       = true;

        float lineWidth = 1.5f;

        /**
         * @brief Executes the draw command.
         *
         * This method binds the necessary shader and vertex array object (VAO),
         * sets the required uniforms, and issues the appropriate draw call based
         * on the command type (mesh or fullscreen).
         *
         * It optimizes state changes by tracking the currently bound shader and VAO,
         * only rebinding them if they differ from the last used ones.
         */
        void execute() const;
    };
} // namespace nexo::renderer
