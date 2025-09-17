//// GameWindow.hpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Jean CARDONNE
//  Date:        2025-06-24
//  Description: Game window for play mode - displays game view without editor elements
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "../PopupManager.hpp"
#include "ADocumentWindow.hpp"
#include "ecs/Definitions.hpp"

namespace nexo::editor {

    class EditorScene;

    class GameWindow final : public ADocumentWindow {
       public:
        using ADocumentWindow::ADocumentWindow;

        /**
         * @brief Sets up the game window, initializing necessary resources and configurations.
         *
         * This method is called when the game window is created. It initializes the viewport,
         * camera entity, and any other resources required for rendering the game view.
         */
        void setup() override;

        /**
         * @brief Cleans up resources used by the game window.
         *
         * This method is called when the game window is closed. It releases any resources
         * allocated during setup and ensures that the game window is properly shut down.
         */
        void shutdown() override;

        /**
         * @brief Renders the game window and its contents.
         *
         * This method is called to display the game window. It handles rendering the game scene,
         * updating the viewport, and managing user interactions within the game window.
         */
        void show() override;

        /**
         * @brief Updates the game window state.
         *
         * This method is called periodically to update the game window's state, including
         * handling input, updating the scene, and refreshing the display as needed.
         */
        void update() override;

        /**
         * @brief Sets the ID of the scene to be displayed in the game window.
         *
         * This method assigns the specified scene ID to the game window, allowing it to
         * load and render the corresponding scene.
         *
         * @param sceneId The ID of the scene to be set for the game window.
         */
        void setSceneId(unsigned int sceneId);

        /**
         * @brief Sets the UUID of the scene to be displayed in the game window.
         *
         * This method assigns the specified UUID to the scene that the game window will render.
         * It is used to identify and load the correct scene for display.
         *
         * @param sceneUuid The UUID of the scene to be set for the game window.
         */
        void setSceneUuid(std::string_view sceneUuid);

       private:
        unsigned int m_sceneId{0};
        std::string m_sceneUuid;

        ecs::Entity m_gameCamera{ecs::INVALID_ENTITY};

        ImVec2 m_viewportBounds[2];

        bool m_isPaused{false};
        PopupManager m_popupManager;

        /**
         * @brief Renders the viewport area of the game window.
         *
         * This method handles the rendering of the game viewport, including setting up
         * the camera view, drawing the scene, and managing any necessary rendering states.
         */
        void renderViewport();

        /**
         * @brief Renders the toolbar of the game window.
         *
         * This method is responsible for drawing the toolbar at the top of the game window,
         * which may include buttons for play, pause, stop, and other game controls.
         */
        void renderToolbar();
    };

} // namespace nexo::editor
