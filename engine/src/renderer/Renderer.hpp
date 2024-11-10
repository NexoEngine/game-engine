//// Renderer.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for renderer class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "RenderCommand.hpp"
#include "core/camera/OrthographicCamera.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>

namespace nexo::renderer {

    class Renderer {
        public:
            static void init();
            static void onWindowResize(unsigned int width, unsigned int height);

            static void beginScene(const camera::OrthographicCamera &camera);
            static void endScene();

            static void submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4 &transform = glm::mat4(1.0f));

            struct SceneData {
                glm::mat4 projectionMatrix;
            };
            static SceneData *_sceneData;

    };
}
