// ...existing code...
#pragma once

#include "save/Serializer.hpp"
#include "save/Serialization.hpp"
#include "save/SerializationContext.hpp"

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
            j = json::array();
            const auto span = arr.entities();
            for (size_t i = 0; i < span.size(); ++i) {
                const auto entity = span[i];
                json item;
                item["entity"] = entity;
                // Use the public save API for the component type
                const T& comp = arr.get(entity);
                json compj;
                nexo::save::serialize(compj, comp, ctx);
                item["component"] = compj;
                j.push_back(item);
            }
        }

        static void deserialize(const json& j, nexo::ecs::ComponentArray<T>& arr,
                                const SerializationContext& ctx = SerializationContext{})
        {
            for (const auto& item : j) {
                const auto entity = item.at("entity").get<nexo::ecs::Entity>();
                T comp{};
                nexo::save::deserialize(item.at("component"), comp, ctx);
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
