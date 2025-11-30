#include <gtest/gtest.h>

#include "save/Serialization.hpp"
#include "Json.hpp"

#include "ecs/Coordinator.hpp"
#include "components/Transform.hpp"
#include "components/Transform.serializer.hpp"
#include "components/Name.hpp"
#include "components/Name.serializer.hpp"
#include "components/Parent.hpp"
#include "components/Parent.serializer.hpp"
#include "components/SceneComponents.hpp"
#include "components/SceneComponents.serializer.hpp"
#include "components/Uuid.hpp"
#include "components/Uuid.serializer.hpp"
#include "components/MaterialComponent.hpp"
#include "components/MaterialComponent.serializer.hpp"

using namespace nexo;

TEST(SaveComponents, CoordinatorSerializeSimpleComponents)
{
    nexo::ecs::Coordinator coord;
    coord.init();

    // Register components
    coord.registerComponent<nexo::components::TransformComponent>();
    coord.addComponentDescription(coord.getComponentType<nexo::components::TransformComponent>(), {"Transform", {}});

    coord.registerComponent<nexo::components::NameComponent>();
    coord.addComponentDescription(coord.getComponentType<nexo::components::NameComponent>(), {"Name", {}});

    coord.registerComponent<nexo::components::ParentComponent>();
    coord.addComponentDescription(coord.getComponentType<nexo::components::ParentComponent>(), {"Parent", {}});

    coord.registerComponent<nexo::components::SceneTag>();
    coord.addComponentDescription(coord.getComponentType<nexo::components::SceneTag>(), {"SceneTag", {}});

    coord.registerComponent<nexo::components::UuidComponent>();
    coord.addComponentDescription(coord.getComponentType<nexo::components::UuidComponent>(), {"Uuid", {}});

    coord.registerComponent<nexo::components::MaterialComponent>();
    coord.addComponentDescription(coord.getComponentType<nexo::components::MaterialComponent>(), {"Material", {}});

    // Create entities and add components
    auto e1 = coord.createEntity();
    auto e2 = coord.createEntity();

    coord.addComponent<nexo::components::TransformComponent>(e1, {});
    coord.addComponent<nexo::components::TransformComponent>(e2, {});

    coord.addComponent<nexo::components::NameComponent>(e1, {"Player"});

    coord.addComponent<nexo::components::ParentComponent>(e2, {e1});

    coord.addComponent<nexo::components::SceneTag>(e1, {0, true, true});

    coord.addComponent<nexo::components::UuidComponent>(e1, {});

    coord.addComponent<nexo::components::MaterialComponent>(e2, {});

    // Serialize each registered type and check
    const auto &typeMap = coord.getTypeIdToTypeIndex();

    nexo::json root;
    root["component_arrays"] = nexo::json::array();

    for (const auto &p : typeMap) {
        const auto &ti = p.second;
        nexo::json out;
        bool ok = coord.serializeComponentArray(ti, out, nexo::save::SerializationContext::editor());
        if (!ok) continue;
        root["component_arrays"].push_back(out);
    }

    // We expect at least transform array to be present with 2 entries
    bool foundTransform = false;
    for (const auto &arr : root["component_arrays"]) {
        // arr is an array of { entity, component }
        if (!arr.is_array()) continue;
        if (arr.size() == 2) foundTransform = true;
    }

    EXPECT_TRUE(foundTransform);
}

