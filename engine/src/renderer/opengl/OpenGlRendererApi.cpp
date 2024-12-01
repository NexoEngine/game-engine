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
#include <Exception.hpp>
#include <RendererExceptions.hpp>

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
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);
        int maxViewportSize[] = {0, 0};
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewportSize);
        m_maxWidth = maxViewportSize[0];
        m_maxHeight = maxViewportSize[1];
        m_initialized = true;
        LOG(NEXO_DEV, "Opengl renderer api initialized");
    }

    void OpenGlRendererApi::setViewport(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
    {
        if (!m_initialized)
            THROW_EXCEPTION(GraphicsApiNotInitialized, "OPENGL");
        if (!width || !height)
            THROW_EXCEPTION(GraphicsApiViewportResizingFailure, "OPENGL", false, width, height);
        if (width > m_maxWidth || height > m_maxHeight)
            THROW_EXCEPTION(GraphicsApiViewportResizingFailure, "OPENGL", true, width, height);
        glViewport(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
    }

    void OpenGlRendererApi::getMaxViewportSize(unsigned int *width, unsigned int *height)
    {
        *width = m_maxWidth;
        *height = m_maxHeight;
    }

    void OpenGlRendererApi::clear()
    {
        if (!m_initialized)
            THROW_EXCEPTION(GraphicsApiNotInitialized, "OPENGL");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGlRendererApi::setClearColor(const glm::vec4 &color)
    {
        if (!m_initialized)
            THROW_EXCEPTION(GraphicsApiNotInitialized, "OPENGL");
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGlRendererApi::setClearDepth(const float depth)
    {
        if (!m_initialized)
            THROW_EXCEPTION(GraphicsApiNotInitialized, "OPENGL");
        glClearDepth(depth);
    }

    void OpenGlRendererApi::drawIndexed(const std::shared_ptr<VertexArray> &vertexArray, const unsigned int indexCount)
    {
        if (!m_initialized)
            THROW_EXCEPTION(GraphicsApiNotInitialized, "OPENGL");
        if (!vertexArray)
            THROW_EXCEPTION(InvalidValue, "OPENGL", "Vertex array cannot be null");
        const unsigned int count = indexCount ? vertexArray->getIndexBuffer()->getCount() : indexCount;
        glDrawElements(GL_TRIANGLES, static_cast<int>(count), GL_UNSIGNED_INT, nullptr);
    }
}
