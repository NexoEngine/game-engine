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
	concept JSONSerializable = requires(const T& t)
	{
		{ to_json(t) } -> std::same_as<json>;
		{ from_json(t, json()) } -> std::same_as<T>;
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
