//// Transform.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/07/2025
//  Description: Source file for the transform component
//
///////////////////////////////////////////////////////////////////////////////

#include "Transform.hpp"

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

    void TransformComponent::addChild(ecs::Entity childEntity)
    {
        if (std::ranges::find(children, childEntity) != children.end())
            return;
        children.push_back(childEntity);
    }

    void TransformComponent::removeChild(ecs::Entity childEntity)
    {
        children.erase(std::ranges::remove(children, childEntity).begin(), children.end());
    }
}
