//// ComponentArray.cpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/06/2025
//  Description: Source file for the component array class
//
///////////////////////////////////////////////////////////////////////////////

#include "ComponentArray.hpp"

namespace nexo::ecs {

    TypeErasedComponentArray::TypeErasedComponentArray(const size_t componentSize, const size_t initialCapacity): m_componentSize(componentSize), m_capacity(initialCapacity)
    {
        if (componentSize == 0) {
            throw std::invalid_argument("Component size cannot be zero");
        }

        m_sparse.resize(m_capacity, INVALID_ENTITY);
        m_dense.reserve(m_capacity);
        m_componentData.reserve(m_capacity * m_componentSize);
    }

    void TypeErasedComponentArray::insert(const Entity entity, const void* componentData)
    {
        insertRaw(entity, componentData);
    }

    void TypeErasedComponentArray::insertRaw(Entity entity, const void* componentData)
    {
        if (entity >= MAX_ENTITIES)
            THROW_EXCEPTION(OutOfRange, entity);

        ensureSparseCapacity(entity);

        if (hasComponent(entity)) {
            LOG(NEXO_WARN, "Entity {} already has component", entity);
            return;
        }

        const size_t newIndex = m_size;
        m_sparse[entity]      = newIndex;
        m_dense.push_back(entity);

        // Resize component data vector if needed
        const size_t requiredSize = (m_size + 1) * m_componentSize;
        if (m_componentData.size() < requiredSize) {
            m_componentData.resize(requiredSize);
        }

        // Copy component data
        std::memcpy(m_componentData.data() + newIndex * m_componentSize,
                    componentData, m_componentSize);

        ++m_size;
    }

    void TypeErasedComponentArray::remove(const Entity entity)
    {
        if (!hasComponent(entity))
            THROW_EXCEPTION(ComponentNotFound, entity);

        size_t indexToRemove = m_sparse[entity];

        // Handle grouped components
        if (indexToRemove < m_groupSize) {
            size_t groupLastIndex = m_groupSize - 1;
            if (indexToRemove != groupLastIndex) {
                swapComponents(indexToRemove, groupLastIndex);
                std::swap(m_dense[indexToRemove], m_dense[groupLastIndex]);
                m_sparse[m_dense[indexToRemove]]  = indexToRemove;
                m_sparse[m_dense[groupLastIndex]] = groupLastIndex;
            }
            --m_groupSize;
            indexToRemove = groupLastIndex;
        }

        // Standard removal
        const size_t lastIndex = m_size - 1;
        if (indexToRemove != lastIndex) {
            swapComponents(indexToRemove, lastIndex);
            std::swap(m_dense[indexToRemove], m_dense[lastIndex]);
            m_sparse[m_dense[indexToRemove]] = indexToRemove;
        }

        m_sparse[entity] = INVALID_ENTITY;
        m_dense.pop_back();
        --m_size;

        shrinkIfNeeded();
    }

    bool TypeErasedComponentArray::hasComponent(const Entity entity) const
    {
        return (entity < m_sparse.size() && m_sparse[entity] != INVALID_ENTITY);
    }

    void TypeErasedComponentArray::entityDestroyed(const Entity entity)
    {
        if (hasComponent(entity))
            remove(entity);
    }

    void TypeErasedComponentArray::duplicateComponent(const Entity sourceEntity, const Entity destEntity)
    {
        if (!hasComponent(sourceEntity))
            THROW_EXCEPTION(ComponentNotFound, sourceEntity);

        const void* sourceData = getRawComponent(sourceEntity);
        insert(destEntity, sourceData);
    }

    size_t TypeErasedComponentArray::getComponentSize() const
    {
        return m_componentSize;
    }

    size_t TypeErasedComponentArray::size() const
    {
        return m_size;
    }

    void* TypeErasedComponentArray::getRawComponent(const Entity entity)
    {
        if (!hasComponent(entity))
            return nullptr;
        return m_componentData.data() + m_sparse[entity] * m_componentSize;
    }

