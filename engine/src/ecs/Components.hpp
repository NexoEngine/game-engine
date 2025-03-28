///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the components and component manager class for the ecs
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <type_traits>
#include <vector>

#include "ECSExceptions.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

namespace nexo::ecs {

	// Entity definitions
	using Entity = std::uint32_t;
	constexpr Entity MAX_ENTITIES = 80191;
	constexpr Entity INVALID_ENTITY = std::numeric_limits<Entity>::max();

	// Component type definitions
	using ComponentType = std::uint8_t;
	constexpr ComponentType MAX_COMPONENT_TYPE = 32;

    // Global counter shared across all component types.
    inline ComponentType globalComponentCounter = 0;

    template<typename T>
    ComponentType getUniqueComponentTypeID()
    {
        // This static variable is instantiated once per type T,
        // but it will be assigned a unique value from the shared global counter.
        static const ComponentType id = []() {
            assert(globalComponentCounter < MAX_COMPONENT_TYPE && "Maximum number of component types exceeded");
            return globalComponentCounter++;
        }();
        return id;
    }

	template<typename T>
	ComponentType getComponentTypeID()
	{
	    return getUniqueComponentTypeID<std::remove_cvref_t<T>>();
	}

	/**
	 * @class IComponentArray
	 *
	 * @brief Interface for a component array in the ECS framework.
	 */
	class IComponentArray {
	public:
	    virtual ~IComponentArray() = default;
	    virtual void entityDestroyed(Entity entity) = 0;
	};

	/**
	 * @class ComponentArray<T>
	 *
	 * @brief Component storage using sparse set pattern for O(1) lookup.
	 *
	 * Dense storage is maintained in dynamically growing vectors. The initial capacity is reserved
	 * at 1024 elements by default and then the vectors grow as needed. When many components are removed,
	 * the dense storage is shrunk to 125% of the current size.
	 *
	 * @tparam T - The type of component to store.
	 */
	template<typename T, unsigned int capacity = 1024,
         typename = std::enable_if_t<(capacity >= 1)>>
	class alignas(64) ComponentArray final : public IComponentArray {
		public:
		    ComponentArray()
			{
		        m_sparse.resize(capacity, INVALID_ENTITY);
		        m_dense.reserve(capacity);
		        m_componentArray.reserve(capacity);
		    }

		    void insertData(Entity entity, T component)
			{
		        if (entity >= MAX_ENTITIES)
		            THROW_EXCEPTION(OutOfRange, entity);

		        // Ensure m_sparse can hold this entity index.
		        ensureSparseCapacity(entity);

		        if (hasComponent(entity)) {
		            LOG(NEXO_WARN, "ECS::ComponentArray::insertData: Component already added to entity {}", entity);
		            return;
		        }

		        const size_t newIndex = m_size;
		        m_sparse[entity] = newIndex;
		        m_dense.push_back(entity);
		        m_componentArray.push_back(std::move(component));
		        ++m_size;
		    }

		    void removeData(Entity entity)
			{
		        if (entity >= m_sparse.size() || !hasComponent(entity))
		            THROW_EXCEPTION(ComponentNotFound, entity);

		        const size_t indexToRemove = m_sparse[entity];
		        const size_t lastIndex = m_size - 1;
		        const Entity lastEntity = m_dense[lastIndex];

		        // Swap and pop.
		        m_componentArray[indexToRemove] = std::move(m_componentArray[lastIndex]);
		        m_dense[indexToRemove] = lastEntity;
		        m_sparse[lastEntity] = indexToRemove;

		        m_sparse[entity] = INVALID_ENTITY;
		        m_componentArray.pop_back();
		        m_dense.pop_back();
		        --m_size;

		        shrinkIfNeeded();
		    }

		    [[nodiscard]] T& getData(Entity entity)
			{
		        if (entity >= m_sparse.size() || !hasComponent(entity))
		            THROW_EXCEPTION(ComponentNotFound, entity);
		        return m_componentArray[m_sparse[entity]];
		    }

		    [[nodiscard]] const T& getData(Entity entity) const
			{
		        if (entity >= m_sparse.size() || !hasComponent(entity))
		            THROW_EXCEPTION(ComponentNotFound, entity);
		        return m_componentArray[m_sparse[entity]];
		    }

		    [[nodiscard]] bool hasComponent(Entity entity) const
			{
		        return (entity < m_sparse.size() && m_sparse[entity] != INVALID_ENTITY);
		    }

		    void entityDestroyed(Entity entity) override
			{
		        if (hasComponent(entity))
		            removeData(entity);
		    }

		    [[nodiscard]] size_t size() const
			{
		        return m_size;
		    }

		    [[nodiscard]] Entity getEntityAtIndex(size_t index) const
			{
				if (index >= m_size)
					THROW_EXCEPTION(OutOfRange, index);
		        return m_dense[index];
		    }

		    [[nodiscard]] std::span<T> rawData()
			{
		        return std::span<T>(m_componentArray.data(), m_size);
		    }

		    [[nodiscard]] std::span<const T> rawData() const
			{
		        return std::span<const T>(m_componentArray.data(), m_size);
		    }

