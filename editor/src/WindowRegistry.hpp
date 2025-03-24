//// WindowRegistry.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/03/2025
//  Description: Header file for the window registry class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "IDocumentWindow.hpp"
#include "DockingRegistry.hpp"
#include "exceptions/Exceptions.hpp"

#include <unordered_map>
#include <typeindex>
#include <memory>

namespace nexo::editor {
	class WindowRegistry {
		public:
			template<typename T>
			requires std::derived_from<T, IDocumentWindow>
			/**
			 * @brief Registers a window instance of type T.
			 *
			 * This template method stores the provided window instance, which must derive from IDocumentWindow,
			 * in an internal registry keyed by its type information. If a window of type T is already registered,
			 * it will be overwritten.
			 *
			 * @tparam T The type of the window, which must be derived from IDocumentWindow.
			 * @param window Shared pointer to the window instance to register.
			 */
			void registerWindow(std::shared_ptr<T> window)
			{
				m_windows[typeid(T)] = window;
			}

			template<typename T>
			/**
			 * @brief Retrieves the registered window instance of type T.
			 *
			 * This template method returns a shared pointer to the window instance corresponding to the type T.
			 * If no window of type T is registered, the function throws a WindowNotRegistered exception.
			 *
			 * @tparam T The specific window type to retrieve (must derive from IDocumentWindow).
			 * @return std::shared_ptr<T> A shared pointer to the registered window instance.
			 *
			 * @throws WindowNotRegistered if a window of type T has not been registered.
			 */
			std::shared_ptr<T> getWindow()
			{
				if (!m_windows.contains(typeid(T)))
					THROW_EXCEPTION(WindowNotRegistered, typeid(T));
				return std::static_pointer_cast<T>(m_windows[typeid(T)]);
			}

			void setDockId(const std::string& name, ImGuiID id);
			ImGuiID getDockId(const std::string& name) const;

			void setup() const;
			void shutdown() const;
			void update() const;
			void render() const;

		private:
			std::unordered_map<std::type_index, std::shared_ptr<IDocumentWindow>> m_windows;

			DockingRegistry m_dockingRegistry;
	};
}