    const void* TypeErasedComponentArray::getRawComponent(const Entity entity) const
    {
        if (!hasComponent(entity))
            return nullptr;
        return m_componentData.data() + m_sparse[entity] * m_componentSize;
    }

    void* TypeErasedComponentArray::getRawData()
    {
        return m_componentData.data();
    }

    const void* TypeErasedComponentArray::getRawData() const
    {
        return m_componentData.data();
    }

    std::span<const Entity> TypeErasedComponentArray::entities() const
    {
        return {m_dense.data(), m_size};
    }

    Entity TypeErasedComponentArray::getEntityAtIndex(const size_t index) const
    {
        if (index >= m_size)
            THROW_EXCEPTION(OutOfRange, index);
        return m_dense[index];
    }

    void TypeErasedComponentArray::addToGroup(const Entity entity)
    {
        if (!hasComponent(entity))
            THROW_EXCEPTION(ComponentNotFound, entity);

        const size_t index = m_sparse[entity];
        if (index < m_groupSize)
            return;

        if (index != m_groupSize) {
            swapComponents(index, m_groupSize);
            std::swap(m_dense[index], m_dense[m_groupSize]);
            m_sparse[m_dense[index]]       = index;
            m_sparse[m_dense[m_groupSize]] = m_groupSize;
        }
        ++m_groupSize;
    }

    void TypeErasedComponentArray::removeFromGroup(const Entity entity)
    {
        if (!hasComponent(entity))
            THROW_EXCEPTION(ComponentNotFound, entity);

        const size_t index = m_sparse[entity];
        if (index >= m_groupSize)
            return;

        --m_groupSize;
        if (index != m_groupSize) {
            swapComponents(index, m_groupSize);
            std::swap(m_dense[index], m_dense[m_groupSize]);
            m_sparse[m_dense[index]]       = index;
            m_sparse[m_dense[m_groupSize]] = m_groupSize;
        }
    }

    constexpr size_t TypeErasedComponentArray::groupSize() const
    {
        return m_groupSize;
    }

    size_t TypeErasedComponentArray::memoryUsage() const
    {
        return m_componentData.capacity()
               + sizeof(size_t) * m_sparse.capacity()
               + sizeof(Entity) * m_dense.capacity();
    }

    void TypeErasedComponentArray::ensureSparseCapacity(const Entity entity)
    {
        if (entity >= m_sparse.size()) {
            size_t newSize = m_sparse.size();
            if (newSize == 0)
                newSize = m_capacity;
            while (entity >= newSize)
                newSize *= 2;
            m_sparse.resize(newSize, INVALID_ENTITY);
        }
    }

    void TypeErasedComponentArray::swapComponents(const size_t index1, const size_t index2)
    {
        if (index1 == index2) return;

        std::byte* data1 = m_componentData.data() + index1 * m_componentSize;
        std::byte* data2 = m_componentData.data() + index2 * m_componentSize;

        // Use a temporary buffer for swapping
        std::vector<std::byte> temp(m_componentSize);
        std::memcpy(temp.data(), data1, m_componentSize);
        std::memcpy(data1, data2, m_componentSize);
        std::memcpy(data2, temp.data(), m_componentSize);
    }

    void TypeErasedComponentArray::shrinkIfNeeded()
    {
        if (m_size < m_componentData.capacity() / 4 && m_componentData.capacity() > m_capacity * m_componentSize * 2) {
            size_t newCapacity = std::max(m_size * 2, m_capacity) * m_componentSize;
            if (newCapacity < m_capacity * m_componentSize)
                newCapacity = m_capacity * m_componentSize;

            m_componentData.shrink_to_fit();
            m_dense.shrink_to_fit();

            m_componentData.reserve(newCapacity);
            m_dense.reserve(newCapacity / m_componentSize);
        }
    }

} // namespace nexo::ecs
