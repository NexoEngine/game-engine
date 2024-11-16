//// PopupManager.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        16/11/2024
//  Description: Header file for the popup manager class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <unordered_map>
#include <string>

#include "SceneManagerBridge.hpp"

namespace nexo::editor {
    class PopupManager {
        public:
            void openPopup(const std::string &popupName);
            bool showPopupModal(const std::string &popupModalName);
            bool showPopup(const std::string &popupName);
            void closePopup();
            void closePopupInContext();

            VariantData &getUserData(const std::string &popupName);
            void setUserData(const std::string &popupName, const VariantData &data);

        private:
            std::unordered_map<std::string, bool> m_popups;
            std::unordered_map<std::string, VariantData> m_userData;
    };
}