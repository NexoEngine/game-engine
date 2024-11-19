//// Renderer.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for renderer class
//
///////////////////////////////////////////////////////////////////////////////
#include "Renderer.hpp"
#include "Renderer2D.hpp"

namespace nexo::renderer {

    Renderer::SceneData *Renderer::_sceneData = new Renderer::SceneData;

    void Renderer::init()
    {
        RenderCommand::init();
    }

    void Renderer::onWindowResize(unsigned int width, unsigned int height)
    {
        RenderCommand::setViewport(0, 0, width, height);
    }

    void Renderer::beginScene(const camera::OrthographicCamera &camera)
    {
        _sceneData->projectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::endScene()
    {
    }

    void Renderer::submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4 &transform)
    {
        shader->bind();
        shader->setUniformMatrix("viewProjection", _sceneData->projectionMatrix);
        shader->setUniformMatrix("transform", transform);

        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }
}
