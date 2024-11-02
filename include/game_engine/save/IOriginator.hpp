/*** IOriginator.hpp **********************************************************
*
*  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
*  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
*  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
*  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
*  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
*
*  Author:      Guillaume Hein
*  Date:        19/10/2024
*  Description: Interface for the Originator design pattern, used to save and
*               restore the state of an object.
*
******************************************************************************/

#pragma once

#include <type_traits>

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
        // Ensure that Memento inherits from IMemento
        static_assert(std::is_base_of_v<IMemento, Memento>,
            "Template Memento must derive from IMemento, "
            "either directly or indirectly");

		virtual ~IOriginator() = default;

		[[nodiscard]] virtual std::shared_ptr<Memento> saveMemento() const = 0;

		virtual void restoreMemento(const Memento &memento) = 0;
	};
} // namespace engine::editor
