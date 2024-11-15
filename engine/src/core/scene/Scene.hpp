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

#include "core/layer/LayerStack.hpp"
#include "core/camera/Camera.hpp"
#include "ecs/Coordinator.hpp"
#include "core/event/Event.hpp"
#include "Logger.hpp"

namespace nexo::scene {
    using SceneId = unsigned int;

    class Scene {
        public:
            Scene() : id(0), name("default") {};
            explicit Scene(const SceneId id, std::string sceneName = "scene") : id(id),
                                                                                name(std::move(sceneName))
            {
                LOG(NEXO_INFO, "Scene {} created with id: {}", name, id);
            };
            ~Scene()
            {
                LOG(NEXO_DEBUG, "Scene {} deleted", name);
            };

            [[nodiscard]] const std::string &getName() const { return name; }

            void addLayer(const std::string &layerName);
            void addOverlay(const std::string &overlayName);
            void removeLayer(const std::string &layerName);
            void removeOverlay(const std::string &overlayName);

            [[nodiscard]] std::shared_ptr<layer::Layer> getLayer(const std::string &layerName) const;
            [[nodiscard]] const layer::LayerStack &getLayerStack() const {return m_layerStack; };

            void addEntityToLayer(ecs::Entity entity, const std::string &layerName);
            void addGlobalEntity(ecs::Entity entity);

            void removeEntityFromLayer(ecs::Entity entity, const std::string &layerName);
            void removeGlobalEntity(ecs::Entity entity);

            void entityDestroyed(ecs::Entity entity);

            [[nodiscard]] std::set<ecs::Entity> getEntities() const;
            [[nodiscard]] std::set<ecs::Entity> getGlobalEntities() const {return m_globalEntities; };

            void attachCameraToLayer(const std::shared_ptr<camera::Camera> &camera, const std::string &layerName);
            void detachCameraFromLayer(const std::string &layerName);

            std::shared_ptr<camera::Camera> getCameraLayer(const std::string &layerName);

            void setLayerRenderStatus(bool status, const std::string &layerName);
            void setLayerActiveStatus(bool status, const std::string &layerName);

            [[nodiscard]] bool getLayerRenderStatus(const std::string &layerName) const;
            [[nodiscard]] bool getLayerActiveStatus(const std::string &layerName) const;

            void setWindowOffset(const glm::vec2 offset) { m_windowOffset = offset; };
            [[nodiscard]] glm::vec2 &getWindowOffset() { return m_windowOffset; };

            void onUpdate(core::Timestep timestep);
            void onRender();

            void dispatchEventToLayers(event::Event &event);

            [[nodiscard]] const layer::LayerStack &getLayers() const { return m_layerStack; };

            SceneId id;

            std::string name;

            bool isActive = true;
            bool isRendered = true;
        private:
            layer::LayerStack m_layerStack;
            std::set<ecs::Entity> m_globalEntities;
            glm::vec2 m_windowOffset = glm::vec2(0.0f);
    };
}
