/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Camera.cpp
*/

#include "game_engine/core/Camera.hpp"
#include "game_engine/EventHandling.hpp"

namespace engine {
    namespace core {
        void MovableCamera::update(void)
        {
            float cameraSpeed = 0.1f; //TODO: Make it a config option
            float mouseSensitivity = 0.2f; //TODO: Make it a config option

            Vector2 mouseDelta = engine::getMouseDelta();

            //Horizontal rotation
            static float yaw = 0.0f;
            //Vertical rotation
            static float pitch = 0.0f;

            if (engine::isMouseButtonDown(ecs::components::input::MouseButtons::MouseRight)) {
                yaw += mouseDelta.x * mouseSensitivity;
                pitch -= mouseDelta.y * mouseSensitivity;

                // clamp pitch to avoid flipping over (gimbal lock)
                pitch = std::max(-89.0f, std::min(89.0f, pitch));

                Vector3 direction;
                direction.x = cosf(pitch * DEG2RAD) * cosf(yaw * DEG2RAD);
                direction.y = sinf(pitch * DEG2RAD);
                direction.z = cosf(pitch * DEG2RAD) * sinf(yaw * DEG2RAD);
                _camera.target = Vector3Add(_camera.position, direction);
            }

            Vector3 forward = Vector3Normalize(Vector3Subtract(_camera.target, _camera.position));
            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, _camera.up));

            if (engine::isKeyDown(ecs::components::input::Keys::KeyW)) {
                _camera.position = Vector3Add(_camera.position, Vector3Scale(forward, cameraSpeed));
                _camera.target = Vector3Add(_camera.target, Vector3Scale(forward, cameraSpeed));  // Move target with position
            }
            if (engine::isKeyDown(ecs::components::input::Keys::KeyS)) {
                _camera.position = Vector3Subtract(_camera.position, Vector3Scale(forward, cameraSpeed));
                _camera.target = Vector3Subtract(_camera.target, Vector3Scale(forward, cameraSpeed));  // Move target with position
            }
            if (engine::isKeyDown(ecs::components::input::Keys::KeyA)) {
                _camera.position = Vector3Subtract(_camera.position, Vector3Scale(right, cameraSpeed));
                _camera.target = Vector3Subtract(_camera.target, Vector3Scale(right, cameraSpeed));  // Move target with position
            }
            if (engine::isKeyDown(ecs::components::input::Keys::KeyD)) {
                _camera.position = Vector3Add(_camera.position, Vector3Scale(right, cameraSpeed));
                _camera.target = Vector3Add(_camera.target, Vector3Scale(right, cameraSpeed));  // Move target with position
            }
        }
    }
}