		    [[nodiscard]] std::span<const Entity> entities() const
			{
		        return std::span<const Entity>(m_dense.data(), m_size);
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

		    // Ensures m_sparse is large enough to index 'entity'.
		    void ensureSparseCapacity(Entity entity)
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

		    // Shrinks the dense vectors if m_size is less than half of their capacity.
		    // New capacity is set to 125% of m_size, with a minimum of the capacity given.
		    void shrinkIfNeeded()
			{
		        auto shrinkVector = [this](auto& vec) {
		            size_t currentCapacity = vec.capacity();
		            if (m_size < currentCapacity / 2) {
		                size_t newCapacity = static_cast<size_t>(m_size * 1.25);
		                if (newCapacity < capacity)
		                    newCapacity = capacity;
		                std::vector<typename std::decay<decltype(vec)>::type::value_type> newVec;
		                newVec.reserve(newCapacity);
		                newVec.assign(vec.begin(), vec.begin() + m_size);
		                vec.swap(newVec);
		            }
		        };

		        shrinkVector(m_componentArray);
		        shrinkVector(m_dense);
		    }
	};

	/**
	 * @class ComponentManager
	 *
	 * @brief Manages all component arrays and provides access to them.
	 */
	class ComponentManager {
		public:
		    ComponentManager() = default;

		    // Non-copyable
		    ComponentManager(const ComponentManager&) = delete;
		    ComponentManager& operator=(const ComponentManager&) = delete;

		    // Movable
		    ComponentManager(ComponentManager&&) noexcept = default;
		    ComponentManager& operator=(ComponentManager&&) noexcept = default;

		    /**
		     * @brief Registers a component type in the system
		     *
		     * @tparam T The component type to register
		     */
		    template<typename T>
		    void registerComponent()
			{
		        const ComponentType typeID = getComponentTypeID<T>();

		        if (m_componentArrays[typeID] != nullptr) {
		            LOG(NEXO_WARN, "ECS::ComponentManager::registerComponent: Component already registered");
		            return;
		        }

		        m_componentArrays[typeID] = std::make_shared<ComponentArray<T>>();
		    }

		    /**
		     * @brief Gets the component type ID
		     *
		     * @tparam T The component type
		     * @return The component type ID
		     */
		    template<typename T>
		    [[nodiscard]] ComponentType getComponentType() const
			{
		        const ComponentType typeID = getComponentTypeID<T>();

		        if (m_componentArrays[typeID] == nullptr) {
		            THROW_EXCEPTION(ComponentNotRegistered);
		        }

		        return typeID;
		    }

		    /**
		     * @brief Adds a component to an entity
		     *
		     * @tparam T The component type
		     * @param entity The entity to add the component to
		     * @param component The component to add
		     */
		    template<typename T>
		    void addComponent(Entity entity, T component)
			{
		        getComponentArray<T>()->insertData(entity, std::move(component));
		    }

		    /**
		     * @brief Removes a component from an entity
		     *
		     * @tparam T The component type
		     * @param entity The entity to remove the component from
		     */
		    template<typename T>
		    void removeComponent(Entity entity)
			{
		        getComponentArray<T>()->removeData(entity);
		    }

		    /**
		     * @brief Tries to remove a component from an entity
		     *
		     * @tparam T The component type
		     * @param entity The entity to remove the component from
		     * @return true if the component was removed, false if it didn't exist
		     */
		    template<typename T>
		    bool tryRemoveComponent(Entity entity)
			{
		        auto componentArray = getComponentArray<T>();
		        if (!componentArray->hasComponent(entity))
		            return false;

		        componentArray->removeData(entity);
		        return true;
		    }

		    /**
		     * @brief Gets a component from an entity
		     *
		     * @tparam T The component type
		     * @param entity The entity to get the component from
		     * @return Reference to the component
		     */
		    template<typename T>
		    [[nodiscard]] T& getComponent(Entity entity)
			{
		        return getComponentArray<T>()->getData(entity);
		    }

		    /**
		     * @brief Gets a component array
		     *
		     * @tparam T The component type
		     * @return Shared pointer to the component array
		     */
		    template<typename T>
		    [[nodiscard]] std::shared_ptr<ComponentArray<T>> getComponentArray()
			{
		        const ComponentType typeID = getComponentTypeID<T>();

		        auto& componentArray = m_componentArrays[typeID];
		        if (componentArray == nullptr)
		            THROW_EXCEPTION(ComponentNotRegistered);

		        return std::static_pointer_cast<ComponentArray<T>>(componentArray);
		    }

		    /**
		     * @brief Gets a component array (const version)
		     *
		     * @tparam T The component type
		     * @return Shared pointer to the component array
		     */
		    template<typename T>
		    [[nodiscard]] std::shared_ptr<const ComponentArray<T>> getComponentArray() const
			{
		        const ComponentType typeID = getComponentTypeID<T>();

		        const auto& componentArray = m_componentArrays[typeID];
		        if (componentArray == nullptr)
		            THROW_EXCEPTION(ComponentNotRegistered);

		        return std::static_pointer_cast<const ComponentArray<T>>(componentArray);
		    }

		    /**
		     * @brief Tries to get a component from an entity
		     *
		     * @tparam T The component type
		     * @param entity The entity to get the component from
		     * @return Optional reference to the component
		     */
		    template<typename T>
		    [[nodiscard]] std::optional<std::reference_wrapper<T>> tryGetComponent(Entity entity)
			{
		        auto componentArray = getComponentArray<T>();
		        if (!componentArray->hasComponent(entity))
		            return std::nullopt;

		        return componentArray->getData(entity);
		    }

		    /**
		     * @brief Notifies all component arrays that an entity has been destroyed
		     *
		     * @param entity The destroyed entity
		     */
		    void entityDestroyed(Entity entity);

		private:
		    // Use fixed-size array for O(1) component type lookup instead of std::unordered_map
		    std::array<std::shared_ptr<IComponentArray>, MAX_COMPONENT_TYPE> m_componentArrays{};
	};
}
