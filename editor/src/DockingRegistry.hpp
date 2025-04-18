//// DockingRegistry.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/03/2025
//  Description: Header file for the docking registry class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <unordered_map>
#include <imgui.h>
#include <string>
#include <optional>

#include "utils/TransparentStringHash.hpp"

namespace nexo::editor {

    /**
     * @brief Manages associations between window names and their docking identifiers.
     *
     * The DockingRegistry maintains a mapping between window names and ImGui dock IDs,
     * allowing the editor to track and restore docking configurations across sessions.
     * This class is central to the editor's window layout management system, enabling
     * persistent window arrangements and proper docking behavior.
     *
     * It uses a TransparentStringHash for efficient string lookups and provides methods
     * for setting, retrieving, and removing dock ID associations.
     */
	class DockingRegistry {
		public:

			/**
			* @brief Registers or updates a docking identifier for a specified name.
			*
			* This method associates the provided docking ID with the given name within the registry,
			* allowing for subsequent retrieval when needed.
			*
			* @param name The name key for the docking entry.
			* @param id The docking identifier to be associated with the name.
			*/
			void setDockId(const std::string& name, ImGuiID id);

			/**
			 * @brief Retrieves the dock ID associated with the specified name.
			 *
			 * This method searches the internal registry for the given name and returns the dock ID
			 * wrapped in an optional if found. If the name does not exist in the registry, it returns an empty optional.
			 *
			 * @param name The name identifier of the dock.
			 * @return std::optional<ImGuiID> Optional containing the dock ID if found; otherwise, std::nullopt.
			 */
			std::optional<ImGuiID> getDockId(const std::string& name) const;

            /**
             * @brief Removes a dock ID association for the specified name.
             *
             * This method removes the association between the given name and its dock ID
             * from the registry. If the name does not exist in the registry, this method
             * has no effect.
             *
             * Removing a dock ID is useful when a window is closed or when its docking
             * configuration needs to be reset to default.
             *
             * @param name The name identifier of the dock association to remove.
             */
			void resetDockId(const std::string &name);

		private:
			std::unordered_map<std::string, ImGuiID, TransparentStringHash, std::equal_to<>> dockIds;
	};
}
