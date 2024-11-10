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

namespace nexo::scene {
    using SceneId = unsigned int;

    class Scene {
        public:
            Scene() : id(0), m_debugSceneName("default") {};
            explicit Scene(const SceneId id, std::string sceneName = "scene") : id(id),
                                                                                m_debugSceneName(std::move(sceneName))
            {};

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
                            layer->addEntity(entity);
                            return;
                        }
                    }
                }
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
                            return;
                        }
                    }
                }
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
                            return;
                        }
                    }
                }
                std::cout << "ERROR::SCENE::attachCamera: scene does not hold layer type!" << std::endl;
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
                            return;
                        }
                    }
                }
                std::cout << "ERROR::SCENE::detachCamera: scene does not hold layer type!" << std::endl;
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
                std::cout << "ERROR:SCENE::getCamera: scene does not hold layer type!" << std::endl;
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
                            return;
                        }
                    }
                }
                std::cout << "ERROR:SCENE::setLayerRenderStatus: scene does not hold layer type!" << std::endl;
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
                            return;
                        }
                    }
                }
                std::cout << "ERROR:SCENE::setLayerActiveStatus: scene does not hold layer type!" << std::endl;
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
                std::cout << "ERROR:SCENE:getLayerRenderStatus: scene does not hold layer type!" << std::endl;
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
                std::cout << "ERROR:SCENE:getLayerActiveStatus: scene does not hold layer type" << std::endl;
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
