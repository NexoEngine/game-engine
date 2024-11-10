//// Layer.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the layer class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <unordered_set>
#include <typeindex>
#include <set>
#include <optional>

#include "core/event/Listener.hpp"
#include "core/event/Event.hpp"
#include "core/Timestep.hpp"
#include "ecs/Entity.hpp"
#include "core/camera/Camera.hpp"

#define LAYER_LISTENS_TO(DerivedLayer, ...) public nexo::layer::LayerEventMixin<DerivedLayer, __VA_ARGS__>

namespace nexo::layer {

    template<typename DerivedLayer, typename EventType>
    static constexpr bool isListenerOf()
    {
        return std::is_base_of_v<event::Listener<EventType>, DerivedLayer>;
    }

    template<typename LayerType, typename... EventTypes>
    std::unordered_set<std::type_index> collectListenedEventTypes()
    {
        std::unordered_set<std::type_index> listenedTypes;

        auto checkListener = [&]([[maybe_unused]] auto eventType) {
            using EventType = decltype(eventType);

            if (isListenerOf<LayerType, EventType>())
            {
                listenedTypes.insert(std::type_index(typeid(EventType)));
            }
        };

        (checkListener(EventTypes{}), ...);

        return listenedTypes;
    }

    struct EntityComparator {
        bool operator()(const ecs::Entity& lhs, const ecs::Entity& rhs) const {
            return ecs::getEntityId(lhs) < ecs::getEntityId(rhs);
        }
    };

    class Layer : public event::BaseListener {
        public:
            explicit Layer(const std::string &name = "Layer") : BaseListener(name), m_debugName(name),
                                                                m_layerId(s_nextLayerId++) {};

            ~Layer() override = default;

            unsigned int getId() const { return m_layerId; };

            template<typename DerivedLayer = void>
            std::optional<std::type_index> getTypeIndex() const
            {
                if constexpr (!std::is_same_v<DerivedLayer, void>)
                    return std::type_index(typeid(DerivedLayer));
                return std::nullopt;
            }

            virtual void onAttach() {};
            virtual void onDetach() {};
            virtual void onUpdate([[maybe_unused]] core::Timestep timestep) {};
            virtual void onRender() {};

            virtual std::unordered_set<std::type_index> getListenedEventTypes() const
            {
                return m_listenedEventTypes;
            }

            virtual void handleEvent(const event::Event &event)
            {
                if (const auto handler = m_callbackEventFunctions.at(typeid(event)))
                    handler(event);
            }

            template<typename EventType>
            void registerCallbackEventFunction(const std::function<void(const event::Event &)> &callbackFunction)
            {
                const std::type_index typeId(typeid(EventType));
                m_callbackEventFunctions[typeId] = callbackFunction;
            }

            void addEntity(const ecs::Entity entity) { m_entities.insert(entity); };

            void removeEntity(const ecs::Entity entity)
            {
                const auto it = std::find_if(m_entities.begin(), m_entities.end(),
                    [entity](const ecs::Entity storedEntity) {
                            return ecs::getEntityId(storedEntity) == entity;
                });
                if (it != m_entities.end())
                    m_entities.erase(it);
                else
                    std::cout << "LAYER::addEntity: Entity " << entity << " does not exist." << std::endl;
            };

            void entityDestroyed(const ecs::Entity entity)
            {
                const auto it = std::find_if(m_entities.begin(), m_entities.end(),
                    [entity](const ecs::Entity storedEntity) {
                            return ecs::getEntityId(storedEntity) == entity;
                });
                if (it != m_entities.end())
                    m_entities.erase(it);
            }

