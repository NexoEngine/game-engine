//// Nexo.hpp /////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Main header file for nexo
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Application.hpp"
#include "core/layer/Layer.hpp"
#include "core/event/KeyCodes.hpp"
#include "core/Timestep.hpp"
#include "core/Timer.hpp"
#include "Logger.hpp"

// Renderer
#include "renderer/Renderer.hpp"
#include "renderer/Renderer2D.hpp"
#include "renderer/RenderCommand.hpp"

#include "renderer/Buffer.hpp"
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
#include "renderer/VertexArray.hpp"

#include "core/camera/OrthographicCamera.hpp"
#include "core/camera/OrthographicCameraController.hpp"

#include "components/Components.hpp"

namespace nexo {
    template<typename DerivedApp, typename ...Args>
    void useApp(Args &&... args)
    {
        Application::setInstance<DerivedApp, Args...>(std::forward<Args>(args)...);
    }

    Application &init();

    Application &getApp();

    void runEngine(bool renderToFrameBuffer = false);
}
