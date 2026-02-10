//// GroupManager.cpp //////////////////////////////////////////////////////////
//
//  Author:      Mehdy MORVAN
//  Date:        10/02/2026
//  Description: Source file for the group manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "GroupManager.hpp"

namespace nexo::ecs {

    void GroupManager::onEntityDestroyed(const Entity entity, const Signature& entitySignature)
    {
        for (const auto& group : m_groupRegistry | std::views::values) {
            if ((entitySignature & group->allSignature()) == group->allSignature()) group->removeFromGroup(entity);
        }
    }

} // namespace nexo::ecs
