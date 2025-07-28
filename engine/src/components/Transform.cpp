//// Transform.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/07/2025
//  Description: Source file for the transform component
//
///////////////////////////////////////////////////////////////////////////////

#include "Transform.hpp"

#include <algorithm>

namespace nexo::components {
    void TransformComponent::restore(const TransformComponent::Memento &memento)
    {
        pos = memento.position;
        quat = memento.rotation;
        size = memento.scale;
        localMatrix = memento.localMatrix;
        localCenter = memento.localCenter;
        children = memento.children;
    }

    [[nodiscard]] TransformComponent::Memento TransformComponent::save() const
    {
        return {pos, quat, size, localMatrix, localCenter, children};
    }

    void TransformComponent::addChild(const ecs::Entity childEntity)
    {
        if (std::ranges::find(children, childEntity) != children.end())
            return;
        children.push_back(childEntity);
    }

    void TransformComponent::removeChild(const ecs::Entity childEntity)
    {
        children.erase(std::ranges::remove(children, childEntity).begin(), children.end());
    }
}
