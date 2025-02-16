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

        glm::vec3 ambientLight;
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

            virtual ~Scene()
            {
                LOG(NEXO_DEBUG, "Scene {} deleted", name);
            };

            [[nodiscard]] const std::string &getName() const { return name; }

            void addLayer(LayerId layerId, const std::string &layerName);

            void addOverlay(LayerId layerId, const std::string &overlayName);

            void removeLayer(LayerId layerId);

            void removeOverlay(LayerId layerId);

            [[nodiscard]] std::shared_ptr<layer::Layer> getLayer(LayerId layerId) const;

            [[nodiscard]] const layer::LayerStack &getLayerStack() const { return m_layerStack; };

            void addEntityToLayer(ecs::Entity entity, LayerId layerId);

            void addGlobalEntity(ecs::Entity entity);

            void removeEntityFromLayer(ecs::Entity entity, LayerId layerId);

            void removeGlobalEntity(ecs::Entity entity);

            void entityDestroyed(ecs::Entity entity);

            [[nodiscard]] std::set<ecs::Entity> getEntities() const;

            [[nodiscard]] std::set<ecs::Entity> getRenderedEntities() const;

            [[nodiscard]] std::set<ecs::Entity> getGlobalEntities() const { return m_globalEntities; };

            void attachCameraToLayer(const std::shared_ptr<camera::Camera> &camera, LayerId id);

            void detachCameraFromLayer(LayerId layerId);

            std::shared_ptr<camera::Camera> getCameraLayer(LayerId layerId);

            void setLayerRenderStatus(bool status, LayerId layerId);

            void setLayerActiveStatus(bool status, LayerId layerId);

            [[nodiscard]] bool getLayerRenderStatus(LayerId layerId) const;

            [[nodiscard]] bool getLayerActiveStatus(LayerId layerId) const;

            void setWindowOffset(const glm::vec2 offset) { m_windowOffset = offset; };
            [[nodiscard]] glm::vec2 &getWindowOffset() { return m_windowOffset; };

            unsigned int addLight(const std::shared_ptr<components::Light> &light);

            void removeLight(unsigned int index);

            void setAmbientLight(const glm::vec3 ambient) { ambientLight = ambient; };
            [[nodiscard]] glm::vec3 getAmbientLight() const { return ambientLight; };

            void onUpdate(Timestep timestep) const;

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
            glm::vec3 ambientLight = glm::vec3(0.2f);

    };
}
