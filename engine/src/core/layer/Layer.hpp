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

#include <Nexo.hpp>
#include <unordered_set>
#include <typeindex>
#include <set>
#include <optional>

#include "core/event/Listener.hpp"
#include "core/event/Event.hpp"
#include "core/Timestep.hpp"
#include "ecs/Entity.hpp"
#include "core/camera/OrthographicCameraController.hpp"
#include "Logger.hpp"

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

        auto checkListener = [&]<typename T0>([[maybe_unused]] T0 eventType) {
            using EventType = T0;

            if (isListenerOf<LayerType, EventType>())
            {
                listenedTypes.insert(std::type_index(typeid(EventType)));
            }
        };

        (checkListener(EventTypes{}), ...);

        return listenedTypes;
    }

    class Layer : public event::BaseListener {
        public:
            explicit Layer(const std::string &name) : BaseListener(name), name(name) {};

            ~Layer() override
            {
                LOG(NEXO_DEBUG, "Layer {} deleted", name);
            };

            template<typename DerivedLayer = void>
            std::optional<std::type_index> getTypeIndex() const
            {
                if constexpr (!std::is_same_v<DerivedLayer, void>)
                    return std::type_index(typeid(DerivedLayer));
                return std::nullopt;
            }

            virtual void onAttach() {};
            virtual void onDetach() {};
            virtual void onUpdate([[maybe_unused]] core::Timestep timestep);
            virtual void onRender();

            virtual std::unordered_set<std::type_index> getListenedEventTypes() const { return m_listenedEventTypes; }
            virtual void handleEvent(const event::Event &event);
            template<typename EventType>
            void registerCallbackEventFunction(const std::function<void(const event::Event &)> &callbackFunction)
            {
                const std::type_index typeId(typeid(EventType));
                m_callbackEventFunctions[typeId] = callbackFunction;
            }

            void addEntity(ecs::Entity entity);
            void removeEntity(ecs::Entity entity);
            void entityDestroyed(ecs::Entity entity);

            std::set<ecs::Entity> &getEntities() { return m_entities; };

            void attachCamera(const std::shared_ptr<camera::Camera> &camera);
            void detachCamera();
            std::shared_ptr<camera::Camera> &getCamera() { return m_camera; };

            std::string name;
            bool isRendered = true;
            bool isActive = true;
        protected:
            std::unordered_set<std::type_index> m_listenedEventTypes;
            std::set<ecs::Entity> m_entities;
            std::shared_ptr<camera::Camera> m_camera;

            template<typename T>
            static T &getComponent(const ecs::Entity entity)
            {
                return Application::m_coordinator->getComponent<T>(entity);
            }

            template<typename T>
            static bool hasComponent(const ecs::Entity entity)
            {
                return Application::m_coordinator->entityHasComponent<T>(entity);
            }

        private:
            std::unordered_map<std::type_index, std::function<void(const event::Event &)> > m_callbackEventFunctions;

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
