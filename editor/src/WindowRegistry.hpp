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
			 * @brief Registers a document window instance in the registry.
			 *
			 * This function stores a shared pointer to a window of type T in an internal map,
			 * using the type's typeid as the key for later retrieval. Registering a window of a
			 * given type will overwrite any previous registration of the same type.
			 *
			 * @tparam T The window type, which must derive from IDocumentWindow.
			 * @param window A shared pointer to the window instance to register.
			 */
			void registerWindow(std::shared_ptr<T> window)
			{
				m_windows[typeid(T)] = window;
			}

			template<typename T>
			/**
			 * @brief Retrieves a registered window of the specified type.
			 *
			 * This template function checks whether a window of type T has been registered.
			 * If the window is not found, it throws a WindowNotRegistered exception.
			 * Otherwise, it returns the window as a shared pointer of type T.
			 *
			 * @tparam T The type of the window, which must derive from IDocumentWindow.
			 * @return std::shared_ptr<T> A shared pointer to the registered window.
			 * @throws WindowNotRegistered Thrown if no window of the specified type is registered.
			 */
			std::shared_ptr<T> getWindow()
			{
				if (!m_windows.contains(typeid(T)))
					THROW_EXCEPTION(WindowNotRegistered, typeid(T));
				return std::static_pointer_cast<T>(m_windows[typeid(T)]);
			}

			void setDockId(const std::string& name, ImGuiID id);
			std::optional<ImGuiID> getDockId(const std::string& name) const;

			void setup() const;
			void shutdown() const;
			void update() const;
			void render() const;

		private:
			std::unordered_map<std::type_index, std::shared_ptr<IDocumentWindow>> m_windows;

			DockingRegistry m_dockingRegistry;
	};
}
