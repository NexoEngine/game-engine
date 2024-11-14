/*
** NEXO ENGINE, 2024
** game_engine
** File description:
** IMemento.hpp
*/

#pragma once

#include "json.hpp"

namespace engine::save {

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
