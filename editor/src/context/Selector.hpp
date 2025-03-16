//// Selector.hpp ///////////////////////////////////////////////////////////////
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
		SPOT_LIGHT,
		POINT_LIGHT,
		ENTITY
	};

	class Selector {
		public:
			int getSelectedEntity() const;
			const std::string &getSelectedUuid() const;
			void setSelectedEntity(const std::string &uuid, int entity);

			void unselectEntity();

			SelectionType getSelectionType() const;
			void setSelectionType(SelectionType type);

			bool isEntitySelected() const;

			const std::string &getUiHandle(const std::string &uuid, const std::string &defaultHandle);
			void setUiHandle(const std::string &uuid, const std::string &handle);

			static Selector &get()
			{
				static Selector instance;
				return instance;
			}

		private:
			std::string m_selectedUuid = "";
			int m_selectedEntity = -1;
			SelectionType m_selectionType = SelectionType::NONE;
			std::unordered_map<std::string, std::string> m_uiHandles;
	};
}
