//// Entity.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the entity class
//
///////////////////////////////////////////////////////////////////////////////

#include "Entity.hpp"
#include "ECSExceptions.hpp"

#include <algorithm>
#include <ranges>

namespace nexo::ecs {

    /**
     * @class EntityManager
     * @brief Manages the creation, destruction, and signatures of entities in the ECS.
     *
     * The EntityManager is responsible for:
     * - Creating and destroying entities
     * - Managing entity signatures
     * - Keeping track of living entities
     */
    EntityManager::EntityManager()
    {
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) m_availableEntities.push_back(entity);
    }

    /**
     * @brief Creates a new entity and assigns it a unique ID.
     *
     * This function retrieves an available entity ID from the pool of unused entities,
     * removes it from the list of available entities, and adds it to the list of living entities.
     * If the maximum number of entities is exceeded, an exception is thrown.
     *
     * @throws TooManyEntities If the number of living entities exceeds the maximum allowed.
     * @return Entity The unique ID of the newly created entity.
     */
    Entity EntityManager::createEntity()
    {
        if (m_livingEntities.size() >= MAX_ENTITIES) THROW_EXCEPTION(TooManyEntities);

        const Entity id = m_availableEntities.front();
        m_availableEntities.pop_front();
        m_livingEntities.push_back(id);

        return id;
    }

    /**
     * @brief Destroys an entity, freeing its ID for future use.
     *
     * This function removes the entity from the list of living entities,
     * resets its signature, and adds its ID back to the pool of available entities.
     * If the entity ID is out of range or does not exist, an exception is thrown.
     *
     * @param entity The ID of the entity to be destroyed.
     * @throws OutOfRange If the entity ID exceeds the maximum allowed.
     */
    void EntityManager::destroyEntity(const Entity entity)
    {
        if (entity >= MAX_ENTITIES) THROW_EXCEPTION(OutOfRange, entity);

        const auto it = std::ranges::find(m_livingEntities, entity);
        if (it != m_livingEntities.end())
            m_livingEntities.erase(it);
        else
            return;
        m_signatures[entity].reset();

        m_availableEntities.push_front(entity);
    }

    /**
     * @brief Sets the signature for a specific entity.
     *
     * This function updates the component signature associated with the given entity ID.
     * If the entity ID is out of range, an exception is thrown.
     *
     * @param entity The ID of the entity whose signature is to be set.
     * @param signature The new signature to associate with the entity.
     * @throws OutOfRange If the entity ID exceeds the maximum allowed.
     */
    void EntityManager::setSignature(const Entity entity, const Signature signature)
    {
        if (entity >= MAX_ENTITIES) THROW_EXCEPTION(OutOfRange, entity);

        m_signatures[entity] = signature;
    }

    /**
     * @brief Retrieves the signature of a specific entity.
     *
     * This function returns the component signature associated with the given entity ID.
     * If the entity ID is out of range, an exception is thrown.
     *
     * @param entity The ID of the entity whose signature is to be retrieved.
     * @throws OutOfRange If the entity ID exceeds the maximum allowed.
     * @return Signature The signature associated with the entity.
     */
    Signature EntityManager::getSignature(const Entity entity) const
    {
        if (entity >= MAX_ENTITIES) THROW_EXCEPTION(OutOfRange, entity);

        return m_signatures[entity];
    }

    /**
     * @brief Gets the current count of living entities.
     *
     * This function returns the number of entities that are currently active (living)
     * in the ECS. It provides a way to monitor the total number of entities being managed.
     *
     * @return size_t The count of living entities.
     */
    size_t EntityManager::getLivingEntityCount() const
    {
        return m_livingEntities.size();
    }

    /**
     * @brief Retrieves a span of all currently living entities.
     *
     * This function returns a read-only span containing the IDs of all entities
     * that are currently active (living) in the ECS. It allows for efficient
     * iteration over the list of living entities without copying the data.
     *
     * @return std::span<const Entity> A span of living entity IDs.
     */
    std::span<const Entity> EntityManager::getLivingEntities() const
    {
        return {m_livingEntities};
    }

} // namespace nexo::ecs
