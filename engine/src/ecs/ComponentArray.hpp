//// ComponentArray.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        01/04/2025
//  Description: Header file for the component array
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Definitions.hpp"
#include "ECSExceptions.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include <vector>
#include <span>

namespace nexo::ecs {
    /**
     * @class IComponentArray
     * @brief Base interface for all component array types.
     *
     * Provides the common interface that all concrete component arrays must implement,
     * allowing type-erased storage and manipulation of components.
     *
     * @note This class is not thread-safe. Access to a ComponentArray should be
     *       synchronized externally when used in multi-threaded contexts.
     */
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;

        /**
         * @brief Checks if an entity has a component in this array
         * @param entity The entity to check
         * @return true if the entity has a component, false otherwise
         */
        [[nodiscard]] virtual bool hasComponent(Entity entity) const = 0;

        /**
         * @brief Handles cleanup when an entity is destroyed
         * @param entity The entity being destroyed
         */
        virtual void entityDestroyed(Entity entity) = 0;
    };

    /**
     * @class ComponentArray
     * @brief Stores and manages components of a specific type T.
     *
     * Implements a sparse-dense storage pattern for efficient component storage and retrieval.
     * Components are stored contiguously for cache-friendly access, while maintaining
     * O(1) lookups via entity IDs.
     *
     * @tparam T The component type stored in this array
     * @tparam capacity Initial capacity for the sparse array
     *
     * @note This class is not thread-safe. Access should be synchronized externally when
     *       used in multi-threaded contexts.
     */
    template<typename T, unsigned int capacity = 1024>
    requires (capacity >= 1)
    class alignas(64) ComponentArray final : public IComponentArray {
    public:
        /**
         * @brief Type alias for the component type
         */
        using component_type = T;

        /**
         * @brief Constructs a new component array with initial capacity
         *
         * Initializes the sparse array with capacity elements, and reserves space
         * for the dense arrays.
         */
        ComponentArray()
        {
            m_sparse.resize(capacity, INVALID_ENTITY);
            m_dense.reserve(capacity);
            m_componentArray.reserve(capacity);
        }

        /**
         * @brief Inserts a new component for the given entity.
         *
         * @param entity The entity to add the component to
         * @param component The component instance to add
         * @throws OutOfRange if entity ID exceeds MAX_ENTITIES
         *
         * @pre The entity must be a valid entity ID
         */
        void insert(Entity entity, T component)
        {
            if (entity >= MAX_ENTITIES)
                THROW_EXCEPTION(OutOfRange, entity);

            // Ensure m_sparse can hold this entity index.
            ensureSparseCapacity(entity);

            if (hasComponent(entity)) {
                LOG(NEXO_WARN, "Entity {} already has component: {}", entity, typeid(T).name());
                return;
            }

            const size_t newIndex = m_size;
            m_sparse[entity] = newIndex;
            m_dense.push_back(entity);
            m_componentArray.push_back(component);

            ++m_size;
        }

        /**
         * @brief Removes the component for the given entity.
         *
         * If the entity is grouped (i.e. within the first m_groupSize entries),
         * then adjusts the group boundary appropriately.
         *
         * @param entity The entity to remove the component from
         * @throws ComponentNotFoundException if the entity doesn't have the component
         *
         * @pre The entity must have the component
         */
        void remove(const Entity entity)
        {
            if (!hasComponent(entity))
                THROW_EXCEPTION(ComponentNotFound, entity);

            size_t indexToRemove = m_sparse[entity];

            // If the entity is part of the group, remove it from the group first.
            if (indexToRemove < m_groupSize) {
                // Swap with the last grouped element if not already at the end.
                size_t groupLastIndex = m_groupSize - 1;
                if (indexToRemove != groupLastIndex) {
                    std::swap(m_componentArray[indexToRemove], m_componentArray[groupLastIndex]);
                    std::swap(m_dense[indexToRemove], m_dense[groupLastIndex]);
                    m_sparse[m_dense[indexToRemove]] = indexToRemove;
                    m_sparse[m_dense[groupLastIndex]] = groupLastIndex;
                }
                --m_groupSize;
                indexToRemove = groupLastIndex;
            }

            // Standard removal from the overall array:
            const size_t lastIndex = m_size - 1;
            if (indexToRemove != lastIndex) {
                std::swap(m_componentArray[indexToRemove], m_componentArray[lastIndex]);
                std::swap(m_dense[indexToRemove], m_dense[lastIndex]);
                m_sparse[m_dense[indexToRemove]] = indexToRemove;
            }
            m_sparse[entity] = INVALID_ENTITY;
            m_componentArray.pop_back();
            m_dense.pop_back();
            --m_size;

            shrinkIfNeeded();
        }

        /**
         * @brief Retrieves a component for the given entity
         *
         * @param entity The entity to get the component from
         * @return Reference to the component
         * @throws ComponentNotFoundException if the entity doesn't have the component
         *
         * @pre The entity must have the component
         */
        [[nodiscard]] T& get(const Entity entity)
        {
            if (!hasComponent(entity))
                THROW_EXCEPTION(ComponentNotFound, entity);
            return m_componentArray[m_sparse[entity]];
        }

        /**
         * @brief Retrieves a component for the given entity (const version)
         *
         * @param entity The entity to get the component from
         * @return Const reference to the component
         * @throws ComponentNotFoundException if the entity doesn't have the component
         *
         * @pre The entity must have the component
         */
        [[nodiscard]] const T& get(const Entity entity) const
        {
            if (!hasComponent(entity))
                THROW_EXCEPTION(ComponentNotFound, entity);
            return m_componentArray[m_sparse[entity]];
        }

        /**
         * @brief Checks if an entity has a component in this array
         *
         * @param entity The entity to check
         * @return true if the entity has a component, false otherwise
         */
        [[nodiscard]] bool hasComponent(const Entity entity) const override
        {
            return (entity < m_sparse.size() && m_sparse[entity] != INVALID_ENTITY);
        }

        /**
         * @brief Removes the component from an entity when it's destroyed
         *
         * @param entity The entity being destroyed
         */
        void entityDestroyed(const Entity entity) override
        {
            if (hasComponent(entity))
                remove(entity);
        }

        /**
         * @brief Gets the total number of components in the array
         *
         * @return The number of active components
         */
        [[nodiscard]] constexpr size_t size() const
        {
            return m_size;
        }

        /**
         * @brief Gets the entity at the given index in the dense array
         *
         * @param index The index to look up
         * @return The entity at that index
         * @throws OutOfRange if the index is invalid
         *
         * @pre The index must be less than the array size
         */
        [[nodiscard]] Entity getEntityAtIndex(const size_t index) const
        {
            if (index >= m_size)
                THROW_EXCEPTION(OutOfRange, index);
            return m_dense[index];
        }

        /**
         * @brief Gets a span view of all components
         *
         * @return Span of component data
         */
        [[nodiscard]] std::span<T> getAllComponents()
        {
            return std::span<T>(m_componentArray.data(), m_size);
        }

        /**
         * @brief Gets a const span view of all components
         *
         * @return Const span of component data
         */
        [[nodiscard]] std::span<const T> getAllComponents() const
        {
            return std::span<const T>(m_componentArray.data(), m_size);
        }

        /**
         * @brief Gets a const span view of all entities with this component
         *
         * @return Const span of entity IDs
         */
        [[nodiscard]] std::span<const Entity> entities() const
        {
            return {m_dense.data(), m_size};
        }

        /**
         * @brief Moves the component for the given entity into the group region.
         *
         * This operation swaps the entity with the one at m_groupSize
         * and then increments the group pointer.
         *
         * @param entity The entity to add to the group
         * @throws ComponentNotFoundException if the entity doesn't have the component
         *
         * @pre The entity must have the component
         */
        void addToGroup(const Entity entity)
        {
            if (!hasComponent(entity))
                THROW_EXCEPTION(ComponentNotFound, entity);

            size_t index = m_sparse[entity];
            if (index < m_groupSize)
                return;
            // Swap with the element at the group boundary.
            if (index != m_groupSize) {
                std::swap(m_componentArray[index], m_componentArray[m_groupSize]);
                std::swap(m_dense[index], m_dense[m_groupSize]);
                m_sparse[m_dense[index]] = index;
                m_sparse[m_dense[m_groupSize]] = m_groupSize;
            }
            ++m_groupSize;
        }

        /**
         * @brief Moves the component for the given entity out of the group region.
         *
         * This operation swaps the entity with the last grouped element
         * and then decrements the group pointer.
         *
         * @param entity The entity to remove from the group
         * @throws ComponentNotFoundException if the entity doesn't have the component
         *
         * @pre The entity must have the component
         */
        void removeFromGroup(const Entity entity)
        {
            if (!hasComponent(entity))
                THROW_EXCEPTION(ComponentNotFound, entity);

            size_t index = m_sparse[entity];
            if (index >= m_groupSize)
                return;
            --m_groupSize;
            if (index != m_groupSize) {
                std::swap(m_componentArray[index], m_componentArray[m_groupSize]);
                std::swap(m_dense[index], m_dense[m_groupSize]);
                m_sparse[m_dense[index]] = index;
                m_sparse[m_dense[m_groupSize]] = m_groupSize;
            }
        }

        /**
         * @brief Forces a component to be set at a specific index (internal use only)
         *
         * Used primarily during group reordering operations.
         *
         * @param index The index to set the component at
         * @param entity The entity to associate with this component
         * @param component The component data to set
         * @throws OutOfRange if the index is invalid
         */
        void forceSetComponentAt(size_t index, const Entity entity, T component)
        {
            if (index >= m_size)
                THROW_EXCEPTION(OutOfRange, index);

            m_sparse[entity] = index;
            m_dense[index] = entity;
            m_componentArray[index] = std::move(component);
        }

        /**
         * @brief Batch insertion of multiple components
         *
         * @tparam EntityIt Iterator type for entities
         * @tparam CompIt Iterator type for components
         * @param entitiesBegin Start iterator for entities
         * @param entitiesEnd End iterator for entities
         * @param componentsBegin Start iterator for components
         *
         * @pre The range [entitiesBegin, entitiesEnd) must be valid
         * @pre componentsBegin must point to a valid range of at least (entitiesEnd - entitiesBegin) elements
         */
        template<typename EntityIt, typename CompIt>
        void insertBatch(EntityIt entitiesBegin, EntityIt entitiesEnd, CompIt componentsBegin)
        {
            CompIt compIt = componentsBegin;
            for (EntityIt entityIt = entitiesBegin; entityIt != entitiesEnd; ++entityIt, ++compIt) {
                insert(*entityIt, *compIt);
            }
        }

        /**
         * @brief Apply a function to each entity-component pair
         *
         * @tparam Func Type of function to apply
         * @param func Function taking (Entity, T&) to apply to each pair
         */
        template<typename Func>
        void forEach(Func&& func)
        {
            for (size_t i = 0; i < m_size; ++i) {
                func(m_dense[i], m_componentArray[i]);
            }
        }

        /**
         * @brief Apply a function to each entity-component pair (const version)
         *
         * @tparam Func Type of function to apply
         * @param func Function taking (Entity, const T&) to apply to each pair
         */
        template<typename Func>
        void forEach(Func&& func) const
        {
            for (size_t i = 0; i < m_size; ++i) {
                func(m_dense[i], m_componentArray[i]);
            }
        }

        /**
         * @brief Gets the number of entities in the group region
         *
         * @return Number of grouped entities
         */
        [[nodiscard]] constexpr size_t groupSize() const
        {
            return m_groupSize;
        }

        /**
         * @brief Get the estimated memory usage of this component array
         *
         * @return Size in bytes of memory used by this component array
         */
        [[nodiscard]] size_t memoryUsage() const
        {
            return sizeof(T) * m_componentArray.capacity()
                            + sizeof(size_t) * m_sparse.capacity()
                            + sizeof(Entity) * m_dense.capacity();
        }

    private:
        // Dense storage for components.
        std::vector<T> m_componentArray;
        // Sparse mapping: maps entity ID to index in the dense arrays.
        std::vector<size_t> m_sparse;
        // Dense storage for entity IDs.
        std::vector<Entity> m_dense;
        // Current number of active components.
        size_t m_size = 0;
        // The first m_groupSize entries in m_dense/m_componentArray are considered "grouped".
        size_t m_groupSize = 0;

        /**
         * @brief Ensures m_sparse is large enough to index 'entity'
         *
         * @param entity The entity to ensure capacity for
         */
        void ensureSparseCapacity(const Entity entity)
        {
            if (entity >= m_sparse.size()) {
                size_t newSize = m_sparse.size();
                if (newSize == 0)
                    newSize = capacity;
                while (entity >= newSize)
                    newSize *= 2;
                m_sparse.resize(newSize, INVALID_ENTITY);
            }
        }

        /**
         * @brief Shrinks vectors if they're significantly larger than needed
         *
         * Reduces memory usage by shrinking the dense vectors when size
         * is less than half of their capacity.
         */
        void shrinkIfNeeded()
        {
            if (m_size < m_componentArray.capacity() / 2 && m_componentArray.capacity() > capacity) {
                // Only shrink if vectors are significantly oversized to avoid frequent reallocations
                auto newCapacity = static_cast<size_t>(m_size * 2);
                if (newCapacity < capacity)
                    newCapacity = capacity;

                m_componentArray.shrink_to_fit();
                m_dense.shrink_to_fit();

                // Reserve the optimized capacity to ensure future growth is efficient
                m_componentArray.reserve(newCapacity);
                m_dense.reserve(newCapacity);
            }
        }
    };
}
