//// ADocumentWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Header file for the document window abstract class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <utility>

#include "IDocumentWindow.hpp"
#include "Nexo.hpp"
#include "WindowRegistry.hpp"
#include "inputs/WindowState.hpp"

namespace nexo::editor {

#define NEXO_WND_USTRID_INSPECTOR "###Inspector"
#define NEXO_WND_USTRID_SCENE_TREE "###Scene Tree"
#define NEXO_WND_USTRID_ASSET_MANAGER "###Asset Manager"
#define NEXO_WND_USTRID_CONSOLE "###Console"
#define NEXO_WND_USTRID_MATERIAL_INSPECTOR "###Material Inspector"
#define NEXO_WND_USTRID_PRIMITIVE_WINDOW "###Primitive Window"
#define NEXO_WND_USTRID_DEFAULT_SCENE "###Default Scene"
#define NEXO_WND_USTRID_BOTTOM_BAR "###CommandsBar"
#define NEXO_WND_USTRID_TEST "###TestWindow"
#define NEXO_WND_USTRID_GAME_WINDOW "###GameWindow"

    class ADocumentWindow : public IDocumentWindow {
       public:
        /**
         * @brief Constructs a new ADocumentWindow instance with a unique window ID.
         *
         * Initializes the document window by storing a reference to the provided WindowRegistry and assigning a unique
         * window identifier. This setup is essential for integrating the window with the docking management system.
         */
        explicit ADocumentWindow(std::string windowName, WindowRegistry &windowRegistry)
            : m_windowName(std::move(windowName)), m_windowRegistry(windowRegistry)
        {
            windowId = nextWindowId++;
        };
        ~ADocumentWindow() override = default;

        /**
         * @brief Checks if the document window is currently focused.
         *
         * This function returns a boolean indicating whether the window is currently focused and active.
         * It is marked as [[nodiscard]] to encourage users to handle the return value appropriately.
         *
         * @return True if the window is focused, false otherwise.
         */
        [[nodiscard]] bool isFocused() const override
        {
            return m_focused;
        }

        /**
         * @brief Checks if the document window is currently opened.
         *
         * This function returns a boolean indicating whether the window is open and should be rendered.
         * It is marked as [[nodiscard]] to encourage users to handle the return value appropriately.
         *
         * @return True if the window is opened, false otherwise.
         */
        [[nodiscard]] bool isOpened() const override
        {
            return m_opened;
        }

        /**
         * @brief Sets the open state of the document window.
         *
         * This function allows external code to modify the open state of the window, determining whether it should be
         * rendered or hidden. It overrides the corresponding method in the IDocumentWindow interface.
         *
         * @param opened A boolean value indicating the desired open state of the window.
         */
        void setOpened(bool opened) override
        {
            m_opened = opened;
        }

        /**
         * @brief Checks if the document window is currently hovered by the mouse cursor.
         *
         * This function returns a boolean indicating whether the window is currently hovered.
         * It is marked as [[nodiscard]] to encourage users to handle the return value appropriately.
         *
         * @return True if the window is hovered, false otherwise.
         */
        [[nodiscard]] bool isHovered() const override
        {
            return m_hovered;
        }

        /**
         * @brief Checks if the document window is currently visible within its docking context.
         *
         * This function returns a boolean indicating whether the window is visible in its docked state.
         * It is marked as [[nodiscard]] to encourage users to handle the return value appropriately.
         *
         * @return True if the window is visible in its dock, false otherwise.
         */
        [[nodiscard]] bool isVisibleInDock() const
        {
            return m_isVisibleInDock;
        }

        /**
         * @brief Retrieves the position of the document window.
         *
         * This function returns a constant reference to the ImVec2 structure representing the window's position
         * on the screen. The returned reference is marked as [[nodiscard]] to encourage users to utilize the
         * information provided by the window position.
         *
         * @return A constant reference to the position of the document window.
         */
        [[nodiscard]] const ImVec2 &getWindowPos() const
        {
            return m_windowPos;
        }
        /**
         * @brief Retrieves the size of the document window.
         *
         * This function returns a constant reference to the ImVec2 structure representing the window's size.
         * The returned reference is marked as [[nodiscard]] to encourage users to utilize the information
         * provided by the window size.
         *
         * @return A constant reference to the size of the document window.
         */
        [[nodiscard]] const ImVec2 &getWindowSize() const
        {
            return m_windowSize;
        }

        /**
         * @brief Retrieves the minimum size of the content area within the document window.
         *
         * This function returns a constant reference to the ImVec2 structure representing the minimum size
         * of the content area. The returned reference is marked as [[nodiscard]] to encourage users to
         * utilize the information provided by the content size.
         *
         * @return A constant reference to the minimum content size of the document window.
         */
        [[nodiscard]] const ImVec2 &getContentSizeMin() const
        {
            return m_contentSizeMin;
        }

