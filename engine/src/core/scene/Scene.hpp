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
#include "core/Logger.hpp"

namespace nexo::scene {
    using SceneId = unsigned int;

    class Scene {
        public:
            Scene() : id(0), m_debugSceneName("default") {};
            explicit Scene(const SceneId id, std::string sceneName = "scene") : id(id),
                                                                                m_debugSceneName(std::move(sceneName))
            {
                LOG(NEXO_INFO, "Scene {} created with id: {}", m_debugSceneName, id);
            };

            ~Scene() = default;

            void addLayer(const std::shared_ptr<layer::Layer> &layer);

            void addOverlay(const std::shared_ptr<layer::Layer> &overlay);

            void removeLayer(const std::shared_ptr<layer::Layer> &layer);

            void removeOverlay(const std::shared_ptr<layer::Layer> &overlay);

            template<typename DerivedLayer = void>
            [[nodiscard]] std::shared_ptr<layer::Layer> getLayer() const
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                            return layer;
                    }
                }
                LOG(NEXO_WARN, "Scene(getLayer): Scene {} does not hold this layer type", m_debugSceneName);
                return nullptr;
            }

            [[nodiscard]] std::shared_ptr<layer::Layer> getLayerAt(unsigned int index) const;

            template<typename DerivedLayer = void>
            void addEntity(const ecs::Entity entity)
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                        {
                            LOG(NEXO_DEV, "Scene(addEntity): entity {} added to scene({})'s layer {}", entity, m_debugSceneName, layer->getName());
                            layer->addEntity(entity);
                            return;
                        }
                    }
                }
                LOG(NEXO_WARN, "Scene(addEntity): Scene {} does not hold this layer type, entity added to global scene entities", m_debugSceneName);
                m_globalEntities.insert(entity);
            }

            void addEntity(ecs::Entity entity, int layerIndex = -1);

            template<typename DerivedLayer = void>
            void removeEntity(const ecs::Entity entity)
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                        {
                            layer->removeEntity(entity);
                            LOG(NEXO_DEV, "Scene(removeEntity): entity {} removed from scene({})'s layer {}", entity, m_debugSceneName, layer->getName());
                            return;
                        }
                    }
                }
                LOG(NEXO_WARN, "Scene(removeEntity): Scene {} does not hold this layer type, entity removed from global scene entities", m_debugSceneName);
                m_globalEntities.erase(entity);
            }

            void removeEntity(ecs::Entity entity, int layerIndex = -1);

            void entityDestroyed(ecs::Entity entity);

            [[nodiscard]] std::set<ecs::Entity> getEntities() const;

            template<typename DerivedLayer = void>
            void attachCamera(std::shared_ptr<camera::Camera> &camera)
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                        {
                            layer->attachCamera(camera);
                            LOG(NEXO_DEV, "Scene(attachCamera): camera added to scene({})'s layer {}", m_debugSceneName, layer->getName());
                            return;
                        }
                    }
                }
                LOG(NEXO_WARN, "Scene(attachCamera): Scene {} does not hold this layer type", m_debugSceneName);
            }

            void attachCamera(const std::shared_ptr<camera::Camera> &camera, unsigned int layerIndex);

            template<typename DerivedLayer = void>
            void detachCamera()
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                        {
                            layer->detachCamera();
                            LOG(NEXO_DEV, "Scene(detachCamera): camera removed from scene({})'s layer {}", m_debugSceneName, layer->getName());
                            return;
                        }
                    }
                }
                LOG(NEXO_WARN, "Scene(detachCamera): Scene {} does not hold this layer type", m_debugSceneName);
            }

            void detachCamera(unsigned int layerIndex);

            template<typename DerivedLayer = void>
            std::shared_ptr<camera::Camera> getCamera()
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                        {
                            return layer->getCamera();
                        }
                    }
                }
                LOG(NEXO_WARN, "Scene(getCamera): Scene {} does not hold this layer type", m_debugSceneName);
                return nullptr;
            }

            template<typename DerivedLayer = void>
            void setLayerRenderStatus(const bool status)
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                        {
                            layer->setRenderStatus(status);
                            std::string strStatus = status ? "rendered" : "not rendered";
                            LOG(NEXO_DEV, "Scene(setLayerRenderStatus): scene({})'s layer {} is now {}", m_debugSceneName, layer->getName(), strStatus);
                            return;
                        }
                    }
                }
                LOG(NEXO_WARN, "Scene(setLayerRenderStatus): Scene {} does not hold this layer type", m_debugSceneName);
            }

            void setLayerRenderStatus(bool status, unsigned int layerIndex);

            template<typename DerivedLayer = void>
            void setLayerActiveStatus(const bool status)
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                        {
                            layer->setActiveStatus(status);
                            std::string strStatus = status ? "active" : "unactive";
                            LOG(NEXO_DEV, "Scene(setLayerActiveStatus): scene({})'s layer {} is now {}", m_debugSceneName, layer->getName(), strStatus);
                            return;
                        }
                    }
                }
                LOG(NEXO_WARN, "Scene(setLayerActiveStatus): Scene {} does not hold this layer type", m_debugSceneName);
            }

            void setLayerActiveStatus(bool status, unsigned int layerIndex);

            template<typename DerivedLayer = void>
            [[nodiscard]] bool getLayerRenderStatus() const
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                            return layer->isRendered();
                    }
                }
                LOG(NEXO_WARN, "Scene(getLayerRenderStatus): Scene {} does not hold this layer type", m_debugSceneName);
                return false;
            }

            [[nodiscard]] bool getLayerRenderStatus(unsigned int layerIndex) const;

            template<typename DerivedLayer = void>
            [[nodiscard]] bool getLayerActiveStatus() const
            {
                for (const auto &layer: m_layerStack)
                {
                    if constexpr (!std::is_same_v<DerivedLayer, void>)
                    {
                        if (layer->getTypeIndex<DerivedLayer>() == std::type_index(typeid(DerivedLayer)))
                            return layer->isActive();
                    }
                }
                LOG(NEXO_WARN, "Scene(getLayerActiveStatus): Scene {} does not hold this layer type", m_debugSceneName);
                return false;
            }

            [[nodiscard]] bool getLayerActiveStatus(unsigned int layerIndex) const;

            std::shared_ptr<camera::Camera> &getCamera(unsigned int layerIndex);

            void setWindowOffset(const glm::vec2 offset) { m_windowOffset = offset; };
            [[nodiscard]] glm::vec2 &getWindowOffset() { return m_windowOffset; };

            void onUpdate(core::Timestep timestep);
            void onRender();

            void dispatchEventToLayers(event::Event &event);

            [[nodiscard]] const layer::LayerStack &getLayers() const { return m_layerStack; };

            void setActiveStatus(bool status);
            [[nodiscard]] bool isActive() const { return m_isActive; };
            void setRenderStatus(bool status);
            [[nodiscard]] bool isRendered() const { return m_isRendered; };
            SceneId id;

        private:
            std::string m_debugSceneName;
            layer::LayerStack m_layerStack;
            std::set<ecs::Entity> m_globalEntities;
            glm::vec2 m_windowOffset = glm::vec2(0.0f);

            bool m_isActive = true;
            bool m_isRendered = true;
    };
}
