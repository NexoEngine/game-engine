//// Renderer.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for renderer class
//
///////////////////////////////////////////////////////////////////////////////

#include "Renderer.hpp"
#include "RenderCommand.hpp"

namespace nexo::renderer {

    NxRenderer::NxSceneData *NxRenderer::_sceneData = new NxSceneData;

    void NxRenderer::init()
    {
        NxRenderCommand::init();
    }

    void NxRenderer::onWindowResize(const unsigned int width, const unsigned int height)
    {
        NxRenderCommand::setViewport(0, 0, width, height);
    }
}
