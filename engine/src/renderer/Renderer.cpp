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
