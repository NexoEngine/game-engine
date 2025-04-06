#pragma once

#include "System.hpp"
#include "Access.hpp"
#include "ComponentArray.hpp"
#include "Coordinator.hpp"
#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace nexo::ecs {
    /**
     * @class QuerySystem
     * @brief System that directly queries component arrays
     *
     * @tparam Components Component access specifiers (Read<T> or Write<T>)
     */
    template<typename... Components>
    class QuerySystem : public AQuerySystem {
    private:
        // Helper template to check if a component type exists in the parameter pack with Read access
        template<typename T, typename First, typename... Rest>
        struct HasReadAccess {
            static constexpr bool value =
                (std::is_same_v<First, Read<T>> ||
                 HasReadAccess<T, Rest...>::value);
        };

        // Base case for the template recursion
        template<typename T, typename First>
        struct HasReadAccess<T, First> {
            static constexpr bool value = std::is_same_v<First, Read<T>>;
        };

        // Convenience function to check read access
        template<typename T>
        static constexpr bool hasReadAccess() {
            return HasReadAccess<T, Components...>::value;
        }

    public:
        QuerySystem() {
            if (!coord) return;

            // Set system signature based on required components
            (setComponentSignature<typename Components::ComponentType>(m_signature), ...);

            // Store component arrays for faster access
            (cacheComponentArray<Components>(), ...);
        }

        /**
         * @brief Get a component for an entity with access type determined at compile time
         *
         * @tparam T The component type
         * @param entity The entity to get the component from
         * @return Reference to the component with appropriate const-ness
         */
        template<typename T>
        typename std::conditional<hasReadAccess<T>(), const T&, T&>::type
        getComponent(Entity entity) {
            auto typeIndex = getTypeIndex<T>();
            auto componentArray = std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeIndex]);

            if constexpr (hasReadAccess<T>()) {
                return componentArray->getData(entity);
            } else {
                return componentArray->getData(entity);
            }
        }

        const Signature &getSignature() const {
            return m_signature;
        }

        Signature &getSignature() {
            return m_signature;
        }

    protected:
        /**
         * @brief Caches component arrays for faster access
         *
         * @tparam ComponentAccessType The component access type to cache
         */
        template<typename ComponentAccessType>
        void cacheComponentArray() {
            using T = typename ComponentAccessType::ComponentType;
            m_componentArrays[getTypeIndex<T>()] = coord->getComponentArray<T>();
        }

        /**
         * @brief Sets the component bit in the system signature
         *
         * @tparam T The component type
         * @param signature The signature to modify
         */
        template<typename T>
        void setComponentSignature(Signature& signature) {
            signature.set(coord->getComponentType<T>(), true);
        }

        /**
         * @brief Gets the type index for a component
         *
         * @tparam T The component type
         * @return std::type_index The type index
         */
        template<typename T>
        std::type_index getTypeIndex() const {
            return std::type_index(typeid(T));
        }

    private:
        // Cache of component arrays for faster access
        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_componentArrays;
        Signature m_signature;
    };
}
