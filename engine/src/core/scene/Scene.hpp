//// Scene.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the scene class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <utility>
#include <array>

#include "core/layer/LayerStack.hpp"
#include "core/camera/Camera.hpp"
#include "ecs/Coordinator.hpp"
#include "core/event/Event.hpp"
#include "Logger.hpp"
#include "renderer/RendererContext.hpp"
#include "components/Light.hpp"

namespace nexo::scene {
    using SceneId = unsigned int;
    using LayerId = unsigned int;

    struct LightContext {
        LightContext() = default;

        float ambientLight{};
        std::array<std::shared_ptr<components::Light>, MAX_POINT_LIGHTS + MAX_DIRECTIONAL_LIGHTS> m_lights;
        unsigned int nbLights = 0;
        unsigned int nbPointLights = 0;
        unsigned int nbDirectionalLights = 0;
    };

    struct SceneContext {
        LightContext lightContext;
    };

    class Scene {
        public:
            Scene() : id(0), name("default") {};

            explicit Scene(const SceneId id, std::string sceneName = "Defaut scene") : id(id),
                                                                                       name(std::move(sceneName))
            {
                LOG(NEXO_INFO, "Scene {} created with id: {}", name, id);
                m_rendererContext = std::make_shared<renderer::RendererContext>();
                m_rendererContext->renderer2D.init();
                m_rendererContext->renderer3D.init();
            };

            ~Scene()
            {
                LOG(NEXO_DEBUG, "Scene {} deleted", name);
            };

            [[nodiscard]] const std::string &getName() const { return name; }

            void addLayer(LayerId id, const std::string &layerName);

            void addOverlay(LayerId id, const std::string &overlayName);

            void removeLayer(LayerId id);

            void removeOverlay(LayerId id);

            [[nodiscard]] std::shared_ptr<layer::Layer> getLayer(LayerId id) const;

            [[nodiscard]] const layer::LayerStack &getLayerStack() const { return m_layerStack; };

            void addEntityToLayer(ecs::Entity entity, LayerId id);

            void addGlobalEntity(ecs::Entity entity);

            void removeEntityFromLayer(ecs::Entity entity, LayerId id);

            void removeGlobalEntity(ecs::Entity entity);

            void entityDestroyed(ecs::Entity entity);

            [[nodiscard]] std::set<ecs::Entity> getEntities() const;

            [[nodiscard]] std::set<ecs::Entity> getRenderedEntities() const;

            [[nodiscard]] std::set<ecs::Entity> getGlobalEntities() const { return m_globalEntities; };

            void attachCameraToLayer(const std::shared_ptr<camera::Camera> &camera, LayerId id);

            void detachCameraFromLayer(LayerId id);

            std::shared_ptr<camera::Camera> getCameraLayer(LayerId id);

            void setLayerRenderStatus(bool status, LayerId id);

            void setLayerActiveStatus(bool status, LayerId id);

            [[nodiscard]] bool getLayerRenderStatus(LayerId id) const;

            [[nodiscard]] bool getLayerActiveStatus(LayerId id) const;

            void setWindowOffset(const glm::vec2 offset) { m_windowOffset = offset; };
            [[nodiscard]] glm::vec2 &getWindowOffset() { return m_windowOffset; };

            unsigned int addLight(const std::shared_ptr<components::Light> &light);

            void removeLight(unsigned int index);

            void setAmbientLight(const float ambient) { ambientLight = ambient; };
            [[nodiscard]] float getAmbientLight() const { return ambientLight; };

            void onUpdate(Timestep timestep);

            void onRender();

            virtual void dispatchEventToLayers(event::IEvent &event);

            [[nodiscard]] const layer::LayerStack &getLayers() const { return m_layerStack; };

            SceneId id;

            std::string name;

            bool isActive = true;
            bool isRendered = true;

        private:
            layer::LayerStack m_layerStack;
            std::set<ecs::Entity> m_globalEntities;
            glm::vec2 m_windowOffset = glm::vec2(0.0f);
            std::shared_ptr<renderer::RendererContext> m_rendererContext;

            std::array<std::shared_ptr<components::Light>, MAX_POINT_LIGHTS + MAX_DIRECTIONAL_LIGHTS> m_lights;
            unsigned int nbLights = 0;
            unsigned int nbPointLights = 0;
            unsigned int nbDirectionalLights = 0;
            float ambientLight = 0.5f;

    };
}
