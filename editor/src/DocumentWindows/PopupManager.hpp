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

namespace nexo::editor {
    class PopupManager {
        public:
            void openPopup(const std::string &popupName);
            bool showPopupModal(const std::string &popupModalName);
            bool showPopup(const std::string &popupName);
            void closePopup() const;
            void closePopupInContext() const;

        private:
            struct TransparentHasher {
                using is_transparent = void; // Required for heterogeneous lookup
                std::size_t operator()(std::string_view key) const noexcept {
                    return std::hash<std::string_view>{}(key);
                }
            };

            std::unordered_map<std::string, bool, TransparentHasher, std::equal_to<>> m_popups;
    };
}
