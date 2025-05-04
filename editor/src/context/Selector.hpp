//// Selector.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/03/2025
//  Description: Header file for the selector class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/Coordinator.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <optional>

namespace nexo::editor {

    enum class SelectionType {
        NONE,
        SCENE,
        CAMERA,
        DIR_LIGHT,
        AMBIENT_LIGHT,
        SPOT_LIGHT,
        POINT_LIGHT,
        ENTITY
    };

    /**
     * @class Selector
     * @brief Singleton class managing entity selection state in the editor
     *
     * The Selector class tracks the currently selected entities, their types, and
     * provides methods to manipulate the selection state. It also maintains
     * entity UUID to UI handle mappings for consistent labeling in the interface.
     */
    class Selector {
    public:
        /**
         * @brief Gets the primary selected entity
         *
         * The primary entity is the entity that gizmos and other operations
         * will primarily act on when multiple entities are selected.
         * This returns the last entity selected if there are multiple selections.
         *
         * @return int The entity ID of the primary selection, or -1 if no entity is selected
         */
        int getPrimaryEntity() const;

        /**
         * @brief Gets all selected entities
         *
         * @return const std::vector<int>& A reference to the vector of all selected entity IDs
         */
        const std::vector<int>& getSelectedEntities() const;

        /**
         * @brief Gets the UUID of the primary entity
         *
         * @return const std::string& The UUID of the primary entity
         */
        const std::string& getPrimaryUuid() const;

        /**
         * @brief Gets all selected entity UUIDs
         *
         * @return std::vector<std::string> Vector containing UUIDs of all selected entities
         */
        std::vector<std::string> getSelectedUuids() const;

        /**
         * @brief Selects a single entity, replacing the current selection
         *
         * @param[in] uuid The UUID of the entity to select
         * @param[in] entity The entity ID to select
         * @param[in] type The type of entity being selected
         */
        void selectEntity(std::string_view uuid, int entity, SelectionType type = SelectionType::ENTITY);

        /**
         * @brief Adds an entity to the current selection
         *
         * @param[in] uuid The UUID of the entity to add
         * @param[in] entity The entity ID to add
         * @param[in] type The type of entity being added
         * @return true If the entity was successfully added to the selection
         * @return false If the entity was already selected
         */
        bool addToSelection(std::string_view uuid, int entity, SelectionType type = SelectionType::ENTITY);

        /**
         * @brief Toggle selection state of an entity
         *
         * @param[in] uuid The UUID of the entity to toggle
         * @param[in] entity The entity ID to toggle
         * @param[in] type The type of entity being toggled
         * @return true If the entity is now selected
         * @return false If the entity is now deselected
         */
        bool toggleSelection(std::string_view uuid, int entity, SelectionType type = SelectionType::ENTITY);

        /**
         * @brief Removes an entity from the selection
         *
         * @param[in] entity The entity ID to remove
         * @return true If the entity was successfully removed from the selection
         * @return false If the entity wasn't selected
         */
        bool removeFromSelection(int entity);

        /**
         * @brief Sets the currently selected scene
         *
         * @param[in] scene The scene entity ID to select
         */
        void setSelectedScene(int scene);

        /**
         * @brief Gets the currently selected scene
         *
         * @return int The entity ID of the selected scene, or -1 if no scene is selected
         */
        int getSelectedScene() const;

        /**
         * @brief Clears the current entity selection
         */
        void clearSelection();

        /**
         * @brief Checks if a specific entity is currently selected
         *
         * @param entity The entity ID to check
         * @return true If the entity is selected
         * @return false If the entity is not selected
         */
        bool isEntitySelected(int entity) const;

        /**
         * @brief Checks if any entity is currently selected
         *
         * @return true If at least one entity is selected
         * @return false If no entities are selected
         */
        bool hasSelection() const;

        /**
         * @brief Gets the primary selection type
         *
         * @return SelectionType The type of the primary selected entity
         */
        SelectionType getPrimarySelectionType() const;

        /**
         * @brief Sets the selection type (only applied to subsequent selections)
         *
         * @param type The selection type to set
         */
        void setSelectionType(SelectionType type);

        /**
         * @brief Gets the UI handle associated with a UUID
         *
         * If the UUID doesn't have an associated handle yet, the default
         * handle is stored and returned.
         *
         * @param[in] uuid The UUID to look up
         * @param[in] defaultHandle The default handle to use if none exists
         * @return const std::string& Reference to the UI handle for the UUID
         */
        const std::string& getUiHandle(const std::string& uuid, const std::string& defaultHandle);

        /**
         * @brief Sets the UI handle associated with a UUID
         *
         * @param[in] uuid The UUID to set the handle for
         * @param[in] handle The handle to set
         */
        void setUiHandle(const std::string& uuid, std::string_view handle);

        static Selector& get() {
            static Selector instance;
            return instance;
        }

    private:
        // Selection data
        struct SelectionData {
            int entityId;
            std::string uuid;
            SelectionType type;
        };
        std::vector<SelectionData> m_selectedEntities;  // Ordered list of selected entities
        std::unordered_set<int> m_selectedEntityIds;    // Set for quick lookups

        int m_selectedScene = -1;
        SelectionType m_defaultSelectionType = SelectionType::ENTITY;

        struct TransparentHasher {
            using is_transparent = void; // Marks this hasher as transparent for heterogeneous lookup

            size_t operator()(std::string_view key) const noexcept {
                return std::hash<std::string_view>{}(key);
            }

            size_t operator()(const std::string& key) const noexcept {
                return std::hash<std::string>{}(key);
            }
        };

        std::unordered_map<std::string, std::string, TransparentHasher, std::equal_to<>> m_uiHandles;

        static void addSelectedTag(int entity);
        static void removeSelectedTag(int entity);
    };
}