        /**
         * @brief Retrieves the maximum size of the content area within the document window.
         *
         * This function returns a constant reference to the ImVec2 structure representing the maximum size
         * of the content area. The returned reference is marked as [[nodiscard]] to encourage users to
         * utilize the information provided by the content size.
         *
         * @return A constant reference to the maximum content size of the document window.
         */
        [[nodiscard]] const ImVec2 &getContentSizeMax() const
        {
            return m_contentSizeMax;
        }

        /**
         * @brief Retrieves the available size of the content area within the document window.
         *
         * This function returns a constant reference to the ImVec2 structure representing the available size
         * of the content area. The returned reference is marked as [[nodiscard]] to encourage users to
         * utilize the information provided by the content size.
         *
         * @return A constant reference to the available content size of the document window.
         */
        [[nodiscard]] const ImVec2 &getContentSize() const override
        {
            return m_contentSize;
        }

        /**
         * @brief Retrieves the open state of the document window.
         *
         * Returns a reference to the internal boolean that tracks whether the window is open.
         * This reference is primarily intended for ImGui integration, allowing direct manipulation
         * of the window's visibility state.
         *
         * @return A reference to the window's open state.
         */
        [[nodiscard]] bool &getOpened() override
        {
            return m_opened;
        }

        /**
         * @brief Retrieves the name of the document window.
         *
         * This function returns a constant reference to the window's name, which is used for identification
         * and docking purposes within the editor. The returned reference is marked as [[nodiscard]] to
         * encourage users to utilize the information provided by the window name.
         *
         * @return A constant reference to the name of the document window.
         */
        [[nodiscard]] const std::string &getWindowName() const override
        {
            return m_windowName;
        }

        /**
         * @brief Retrieves the current state of the window.
         *
         * This function returns a constant reference to the WindowState object, which encapsulates
         * various attributes of the window's state, such as its position, size, focus status, and visibility.
         * The returned reference is marked as [[nodiscard]] to encourage users to utilize the information
         * provided by the WindowState.
         *
         * @return A constant reference to the WindowState of the document window.
         */
        [[nodiscard]] const WindowState &getWindowState() const override
        {
            return m_windowState;
        };

        WindowId windowId;

       protected:
        bool m_opened              = true;
        bool m_focused             = false;
        bool m_hovered             = false; // TODO: make these update without user intervention
        bool m_wasVisibleLastFrame = false;
        bool m_isVisibleInDock     = true;

        ImVec2 m_windowPos;
        ImVec2 m_windowSize;
        ImVec2 m_contentSizeMin;
        ImVec2 m_contentSizeMax;
        ImVec2 m_contentSize;

        bool m_firstOpened = true;

        std::string m_windowName;
        WindowState m_windowState;

        WindowRegistry &m_windowRegistry;

        /**
         * @brief Prepares the document window for rendering.
         *
         * This function should be called at the beginning of the window's rendering process. It handles
         * docking configuration, visibility updates, and size adjustments to ensure the window is correctly
         * set up before any content is drawn. The function takes the window name as a parameter to manage
         * docking behavior through the WindowRegistry.
         *
         * @param windowName The name of the window used for docking management.
         */
        void beginRender(const std::string &windowName);

       private:
        /**
         * @brief Initializes the docking configuration for the document window on its first display.
         *
         * This function retrieves the current ImGui window and checks its docking state to ensure it aligns with the
         * expected configuration from the WindowRegistry. On the first open (when m_firstOpened is true), if the window
         * is not actively docked or its current dock ID does not match the expected ID obtained via the provided window
         * name, the function assigns the expected dock ID to the window. If the window is already docked but the dock
         * IDs still differ, the current dock ID is saved to the WindowRegistry. The m_firstOpened flag is then set to
         * false so that the docking configuration is applied only once.
         *
         * @param windowName The name used to look up the expected dock identifier in the WindowRegistry.
         */
        void dockingUpdate(const std::string &windowName);

        /**
         * @brief Updates the visibility status of the document window.
         *
         * This function checks whether the window is currently focused, docked, and active to determine its visibility
         * state. If the window is docked, it verifies that it is not hidden or set to skip items to consider it
         * visible. For undocked windows, reaching this function implies visibility. The function also updates the
         * m_hovered status based on whether the window is currently hovered by the mouse cursor.
         */
        void visibilityUpdate();

        /**
         * @brief Updates the size and position attributes of the document window.
         *
         * This function retrieves the current ImGui window and updates the internal state variables to reflect the
         * window's position, size, and content region dimensions. It captures the overall window position and size,
         * as well as the minimum and maximum coordinates of the content region, and calculates the available content
         * size for rendering purposes.
         */
        void sizeUpdate();
    };
} // namespace nexo::editor
