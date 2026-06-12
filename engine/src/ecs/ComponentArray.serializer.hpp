// ...existing code...
#pragma once

#include "save/Serializer.hpp"
#include "save/Serialization.hpp"
#include "save/SerializationContext.hpp"

#include "save/SerializationException.hpp"

#include "ComponentArray.hpp"

namespace nexo::save {

    template<typename T>
    struct CurrentVersion<nexo::ecs::ComponentArray<T>> {
        static constexpr uint32_t value = 0;
    };

    template<typename T>
        requires(nexo::JSONSerializable<T>)
    struct Serializer<nexo::ecs::ComponentArray<T>, 0> {
        static void serialize(json& j, const nexo::ecs::ComponentArray<T>& arr,
                              const SerializationContext& ctx = SerializationContext{})
        {
            j["entitiesDense"] = arr.entities();
            j["componentsDense"] = arr.getAllComponents();
        }

        static void deserialize(const json& j, nexo::ecs::ComponentArray<T>& arr,
                                const SerializationContext& ctx = SerializationContext{})
        {
            const json& jEntities = j.at("entitiesDense");
            const json& jComponents = j.at("componentsDense");

            // safety check
            if (jEntities.size() != jComponents.size()) {
                THROW_EXCEPTION(DeserializationException, "Component array discrepancy, 'entitiesDense' and 'componentsDense' do not have the same size");
            }

            // we reset the ComponentArray and then reinsert all entities
            arr.reset();

            for (size_t i = 0 ; i < jEntities.size() ; ++i) {
                const auto entity = jEntities.at(i).get<nexo::ecs::Entity>();
                T comp{};
                nexo::save::deserialize(jComponents.at(i), comp, ctx);
                arr.insert(entity, std::move(comp));
            }
        }

        static void migrate_from_previous(json& j)
        {
            (void)j;
        }
    };

} // namespace nexo::save
// ...existing code...
