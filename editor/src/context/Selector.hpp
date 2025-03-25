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
     * The Selector class tracks the currently selected entity, its type, and
     * provides methods to manipulate the selection state. It also maintains
     * entity UUID to UI handle mappings for consistent labeling in the interface.
     */
	class Selector {
		public:
			int getSelectedEntity() const;
			const std::string &getSelectedUuid() const;
			void setSelectedEntity(std::string_view uuid, int entity);

			void setSelectedScene(int scene);
			int getSelectedScene() const;

			void unselectEntity();

			SelectionType getSelectionType() const;
			void setSelectionType(SelectionType type);

			bool isEntitySelected() const;

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
			const std::string &getUiHandle(const std::string &uuid, const std::string &defaultHandle);
			void setUiHandle(const std::string &uuid, std::string_view handle);

			static Selector &get()
			{
				static Selector instance;
				return instance;
			}

		private:
			std::string m_selectedUuid;
			int m_selectedEntity = -1;
			int m_selectedScene = -1;
			SelectionType m_selectionType = SelectionType::NONE;

			struct TransparentHasher {
			    using is_transparent = void; // Marks this hasher as transparent for heterogeneous lookup

			    size_t operator()(std::string_view key) const noexcept {
			        return std::hash<std::string_view>{}(key);
			    }

			    size_t operator()(const std::string &key) const noexcept {
			        return std::hash<std::string>{}(key);
			    }
			};

    		std::unordered_map<std::string, std::string, TransparentHasher, std::equal_to<>> m_uiHandles;
	};
}
