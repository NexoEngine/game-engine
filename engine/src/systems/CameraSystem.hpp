//// CameraSystem.hpp /////////////////////////////////////////////////////////
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
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Header file for the camera systems
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Timestep.hpp"
#include "components/Camera.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "core/event/Event.hpp"
#include "core/event/WindowEvent.hpp"
#include "ecs/GroupSystem.hpp"
#include "ecs/QuerySystem.hpp"
#include "ecs/System.hpp"

namespace nexo::system {

    /**
     * @brief System responsible for updating the camera context.
     *
     * This system iterates over all active camera entities and computes their view-projection
     * matrices using the CameraComponent and TransformComponent. The computed CameraContext is
     * then pushed into the RenderContext (a singleton component).
     *
     * @note Component Access Rights:
     *  - READ access to components::CameraComponent (owned)
     *  - READ access to components::SceneTag (non-owned)
     *  - READ access to components::TransformComponent (non-owned)
     *  - WRITE access to components::RenderContext (singleton)
     *
     * @note The system uses scene partitioning to only process camera entities belonging to the
     * currently active scene (identified by RenderContext.sceneRendered).
     */
    class CameraContextSystem final
        : public ecs::GroupSystem<
              ecs::Owned<ecs::Read<components::CameraComponent>>,
              ecs::NonOwned<ecs::Read<components::SceneTag>, ecs::Read<components::TransformComponent>>,
              ecs::WriteSingleton<components::RenderContext>> {
       public:
        void update();
    };

    /**
     * @brief System for controlling perspective cameras via keyboard and mouse input.
     *
     * This system handles movement of perspective cameras based on keyboard input (e.g. WASD,
     * space, tab) and adjusts camera orientation based on mouse movement. It also processes mouse
     * scroll events for zooming.
     *
     * @note Component Access Rights:
     *  - WRITE access to components::CameraComponent
     *  - WRITE access to components::PerspectiveCameraController
     *  - READ access to components::SceneTag
     *  - WRITE access to components::TransformComponent
     *  - READ access to components::RenderContext (singleton)
     *
     * @note Event Listeners:
     *  - event::EventMouseScroll - For camera zoom functionality
     *  - event::EventMouseMove - For camera rotation functionality
     *
     * @note The system only processes camera entities belonging to the currently active scene.
     */
    class PerspectiveCameraControllerSystem final
        : public ecs::QuerySystem<ecs::Write<components::CameraComponent>,
                                  ecs::Write<components::PerspectiveCameraController>, ecs::Read<components::SceneTag>,
                                  ecs::Write<components::TransformComponent>,
                                  ecs::ReadSingleton<components::RenderContext>>,
          LISTENS_TO(event::EventMouseScroll, event::EventMouseMove) {
       public:
        PerspectiveCameraControllerSystem();

        /**
         * @brief Updates camera position based on keyboard input.
         * This method processes keyboard inputs (WASD, space, tab) to move the camera
         * in the corresponding directions. Movement speed is adjusted based on whether
         * the shift key is held down. The camera's position is updated accordingly.
         * @param ts The timestep representing the time elapsed since the last update.
         */
        void update(Timestep ts);

        /**
         * @brief Handles mouse scroll events for zooming the camera.
         * This method processes mouse scroll events to adjust the camera's distance
         * from its target entity, effectively implementing a zoom functionality.
         * The camera's position is updated accordingly.
         * @param event The mouse scroll event containing scroll delta information.
         */
        void handleEvent(event::EventMouseScroll &event) override;

        /**
         * @brief Handles mouse move events for rotating the camera.
         * This method processes mouse move events to rotate the camera based on
         * mouse movement. It updates the camera's orientation to look around
         * based on the change in mouse position. Pitch is constrained to prevent
         * flipping.
         * @param event The mouse move event containing current mouse position.
         */
        void handleEvent(event::EventMouseMove &event) override;
    };

    /**
     * @brief System for controlling perspective cameras that orbit around a target.
     *
     * This system processes mouse scroll and mouse move events to adjust the camera's distance
     * from its target entity as well as its orientation to always face the target. The camera's
     * position is updated accordingly.
     *
     * @note Component Access Rights:
     *  - WRITE access to components::CameraComponent
     *  - WRITE access to components::PerspectiveCameraTarget
     *  - READ access to components::SceneTag
     *  - WRITE access to components::TransformComponent
     *  - READ access to components::RenderContext (singleton)
     *
     * @note Event Listeners:
     *  - event::EventMouseScroll - For adjusting camera distance from target
     *  - event::EventMouseMove - For orbiting camera around target
     *
     * @note The system only processes camera entities belonging to the currently active scene.
     */
    class PerspectiveCameraTargetSystem final
        : public ecs::QuerySystem<ecs::Write<components::CameraComponent>,
                                  ecs::Write<components::PerspectiveCameraTarget>, ecs::Read<components::SceneTag>,
                                  ecs::Write<components::TransformComponent>,
                                  ecs::ReadSingleton<components::RenderContext>>,
          LISTENS_TO(event::EventMouseScroll, event::EventMouseMove) {
       public:
        PerspectiveCameraTargetSystem();

        /**     * @brief Handles mouse move events to orbit the camera around its target.
         * This method processes mouse move events to rotate the camera around its
         * target entity based on mouse movement. The camera's position and orientation
         * are updated accordingly to maintain focus on the target.
         * @param event The mouse move event containing current mouse position.
         */
        void handleEvent(event::EventMouseMove &event) override;

        /**
         * @brief Handles mouse scroll events to adjust camera distance from target.
         * This method processes mouse scroll events to change the camera's distance
         * from its target entity, effectively implementing a zoom functionality.
         * The camera's position is updated accordingly.
         * @param event The mouse scroll event containing scroll delta information.
         */
        void handleEvent(event::EventMouseScroll &event) override;
    };
} // namespace nexo::system
