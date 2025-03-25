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
#include "Logger.hpp"

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include <algorithm>
#include <ranges>

namespace nexo::editor {

	template<typename T>
	std::shared_ptr<T> castWindow(const std::shared_ptr<IDocumentWindow>& ptr) {
	    return std::static_pointer_cast<T>(ptr);
	}

	class WindowRegistry {
		public:

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
			template<typename T>
			requires std::derived_from<T, IDocumentWindow>
			void registerWindow(std::shared_ptr<T> window)
			{
			    auto &windowsOfType = m_windows[typeid(T)];
			    if (std::ranges::any_of(windowsOfType, [&](const auto &existingWindow) {
			            return existingWindow->getWindowName() == window->getWindowName();
			        }))
			    {
			        THROW_EXCEPTION(WindowAlreadyRegistered, typeid(T), window->getWindowName());
			    }
			    windowsOfType.push_back(window);
			}

			/**
			 * @brief Retrieves a registered window of the specified type and name.
			 *
			 * This template function checks whether a window of type T with the given name has been registered.
			 * If the window is found, it returns the window as a weak pointer of type T.
			 * Otherwise, it returns an empty weak pointer.
			 *
			 * @tparam T The type of the window, which must derive from IDocumentWindow.
			 * @param windowName The unique name of the window to look for.
			 * @return std::weak_ptr<T> A weak pointer to the registered window if found; otherwise, an empty weak pointer.
			 */
			template<typename T>
			requires std::derived_from<T, IDocumentWindow>
			std::weak_ptr<T> getWindow(const std::string &windowName) const
			{
			    auto it = m_windows.find(typeid(T));
			    if (it == m_windows.end())
				{
					LOG(NEXO_WARN, "Window of type {} not found", typeid(T).name());
     				return {};
				}

			    auto &windowsOfType = it->second;
			    auto found = std::ranges::find_if(windowsOfType, [&windowName](const auto &w) {
			        return w->getWindowName() == windowName;
			    });

			    if (found == windowsOfType.end())
				{
					LOG(NEXO_WARN, "Window of type {} with name {} not found", typeid(T).name(), windowName);
     				return {};
				}

			    return std::static_pointer_cast<T>(*found);
			}



			template<typename T>
			requires std::derived_from<T, IDocumentWindow>
			std::ranges::transform_view<
			    std::ranges::ref_view<const std::vector<std::shared_ptr<IDocumentWindow>>>,
			    std::shared_ptr<T>(*)(const std::shared_ptr<IDocumentWindow>&)
			>
			getWindows() const
			{
			    // Helper: non-capturing function for casting:
			    std::shared_ptr<T>(*caster)(const std::shared_ptr<IDocumentWindow>&) = &castWindow<T>;

			    auto it = m_windows.find(typeid(T));
			    if (it == m_windows.end()) {
			        static const std::vector<std::shared_ptr<IDocumentWindow>> empty;
			        return std::ranges::transform_view(std::ranges::ref_view(empty), caster);
			    }
			    return std::ranges::transform_view(std::ranges::ref_view(it->second), caster);
			}

			/**
			 * @brief Assigns a docking identifier to a window.
			 *
			 * Registers the specified docking identifier for the window identified by its name by delegating to the docking registry.
			 *
			 * @param name The unique name of the window.
			 * @param id The docking identifier to associate with the window.
			 */
			void setDockId(const std::string& name, ImGuiID id);

			/**
			 * @brief Retrieves the docking identifier associated with a specified window.
			 *
			 * This function queries the docking registry for the docking identifier corresponding to the given window name.
			 * If the window does not have an assigned docking ID, an empty optional is returned.
			 *
			 * @param name The name of the window whose docking identifier is being requested.
			 * @return std::optional<ImGuiID> The docking identifier if it exists; otherwise, an empty optional.
			 */
			std::optional<ImGuiID> getDockId(const std::string& name) const;

			/**
			 * @brief Initializes all managed windows.
			 *
			 * Iterates through the collection of windows and calls the `setup()` method on each one.
			 * This function does not handle errors; it assumes each window's setup process runs without exceptions.
			 */
			void setup() const;

			/**
			 * @brief Shuts down all registered windows.
			 *
			 * Iterates through each window in the registry and invokes its shutdown routine.
			 */
			void shutdown() const;

			/**
			 * @brief Updates all registered windows.
			 *
			 * Iterates over the collection of registered windows and calls the update() method on each,
			 * ensuring that their state is refreshed.
			 */
			void update() const;

			/**
			 * @brief Renders all open windows.
			 *
			 * Iterates through the registered windows and invokes the show() method on each window that is currently opened.
			 */
			void render() const;

		private:
			std::unordered_map<std::type_index, std::vector<std::shared_ptr<IDocumentWindow>>> m_windows;

			DockingRegistry m_dockingRegistry;
	};
}
