//// System.cpp ///////////////////////////////////////////////////////////////
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
//  Date:        10/11/2024
//  Description: Source file for the system manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "System.hpp"

#include <ranges>

namespace nexo::ecs {

    /**
     * @brief Inserts an entity into the sparse set.
     *
     * If the entity already exists in the set, a warning is logged and no action is taken.
     *
     * @param entity The entity to insert.
     */
    void SparseSet::insert(Entity entity)
    {
        if (contains(entity)) {
            LOG(NEXO_WARN, "Entity {} already added to the sparse set", entity);
            return;
        }

        sparse[entity] = dense.size();
        dense.push_back(entity);
    }

    /**
     * @brief Erases an entity from the sparse set.
     *
     * If the entity does not exist in the set, a warning is logged and no action is taken.
     * The entity is removed by swapping it with the last entity in the dense array
     * and updating the sparse mapping accordingly.
     *
     * @param entity The entity to erase.
     */
    void SparseSet::erase(Entity entity)
    {
        if (!contains(entity)) {
            LOG(NEXO_WARN, "Entity {} does not exist in the sparse set", entity);
            return;
        }

        const size_t index      = sparse[entity];
        const size_t lastIndex  = dense.size() - 1;
        const Entity lastEntity = dense[lastIndex];

        dense[index]       = lastEntity;
        sparse[lastEntity] = index;
        dense.pop_back();
        sparse.erase(entity);
    }

    /**
     * @brief Removes an entity from all systems whose signature matches the entity's signature.
     *
     * This function iterates through all registered systems and checks if the entity's
     * signature matches the system's signature. If a match is found, the entity is
     * removed from the system's set of entities.
     *
     * @param entity The entity to be removed.
     * @param signature The signature of the entity, representing its component composition.
     */
    void SystemManager::entityDestroyed(const Entity entity, const Signature signature) const
    {
        for (const auto& system : std::ranges::views::values(m_querySystems)) {
            if (const Signature& systemSignature = system->getSignature();
                (signature & systemSignature) == systemSignature)
                system->entities.erase(entity);
        }
    }

    /**
     * @brief Updates systems when an entity's signature changes.
     *
     * This function checks each registered system to see if the entity's new signature
     * qualifies it for inclusion in the system based on the system's signature. If the
     * entity now qualifies but did not before, it is added to the system. Conversely,
     * if the entity no longer qualifies but did before, it is removed from the system.
     *
     * @param entity The entity whose signature has changed.
     * @param oldSignature The previous signature of the entity.
     * @param newSignature The updated signature of the entity.
     */
    void SystemManager::entitySignatureChanged(const Entity entity, const Signature oldSignature,
                                               const Signature newSignature)
    {
        for (const auto& system : std::ranges::views::values(m_querySystems)) {
            const Signature systemSignature = system->getSignature();
            // Check if entity qualifies now but did not qualify before.
            if (((oldSignature & systemSignature) != systemSignature) &&
                ((newSignature & systemSignature) == systemSignature)) {
                system->entities.insert(entity);
            }
            // Otherwise, if the entity no longer qualifies but did before, remove it.
            else if (((oldSignature & systemSignature) == systemSignature) &&
                     ((newSignature & systemSignature) != systemSignature)) {
                system->entities.erase(entity);
            }
        }
    }
} // namespace nexo::ecs
