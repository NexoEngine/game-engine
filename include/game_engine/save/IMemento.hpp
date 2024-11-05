/*
** NEXO ENGINE, 2024
** game_engine
** File description:
** IMemento.hpp
*/

#pragma once

#include <nlohmann/json.hpp>

namespace engine::save {

	using json = nlohmann::json;

	template <typename T>
	concept JSONSerializable = requires(T obj, json& j) {
		{ to_json(j, obj) } -> std::same_as<void>;
		{ from_json(j, obj) } -> std::same_as<void>;
	};

    /**
	 * @class IMemento
	 * @brief Interface for the Memento design pattern, used to save and restore the state of an object.
	 * 
	 * @note This interface is used in conjunction with the IOriginator interface.
	 * @warning It is best to define POD types for the child Memento class.
	 *
	 */
	struct IMemento {
	};
} // namespace engine::editor
