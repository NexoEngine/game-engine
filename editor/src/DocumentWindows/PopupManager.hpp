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

	/**
     * @brief Manages the state of popups within the UI.
     *
     * The PopupManager allows you to open, display, and close popups by name.
     * It internally stores popup states and uses ImGui to render popups.
     */
    class PopupManager {
        public:

			/**
			* @brief Opens a popup by name.
			*
			* Marks the popup as active so that it will be opened in the next frame.
			*
			* @param popupName The unique name of the popup.
			*/
            void openPopup(const std::string &popupName);

			/**
			* @brief Displays a modal popup.
			*
			* If the popup was marked as active, opens it using ImGui's modal popup functions.
			* This function returns true if the modal popup is open.
			*
			* @param popupModalName The unique name of the modal popup.
			* @return true if the modal popup is currently open; false otherwise.
			*/
            bool showPopupModal(const std::string &popupModalName);

			/**
			* @brief Displays a non-modal popup.
			*
			* If the popup was marked as active, opens it using ImGui's non-modal popup functions.
			* This function returns true if the popup is open.
			*
			* @param popupName The unique name of the popup.
			* @return true if the popup is currently open; false otherwise.
			*/
            bool showPopup(const std::string &popupName);

			/**
			* @brief Closes the current popup.
			*
			* Ends the current ImGui popup.
			*/
            void closePopup() const;

			/**
			* @brief Closes the current popup in its context.
			*
			* Requests ImGui to close the current popup.
			*/
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
