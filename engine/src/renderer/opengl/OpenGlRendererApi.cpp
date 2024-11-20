//// OpenGlRendererApi.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for renderer api class
//
///////////////////////////////////////////////////////////////////////////////
#include "OpenGlRendererAPI.hpp"
#include "Logger.hpp"

#include <glad/glad.h>

namespace nexo::renderer {

    void OpenGlRendererApi::init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        LOG(NEXO_DEV, "Opengl renderer api initialized");
    }

    void OpenGlRendererApi::setViewport(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
    {
        glViewport(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
    }

    void OpenGlRendererApi::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGlRendererApi::setClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGlRendererApi::drawIndexed(const std::shared_ptr<VertexArray> &vertexArray, const unsigned int indexCount)
    {
        const unsigned int count = indexCount ? vertexArray->getIndexBuffer()->getCount() : indexCount;
        glDrawElements(GL_TRIANGLES, static_cast<int>(count), GL_UNSIGNED_INT, nullptr);
    }
}
