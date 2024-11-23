//// PopupManager.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        16/11/2024
//  Description: Source file for the popup manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "PopupManager.hpp"

#include <imgui.h>

namespace nexo::editor {
    void PopupManager::openPopup(const std::string &popupName)
    {
        LOG(NEXO_INFO, "Set {} to true", popupName);
        m_popups[popupName] = true;
    }

    void PopupManager::closePopup() const
    {
        ImGui::EndPopup();
    }

    void PopupManager::closePopupInContext() const
    {
        ImGui::CloseCurrentPopup();
    }

    bool PopupManager::showPopup(const std::string &popupName)
    {
        if (!m_popups.contains(popupName))
            return false;
        if (m_popups.at(popupName))
        {
            ImGui::OpenPopup(popupName.c_str());
            m_popups.at(popupName) = false;
        }
        return ImGui::BeginPopup(popupName.c_str());
    }

    bool PopupManager::showPopupModal(const std::string &popupModalName)
    {
        if (!m_popups.contains(popupModalName))
            return false;
        if (m_popups.at(popupModalName))
        {
            LOG(NEXO_INFO, "Opened {} popup", popupModalName);

            ImGui::OpenPopup(popupModalName.c_str());
            m_popups.at(popupModalName) = false;
        }
        return ImGui::BeginPopupModal(popupModalName.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    }

    VariantData &PopupManager::getUserData(const std::string &popupName)
    {
        if (!m_userData.contains(popupName))
        {
            static VariantData defaultValue = std::monostate{};
            return defaultValue;
        }
        return m_userData.at(popupName);
    }

    void PopupManager::setUserData(const std::string &popupName, const VariantData &data)
    {
        m_userData[popupName] = data;
    }
}
