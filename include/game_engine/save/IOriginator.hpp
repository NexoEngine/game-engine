/*
** NEXO ENGINE, 2024
** game_engine
** File description:
** IOriginator.hpp
*/

#pragma once

#include "IMemento.hpp"

namespace engine::save {
	/**
	 * @class IOriginator
	 * @brief Interface for the Originator design pattern, used to save and restore the state of an object.
	 * 
	 * @note This interface is used in conjunction with the IMemento interface.
	 *
	 */
	template <class Memento>
	class IOriginator {
	public:
		virtual ~IOriginator() = default;
		virtual void restoreMemento(const Memento &memento) = 0;
		virtual Memento saveMemento() const = 0;
	};
} // namespace engine::editor