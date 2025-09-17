//// Editor.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        09/11/2024
//  Description: Header file for the main editor class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IDocumentWindow.hpp"
#define L_DEBUG 1
#include <memory>

#include "WindowRegistry.hpp"
#include "inputs/InputManager.hpp"

namespace nexo::editor {

    class Editor {
       private:
        // Singleton: private constructor and destructor
        Editor()  = default;
        ~Editor() = default;

       public:
        // Singleton: Meyers' Singleton Pattern
        /**
         * @brief Retrieves the singleton instance of the Editor class.
         *
         * This method ensures that only one instance of the Editor class exists
         * throughout the application lifecycle. It provides global access to the
         * editor instance.
         *
         * @return Editor& A reference to the singleton Editor instance.
         */
        static Editor &getInstance()
        {
            static Editor s_instance;
            return s_instance;
        }

        // Singleton: delete copy constructor and assignment operator
        /**
         * @brief Deleted copy constructor to prevent copying of the Editor instance.
         *
         * This prevents the creation of multiple instances of the Editor class
         * by disallowing copy construction.
         */
        Editor(Editor const &)         = delete;
        void operator=(Editor const &) = delete;

        /**
         * @brief Initializes the editor.
         *
         * Configures the application engine, sets up the UI style, and initializes the window registry.
         */
        void init() const;

        /**
         * @brief Checks if the editor is currently open.
         *
         * The editor is considered open when it has not been signaled to quit, the application window is open, and the
         * application is still running.
         *
         * @return true if the editor is open, false otherwise.
         */
        [[nodiscard]] bool isOpen() const;

        /**
         * @brief Updates the editor's state for the current frame.
         *
         * This function updates the window registry to process pending window changes and then signals the application
         * to conclude the current frame.
         */
        void update() const;

        /**
         * @brief Renders the editor's user interface.
         *
         * This function handles the complete rendering cycle for the editor. It initiates a new frame, configures the
         * ImGui and ImGuizmo contexts, and builds the UI layout including the docks pace and main menu bar. Registered
         * windows are rendered afterward, and a full-viewport gradient background is drawn before finalizing the
         * frame.
         */
        void render();

        /**
         * @brief Shuts down the editor.
         *
         * This method finalizes the editor shutdown process by logging the closing event,
         * destroying all active windows via the window registry, and terminating the ImGui backend.
         */
        void shutdown() const;

        /**
         * @brief Creates and registers a new window of type T.
         *
         * This function instantiates a new window of type T using the provided name and the editor's window registry.
         * The new window is then registered with the registry so that it can be retrieved later. The type T must be
         * derived from IDocumentWindow.
         *
         * @tparam T The type of the window to create and register. Must inherit from IDocumentWindow.
         * @param name The name to assign to the new window.
         */
        template<typename T>
            requires std::derived_from<T, IDocumentWindow>
        void registerWindow(const std::string &name)
        {
            if (m_windowRegistry.hasWindow(name)) {
                LOG(NEXO_WARN, "A window with the name '{}' already exists. Please choose a different name.", name);
                return;
            }
            auto window = std::make_shared<T>(name, m_windowRegistry);
            m_windowRegistry.registerWindow<T>(window);
        }

        /**
         * @brief Retrieves a registered window of type T by its name.
         *
         * This function returns a weak pointer to a window instance of type T from the editor's window registry.
         * The type T must be derived from IDocumentWindow. If no window with the specified name is registered, an empty
         * weak pointer is returned.
         *
         * @tparam T The type of the window to retrieve. Must inherit from IDocumentWindow.
         * @param windowName The name of the window to retrieve.
         * @return std::weak_ptr<T> A weak pointer to the registered window of type T, or an empty pointer if no such
         * window exists.
         */
        template<typename T>
            requires std::derived_from<T, IDocumentWindow>
        std::weak_ptr<T> getWindow(const std::string &windowName)
        {
            return m_windowRegistry.getWindow<T>(windowName);
        }

       private:
        /**
         * @brief Initializes the core engine and configures ImGui components.
         *
         * This function sets up essential engine features, including:
         * - On Linux, configuring the window's Wayland app ID and window manager class if WAYLAND_APP_ID is defined. A
         * warning is issued if the macro is undefined.
         * - Initializing the engine core via nexo::init().
         * - Creating and initializing the ImGui context along with its backend error callback.
         * - Setting the path for ImGui's default layout configuration.
         * - Applying a dark color style and configuring ImGuizmo with the current ImGui context.
         */
        static void setupEngine();

        /**
         * @brief Configures the visual style for the ImGui interface.
         *
         * This method applies a dark color theme to the ImGui interface and customizes various style parameters
         * such as window rounding, frame padding, item spacing, and tab appearance to enhance the overall look
         * and feel of the editor.
         */
        static void setupStyle();

        /**
         * @brief Configures and loads fonts for the ImGui interface.
         *
         * Initializes the default, SourceSans, and FontAwesome fonts, adjusting the font size based on the
         * provided horizontal and vertical DPI scaling factors. Merges FontAwesome icons with the primary font
         * and initializes the backend font atlas.
         *
         * @param scaleFactorX Horizontal DPI scaling factor.
         * @param scaleFactorY Vertical DPI scaling factor.
         */
        static void setupFonts(float scaleFactorX, float scaleFactorY) ;

        /**
         * @brief Constructs and configures the editor's dockspace layout.
         *
         * This method initializes the dockspace on the main viewport using ImGui's DockBuilder API.
         * It subdivides the viewport into designated regions for key UI components such as the main scene,
         * console, scene tree, inspector, and material inspector. The dock nodes are registered with the
         * window registry to maintain a consistent layout. If a dockspace already exists but the registry
         * is not yet populated, the dock IDs are retrieved from the configuration.
         */
        void buildDockspace();

        /**
         * @brief Draws the main menu bar for the editor.
         *
         * Constructs the main menu bar UI component which features the "File" menu. Selecting the "Exit" option in the
         * menu sets a flag to signal that the editor should quit.
         */
        void drawMenuBar();

        /**
         * @brief Renders a full-viewport gradient background.
         *
         * This method draws a visually appealing gradient background that spans the entire viewport of the editor.
         * The gradient transitions smoothly between two specified colors, enhancing the overall aesthetic of the UI.
         */
        static void drawShortcutBar(const std::vector<CommandInfo> &possibleCommands) ;

        /**
         * @brief Renders a full-viewport gradient background.
         *
         * This method draws a visually appealing gradient background that spans the entire viewport of the editor.
         * The gradient transitions smoothly between two specified colors, enhancing the overall aesthetic of the UI.
         */
        static void drawBackground();

        /**
         * @brief Processes global commands and executes associated actions.
         *
         * This method checks for any global commands that have been triggered, such as quitting the application
         * or toggling the demo window. If a command is detected, the corresponding action is executed.
         */
        void handleGlobalCommands();

        /**
         * @brief Processes commands for the currently focused window and retrieves relevant command information.
         *
         * This method checks which window is currently focused in the editor. If a window is focused, it retrieves
         * any commands associated with that window and executes them. The function returns a list of command
         * information that can be used to display shortcuts or other command-related UI elements.
         *
         * @return std::vector<CommandInfo> A vector containing command information for the focused window.
         */
        [[nodiscard]] std::vector<CommandInfo> handleFocusedWindowCommands() const;

        bool m_quit           = false;
        bool m_showDemoWindow = false;
        WindowRegistry m_windowRegistry;
        InputManager m_inputManager;
    };
} // namespace nexo::editor
