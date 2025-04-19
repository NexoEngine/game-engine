//// Selector.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/03/2025
//  Description: Source file for the selector class
//
///////////////////////////////////////////////////////////////////////////////

#include "Selector.hpp"
#include "Application.hpp"
#include "components/Editor.hpp"

namespace nexo::editor {
	int Selector::getSelectedEntity() const
	{
		return m_selectedEntity;
	}

	const std::string &Selector::getSelectedUuid() const
	{
		return m_selectedUuid;
	}

	void Selector::setSelectedEntity(std::string_view uuid, const int entity)
	{
    	if (m_selectionType != SelectionType::NONE && m_selectionType != SelectionType::SCENE)
    	{
        	Application::m_coordinator->removeComponent<components::SelectedTag>(m_selectedEntity);
    	}
		m_selectedUuid = uuid;
		m_selectedEntity = entity;
        components::SelectedTag selectTag{};
        Application::m_coordinator->addComponent(m_selectedEntity, selectTag);
	}

	void Selector::setSelectedScene(int scene)
	{
		m_selectedScene = scene;
	}

	int Selector::getSelectedScene() const
	{
		return m_selectedScene;
	}

    void Selector::unselectEntity()
    {
        if (m_selectionType != SelectionType::NONE && m_selectionType != SelectionType::SCENE)
        {
            Application::m_coordinator->removeComponent<components::SelectedTag>(m_selectedEntity);
        }
        m_selectionType = SelectionType::NONE;
        m_selectedEntity = -1;
        m_selectedUuid = "";
    }

	SelectionType Selector::getSelectionType() const
	{
		return m_selectionType;
	}

	void Selector::setSelectionType(SelectionType type)
	{
		m_selectionType = type;
	}

	bool Selector::isEntitySelected() const
	{
		return (m_selectedEntity != -1);
	}

	const std::string &Selector::getUiHandle(const std::string &uuid, const std::string &defaultHandle)
	{
		const auto it = m_uiHandles.find(uuid);
		if (it == m_uiHandles.end())
		{
			m_uiHandles[uuid] = defaultHandle;
			return defaultHandle;
		}
    	return it->second;
	}

	void Selector::setUiHandle(const std::string &uuid, std::string_view handle)
	{
		m_uiHandles[uuid] = handle;
	}

}