            void setEntityRenderStatus(const ecs::Entity entity, const bool status)
            {
                const auto it = std::find_if(m_entities.begin(), m_entities.end(),
                                             [entity](const ecs::Entity storedEntity) {
                                                 return ecs::getEntityId(storedEntity) == ecs::getEntityId(entity); // Ensure comparison is on raw ID
                                             });

                if (it != m_entities.end()) {
                    ecs::Entity modifiedEntity = *it;
                    m_entities.erase(it);
                    ecs::setRendered(modifiedEntity, status);
                    m_entities.insert(modifiedEntity);
                } else {
                    std::cout << "LAYER::setEntityRenderStatus: Entity " << entity << " does not exist." << std::endl;
                }
            }

            bool isEntityRendered(const ecs::Entity entity) const
            {
                const auto it = std::find_if(m_entities.begin(), m_entities.end(),
                             [entity](const ecs::Entity storedEntity) {
                                 return ecs::getEntityId(storedEntity) == ecs::getEntityId(entity);
                             });

                if (it != m_entities.end()) {
                    return ecs::isRendered(*it);
                }
                std::cout << "LAYER::isEntityRendered: Entity " << entity << " does not exist." << std::endl;
                return false;
            };

            void setEntityActiveStatus(const ecs::Entity entity, const bool status)
            {
                const auto it = std::find_if(m_entities.begin(), m_entities.end(),
                             [entity](const ecs::Entity storedEntity) {
                                 return ecs::getEntityId(storedEntity) == ecs::getEntityId(entity);
                             });

                if (it != m_entities.end()) {
                    ecs::Entity modifiedEntity = *it;
                    m_entities.erase(it);
                    ecs::setActive(modifiedEntity, status);
                    m_entities.insert(modifiedEntity);
                } else {
                    std::cout << "LAYER::setEntityActiveStatus: Entity " << entity << " does not exist." << std::endl;
                }
            };

            bool isEntityActive(const ecs::Entity entity) const
            {
                const auto it = std::find_if(m_entities.begin(), m_entities.end(),
                     [entity](const ecs::Entity storedEntity) {
                         return ecs::getEntityId(storedEntity) == ecs::getEntityId(entity);
                });

                if (it != m_entities.end()) {
                    return ecs::isActive(*it);
                }
                std::cout << "LAYER::isEntityActive: Entity " << entity << " does not exist." << std::endl;
                return false;
            }

            std::set<ecs::Entity, EntityComparator> &getEntities() { return m_entities; };

            void attachCamera(const std::shared_ptr<camera::Camera> &camera) { m_camera = camera; };
            void detachCamera() { m_camera.reset(); };
            std::shared_ptr<camera::Camera> &getCamera() { return m_camera; };

            const std::string &getName() const { return m_debugName; };

            void setRenderStatus(const bool status)
            {
                m_isRendered = status;
                for (ecs::Entity entity: m_entities)
                    ecs::setRendered(entity, status);
            };

            void setActiveStatus(const bool status)
            {
                m_isActive = status;
                for (ecs::Entity entity: m_entities)
                    ecs::setActive(entity, status);
            };
            bool isRendered() const { return m_isRendered; };
            bool isActive() const { return m_isActive; };

        protected:
            std::string m_debugName;
            std::unordered_set<std::type_index> m_listenedEventTypes;
            std::set<ecs::Entity, EntityComparator> m_entities;
            std::shared_ptr<camera::Camera> m_camera;
            bool m_isRendered = true;
            bool m_isActive = true;

        private:
            std::unordered_map<std::type_index, std::function<void(const event::Event &)> > m_callbackEventFunctions;
            static inline unsigned int s_nextLayerId = 0;
            unsigned int m_layerId;
    };

    template<typename DerivedLayer, typename... EventTypes>
    class LayerEventMixin : public virtual Layer, public event::Listener<EventTypes>... {
        public:
            LayerEventMixin()
            {
                m_listenedEventTypes = collectListenedEventTypes<DerivedLayer, EventTypes...>();
            };

            std::unordered_set<std::type_index> getListenedEventTypes() const override
            {
                return m_listenedEventTypes;
            }

        private:
            std::unordered_set<std::type_index> m_listenedEventTypes;
    };

}
