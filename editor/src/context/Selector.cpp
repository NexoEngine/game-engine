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

    int Selector::getPrimaryEntity() const
    {
        if (m_selectedEntities.empty()) {
            return -1;
        }
        return m_selectedEntities.front().entityId; // First entity is the primary
    }

    const std::vector<int>& Selector::getSelectedEntities() const
    {
        static std::vector<int> entityIds;
        entityIds.clear();

        for (const auto& data : m_selectedEntities) {
            entityIds.push_back(data.entityId);
        }

        return entityIds;
    }

    const std::string& Selector::getPrimaryUuid() const
    {
        static std::string emptyString;
        if (m_selectedEntities.empty()) {
            return emptyString;
        }
        return m_selectedEntities.front().uuid;
    }

    std::vector<std::string> Selector::getSelectedUuids() const
    {
        std::vector<std::string> uuids;
        uuids.reserve(m_selectedEntities.size());

        for (const auto& data : m_selectedEntities) {
            uuids.push_back(data.uuid);
        }

        return uuids;
    }

    void Selector::selectEntity(const std::string_view uuid, const int entity, const SelectionType type)
    {
        clearSelection();
        addToSelection(uuid, entity, type);
    }

    bool Selector::addToSelection(const std::string_view uuid, const int entity, const SelectionType type)
    {
        if (m_selectedEntityIds.contains(entity))
            return false;

        SelectionData data = {
            .entityId = entity,
            .uuid = std::string(uuid),
            .type = type
        };
        m_selectedEntities.push_back(std::move(data));
        m_selectedEntityIds.insert(entity);

        addSelectedTag(entity);
        return true;
    }

    bool Selector::toggleSelection(const std::string_view uuid, const int entity, const SelectionType type)
    {
        if (isEntitySelected(entity)) {
            removeFromSelection(entity);
            return false;
        }
        addToSelection(uuid, entity, type);
        return true;
    }

    bool Selector::removeFromSelection(const int entity) {
        if (!m_selectedEntityIds.contains(entity))
            return false;

        m_selectedEntityIds.erase(entity);
        for (auto it = m_selectedEntities.begin(); it != m_selectedEntities.end(); ++it) {
            if (it->entityId == entity) {
                m_selectedEntities.erase(it);
                break;
            }
        }

        removeSelectedTag(entity);
        return true;
    }

    void Selector::setSelectedScene(const int scene)
    {
        m_selectedScene = scene;
    }

    int Selector::getSelectedScene() const
    {
        return m_selectedScene;
    }

    void Selector::clearSelection()
    {
        for (const auto& data : m_selectedEntities)
            removeSelectedTag(data.entityId);

        m_selectedEntities.clear();
        m_selectedEntityIds.clear();
    }

    bool Selector::isEntitySelected(const int entity) const
    {
        return m_selectedEntityIds.contains(entity);
    }

    bool Selector::hasSelection() const
    {
        return !m_selectedEntities.empty();
    }

    SelectionType Selector::getPrimarySelectionType() const
    {
        if (m_selectedEntities.empty()) {
            return SelectionType::NONE;
        }
        return m_selectedEntities.front().type;
    }

    SelectionType Selector::getSelectionType(const int entity) const
    {
        if (!m_selectedEntityIds.contains(entity))
            return SelectionType::NONE;

        for (const auto& data : m_selectedEntities) {
            if (data.entityId == entity) {
                return data.type;
            }
        }

        static_assert(true, "Unreachable code in getSelectionType");

        // This should not happen if m_selectedEntityIds is kept in sync with m_selectedEntities
        return SelectionType::NONE;
    }

    void Selector::setSelectionType(const SelectionType type)
    {
        m_defaultSelectionType = type;
    }

    const std::string& Selector::getUiHandle(const std::string& uuid, const std::string& defaultHandle)
    {
        const auto it = m_uiHandles.find(uuid);
        if (it == m_uiHandles.end()) {
            m_uiHandles[uuid] = defaultHandle;
            return defaultHandle;
        }
        return it->second;
    }

    void Selector::setUiHandle(const std::string& uuid, const std::string_view handle)
    {
        m_uiHandles[uuid] = handle;
    }

    void Selector::addSelectedTag(const int entity)
    {
        constexpr components::SelectedTag selectTag{};
        Application::m_coordinator->addComponent(entity, selectTag);
    }

    void Selector::removeSelectedTag(const int entity)
    {
        if (Application::m_coordinator->entityHasComponent<components::SelectedTag>(entity))
            Application::m_coordinator->removeComponent<components::SelectedTag>(entity);
    }
}
