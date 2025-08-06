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
#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

namespace nexo::renderer {

    void NxOpenGlRendererApi::init()
    {
        ZoneScoped;
        ZoneName("OpenGL Renderer API Init", 23);

        {
            ZoneScopedN("Setup OpenGL State");
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
        }

        {
            ZoneScopedN("Query OpenGL Capabilities");
            int maxViewportSize[] = {0, 0};
            glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewportSize);
            m_maxWidth = static_cast<unsigned int>(maxViewportSize[0]);
            m_maxHeight = static_cast<unsigned int>(maxViewportSize[1]);
        }

        // Initialize Tracy GPU context for OpenGL profiling
        {
            ZoneScopedN("Initialize GPU Profiling");
            TracyGpuContext;
            TracyGpuContextName("OpenGL Main Context", 20);
        }

        m_initialized = true;
        LOG(NEXO_DEV, "Opengl renderer api initialized");
    }

    void NxOpenGlRendererApi::setViewport(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
    {
        ZoneScoped;
        ZoneName("Set Viewport", 12);
        TracyGpuZone("GPU Viewport");
        ZoneValue(static_cast<int64_t>(width * height));

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
        ZoneScoped;
        ZoneName("Clear Buffers", 13);
        TracyGpuZone("GPU Clear");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void NxOpenGlRendererApi::setClearColor(const glm::vec4 &color)
    {
        ZoneScoped;
        ZoneName("Set Clear Color", 15);
        TracyGpuZone("GPU Set Clear Color");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void NxOpenGlRendererApi::setClearDepth(const float depth)
    {
        ZoneScoped;
        ZoneName("Set Clear Depth", 15);
        TracyGpuZone("GPU Set Clear Depth");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glClearDepth(depth);
    }

    void NxOpenGlRendererApi::setDepthTest(const bool enable)
    {
        ZoneScoped;
        ZoneName("Set Depth Test", 14);
        TracyGpuZone("GPU Set Depth Test");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (enable)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void NxOpenGlRendererApi::setDepthFunc(const unsigned int func)
    {
        ZoneScoped;
        ZoneName("Set Depth Function", 18);
        TracyGpuZone("GPU Set Depth Func");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glDepthFunc(func);
    }

    void NxOpenGlRendererApi::setDepthMask(const bool enable)
    {
        ZoneScoped;
        ZoneName("Set Depth Mask", 14);
        TracyGpuZone("GPU Set Depth Mask");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (enable)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);
    }

    void NxOpenGlRendererApi::drawIndexed(const std::shared_ptr<NxVertexArray> &vertexArray, const unsigned int indexCount)
    {
        ZoneScoped;
        ZoneName("Draw Indexed", 12);
        TracyGpuZone("GPU Draw Indexed");
        ZoneValue(static_cast<int64_t>(indexCount));

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (!vertexArray)
            THROW_EXCEPTION(NxInvalidValue, "OPENGL", "Vertex array cannot be null");
        const unsigned int count = indexCount ? vertexArray->getIndexBuffer()->getCount() : indexCount;
        glDrawElements(GL_TRIANGLES, static_cast<int>(count), GL_UNSIGNED_INT, nullptr);
    }

    void NxOpenGlRendererApi::drawUnIndexed(const unsigned int verticesCount)
    {
        ZoneScoped;
        ZoneName("Draw Unindexed", 14);
        TracyGpuZone("GPU Draw Arrays");
        ZoneValue(static_cast<int64_t>(verticesCount));

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(verticesCount));
    }

    void NxOpenGlRendererApi::setStencilTest(const bool enable)
    {
        ZoneScoped;
        ZoneName("Set Stencil Test", 16);
        TracyGpuZone("GPU Set Stencil Test");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (enable)
            glEnable(GL_STENCIL_TEST);
        else
            glDisable(GL_STENCIL_TEST);
    }

    void NxOpenGlRendererApi::setStencilMask(const unsigned int mask)
    {
        ZoneScoped;
        ZoneName("Set Stencil Mask", 16);
        TracyGpuZone("GPU Set Stencil Mask");
        ZoneValue(static_cast<int64_t>(mask));

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glStencilMask(mask);
    }

    void NxOpenGlRendererApi::setStencilFunc(const unsigned int func, const int ref, const unsigned int mask)
    {
        ZoneScoped;
        ZoneName("Set Stencil Function", 19);
        TracyGpuZone("GPU Set Stencil Func");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glStencilFunc(func, ref, mask);
    }

    void NxOpenGlRendererApi::setStencilOp(const unsigned int sfail, const unsigned int dpfail, const unsigned int dppass)
    {
        ZoneScoped;
        ZoneName("Set Stencil Operation", 20);
        TracyGpuZone("GPU Set Stencil Op");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        glStencilOp(sfail, dpfail, dppass);
    }

    void NxOpenGlRendererApi::setCulling(const bool enable)
    {
        ZoneScoped;
        ZoneName("Set Face Culling", 16);
        TracyGpuZone("GPU Set Culling");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (enable)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
    }

    void NxOpenGlRendererApi::setCulledFace(const CulledFace face)
    {
        ZoneScoped;
        ZoneName("Set Culled Face", 15);
        TracyGpuZone("GPU Set Culled Face");

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
        ZoneScoped;
        ZoneName("Set Winding Order", 17);
        TracyGpuZone("GPU Set Winding Order");

        if (!m_initialized)
            THROW_EXCEPTION(NxGraphicsApiNotInitialized, "OPENGL");
        if (order == WindingOrder::CCW)
            glFrontFace(GL_CCW);
        else if (order == WindingOrder::CW)
            glFrontFace(GL_CW);
    }
}
