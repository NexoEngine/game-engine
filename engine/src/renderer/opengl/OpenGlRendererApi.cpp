//// OpenGlRendererApi.cpp ////////////////////////////////////////////////////
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

    void NxOpenGlRendererApi::init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilMask(0xFF);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        int maxViewportSize[] = {0, 0};
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewportSize);
        m_maxWidth = static_cast<unsigned int>(maxViewportSize[0]);
        m_maxHeight = static_cast<unsigned int>(maxViewportSize[1]);
        m_initialized = true;
        LOG(NEXO_DEV, "Opengl renderer api initialized");
    }

    void NxOpenGlRendererApi::setViewport(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (!width || !height)
            THROW_EXCEPTION(NxGraphicsApiViewportResizingFailure, "OPENGL", false, width, height);
        if (width > m_maxWidth || height > m_maxHeight)
            THROW_EXCEPTION(NxGraphicsApiViewportResizingFailure, "OPENGL", true, width, height);
        glViewport(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
    }

    void NxOpenGlRendererApi::getMaxViewportSize(unsigned int *width, unsigned int *height)
    {
        *width = m_maxWidth;
        *height = m_maxHeight;
    }

    void NxOpenGlRendererApi::clear()
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void NxOpenGlRendererApi::setClearColor(const glm::vec4 &color)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void NxOpenGlRendererApi::setClearDepth(const float depth)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glClearDepth(depth);
    }

    void NxOpenGlRendererApi::setDepthTest(const bool enable)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (enable)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void NxOpenGlRendererApi::setDepthFunc(const unsigned int func)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glDepthFunc(func);
    }

    void NxOpenGlRendererApi::setDepthMask(const bool enable)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (enable)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);
    }

    void NxOpenGlRendererApi::drawIndexed(const std::shared_ptr<NxVertexArray> &vertexArray, const unsigned int indexCount)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (!vertexArray)
            THROW_EXCEPTION(NxInvalidValue, "OPENGL", "Vertex array cannot be null");
        const unsigned int count = indexCount ? vertexArray->getIndexBuffer()->getCount() : indexCount;
        glDrawElements(GL_TRIANGLES, static_cast<int>(count), GL_UNSIGNED_INT, nullptr);
    }

    void NxOpenGlRendererApi::drawUnIndexed(const unsigned int verticesCount)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(verticesCount));
    }

    void NxOpenGlRendererApi::setStencilTest(const bool enable)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (enable)
            glEnable(GL_STENCIL_TEST);
        else
            glDisable(GL_STENCIL_TEST);
    }

    void NxOpenGlRendererApi::setStencilMask(const unsigned int mask)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glStencilMask(mask);
    }

    void NxOpenGlRendererApi::setStencilFunc(const unsigned int func, const int ref, const unsigned int mask)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glStencilFunc(func, ref, mask);
    }

    void NxOpenGlRendererApi::setStencilOp(const unsigned int sfail, const unsigned int dpfail, const unsigned int dppass)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glStencilOp(sfail, dpfail, dppass);
    }

    void NxOpenGlRendererApi::setCulling(const bool enable)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (enable)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
    }

    void NxOpenGlRendererApi::setCulledFace(const CulledFace face)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (face == CulledFace::BACK)
            glCullFace(GL_BACK);
        else if (face == CulledFace::FRONT)
            glCullFace(GL_FRONT);
        else if (face == CulledFace::FRONT_AND_BACK)
            glCullFace(GL_FRONT_AND_BACK);
    }

    void NxOpenGlRendererApi::setWindingOrder(const WindingOrder order)
    {
        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (order == WindingOrder::CCW)
            glFrontFace(GL_CCW);
        else if (order == WindingOrder::CW)
            glFrontFace(GL_CW);
    }
}
