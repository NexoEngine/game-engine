/*** ASerializableMemento.hpp *************************************************
*
*  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
*  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
*  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
*  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
*  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
*
*  Author:      Guillaume Hein
*  Date:        19/10/2024
*  Description: Interface for the Memento design pattern, makes a memento
*               serializable.
*
******************************************************************************/

#pragma once

#include <string>
#include <boost/preprocessor.hpp>

#include "game_engine/save/IMemento.hpp"

namespace engine::save {

    #define P_NEXO_SERIALIZE_FIELDS_EACH(r, data, field) {BOOST_PP_STRINGIZE(field), field},

    #define P_NEXO_DEFINE_SERIALIZE(...) \
    [[nodiscard]] engine::save::json serialize() const final { \
        engine::save::json data = { \
            BOOST_PP_SEQ_FOR_EACH(P_NEXO_SERIALIZE_FIELDS_EACH, data, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
        }; \
        return data; \
    }

    #define P_NEXO_DESERIALIZE_FIELDS_EACH(r, data, field) data.at(BOOST_PP_STRINGIZE(field)).get_to(field);

    #define P_NEXO_DEFINE_DESERIALIZE(...) \
    void deserialize(const engine::save::json& data) final { \
        BOOST_PP_SEQ_FOR_EACH(P_NEXO_DESERIALIZE_FIELDS_EACH, data, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
    }

    /**
    * @brief Macro to define the serializable fields of a serializable class.
    *
    * @note Uses nlohmann::json to automatically serialize fields. For custom types
    *
    */
    #define NEXO_SERIALIZABLE_FIELDS(...) \
        P_NEXO_DEFINE_SERIALIZE(__VA_ARGS__) \
        P_NEXO_DEFINE_DESERIALIZE(__VA_ARGS__)

    /**
     * @class ASerializableMemento
     * @brief Abstract interface for the Memento design pattern, makes a memento serializable.
     *
     * @note This abstract is used in conjunction with the IOriginator interface.
     * @warning It is best to define POD types for the child Memento class.
     *
     */
    struct ASerializableMemento : public IMemento {
        virtual ~ASerializableMemento() = default;

        /**
         * @brief Serialize the memento to a json object.
         *
         * @return json The serialized memento.
         */
        [[nodiscard]] virtual json serialize() const = 0;

        /**
         * @brief Deserialize the memento from a json object.
         *
         * @param data The json object to deserialize.
         */
        virtual void deserialize(const json& data) = 0;

        friend void to_json(json& nlohmann_json_j, const ASerializableMemento& nlohmann_json_t);
        friend void from_json(const json& nlohmann_json_j, ASerializableMemento& nlohmann_json_t);
    };

    /**
    * @note These functions link the mementos to the nlohmann::json library.
    *       It is needed to simplify usage of json library with our own memento.
    */
    inline void to_json(nlohmann::json& nlohmann_json_j, const ASerializableMemento& nlohmann_json_t)
    {
        nlohmann_json_j = nlohmann_json_t.serialize();
    }

    inline void from_json(const nlohmann::json& nlohmann_json_j, ASerializableMemento& nlohmann_json_t)
    {
        nlohmann_json_t.deserialize(nlohmann_json_j);
    }

    inline void to_json(nlohmann::json& nlohmann_json_j, const std::shared_ptr<ASerializableMemento>& nlohmann_json_t)
    {
        nlohmann_json_j = nlohmann_json_t->serialize();
    }

    inline void from_json(const nlohmann::json& nlohmann_json_j, const std::shared_ptr<ASerializableMemento>& nlohmann_json_t)
    {
        nlohmann_json_t->deserialize(nlohmann_json_j);
    }
} // namespace engine::editor
