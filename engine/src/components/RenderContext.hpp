//// RenderContext.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Header file for the render context singleton component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <queue>
#include "Camera.hpp"
#include "Types.hpp"
#include "renderer/Renderer3D.hpp"
#include "Light.hpp"

namespace nexo::components {
    struct RenderContext {
        int sceneRendered = -1;
        SceneType sceneType;
        bool isChildWindow = false; //<< Is the current scene embedded in a sub window ?
        glm::vec2 viewportBounds[2]{}; //<< Viewport bounds in absolute coordinates (if the window viewport is embedded in the window), this is used for mouse coordinates
        struct GridParams {
        bool enabled = true;
        float gridSize = 100.0f;
        float minPixelsBetweenCells = 2.0f;
        float cellSize = 0.025f;
        };
        GridParams gridParams;
        renderer::NxRenderer3D renderer3D;
        std::queue<CameraContext> cameras;
        LightContext sceneLights;


        RenderContext(): sceneType(), sceneLights()
        {
            renderer3D.init();
        }

        // Delete copy constructor to enforce singleton semantics
        RenderContext(const RenderContext&) = delete;
        RenderContext& operator=(const RenderContext&) = delete;

        RenderContext(RenderContext&& other) noexcept
            : sceneRendered(other.sceneRendered), sceneType{},
              renderer3D(std::move(other.renderer3D)),
              cameras(std::move(other.cameras)),
              sceneLights(other.sceneLights)
        {}

        ~RenderContext()
        {
        renderer3D.shutdown();

        reset();
        }

        void reset()
        {
        sceneRendered = -1;
        isChildWindow = false;
        viewportBounds[0] = glm::vec2{};
        viewportBounds[1] = glm::vec2{};
        std::queue<CameraContext> empty;
        std::swap(cameras, empty);
        sceneLights.ambientLight = glm::vec3(0.0f);
        sceneLights.pointLightCount = 0;
        sceneLights.spotLightCount = 0;
        sceneLights.dirLight = DirectionalLightComponent{};
        }
    };
}
