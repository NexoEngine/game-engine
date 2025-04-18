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
#include "ImNexo/Elements.hpp"
#include "Logger.hpp"

#include <imgui.h>

namespace nexo::editor {
    void PopupManager::openPopup(const std::string &popupName, const ImVec2 &popupSize)
    {
        PopupProps props{
            .open = true,
            .callback = nullptr,
            .size = popupSize
        };
        m_popups[popupName] = props;
    }

    void PopupManager::openPopupWithCallback(const std::string &popupName, PopupCallback callback, const ImVec2 &popupSize)
    {
        PopupProps props{
            .open = true,
            .callback = callback,
            .size = popupSize
        };
        m_popups[popupName] = props;
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
        PopupProps &props = m_popups.at(popupName);
        if (props.open)
        {
            ImGui::OpenPopup(popupName.c_str());
            props.open = false;
        }
        if (props.size.x != 0 && props.size.y != 0)
        {
            ImGui::SetNextWindowSize(props.size);
        }
        return ImGui::BeginPopup(popupName.c_str());
    }

    bool PopupManager::showPopupModal(const std::string &popupModalName)
    {
        if (!m_popups.contains(popupModalName))
            return false;
        PopupProps &props = m_popups.at(popupModalName);
        if (m_popups.at(popupModalName).open)
        {
            ImGui::OpenPopup(popupModalName.c_str());
            props.open = false;
        }

        if (props.size.x != 0 && props.size.y != 0)
        {
            ImGui::SetNextWindowSize(props.size);
        }

        ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize
                               | ImGuiWindowFlags_NoBackground
                               | ImGuiWindowFlags_NoTitleBar;
        if (!ImGui::BeginPopupModal(popupModalName.c_str(), nullptr, flags))
            return false;
        ImVec2 pMin = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();
        ImVec2 pMax = ImVec2(pMin.x + size.x, pMin.y + size.y);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        const std::vector<ImNexo::GradientStop> stops = {
            { 0.06f, IM_COL32(58 / 3, 124 / 3, 161 / 3, 255) },
            { 0.26f, IM_COL32(88 / 3,  87 / 3, 154 / 3, 255) },
            { 0.50f, IM_COL32(88 / 3,  87 / 3, 154 / 3, 255) },
            { 0.73f, IM_COL32(58 / 3, 124 / 3, 161 / 3, 255) },
        };
        float angle = 148.0f;

        ImNexo::RectFilledLinearGradient(pMin, pMax, angle, stops, drawList);

        return true;
    }

    void PopupManager::runPopupCallback(const std::string &popupName)
    {
        if (m_popups.contains(popupName) && m_popups.at(popupName).callback != nullptr)
        {
            m_popups.at(popupName).callback();
        }
    }

}
