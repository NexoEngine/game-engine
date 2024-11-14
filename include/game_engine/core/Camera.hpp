/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Camera.hpp
*/

#pragma once

#include "raylib.h"

#include "game_engine/ecs/components/Shader.hpp"

#include <cstddef>
#include <raymath.h>
#include <iostream>

namespace engine {
    namespace math {
        void ExtractCameraViewComponents(Matrix viewMatrix, Vector3& position, Vector3& target, Vector3& up);
    }

    namespace core {
        using CameraID = std::size_t;

        /**
         * @class Camera
         * @brief Represents a camera in the graphics engine, providing control
         *  over camera properties and rendering.
         *
         * The Camera class encapsulates the functionality for creating and managing a camera, including its
         * position, target, and up vector. It also handles the rendering
         * camera and clipboard text operations.
         */
        class EngineCamera {
            public:
                /**
                 * @brief Constructs a new Camera object with given position, target, and up vector.
                 * @param position Position of the camera, defaulting to {0, 0, 0}.
                 * @param target Target of the camera, defaulting to {0, 0, 0}.
                 * @param up Up vector of the camera, defaulting to {0, 0, 0}.
                 */
                EngineCamera(
                    CameraID id,
                    Vector3 position = {0, 0, 0},
                    Vector3 target = {0, 0, 0},
                    Vector3 up = {0, 1, 0},
                    int mode = CAMERA_PERSPECTIVE,
                    float fov = 90.0f,
                    int renderTextureWidth = 1920,
                    int renderTextureHeight = 1080
                    ) : _id(id),
                    _camera{
                        .position = position,
                        .target = target,
                        .up = up,
                        .fovy = fov,
                        .projection = mode,
                    },
                    _viewTexture(LoadRenderTexture(renderTextureWidth, renderTextureHeight))
                {
                };

                ~EngineCamera()
                {
                    UnloadRenderTexture(_viewTexture);
                };

                /**
                 * @brief Sets the position of the camera.
                 * @param position Position of the camera.
                 */
                void setPosition(Vector3 position)
                {
                    _camera.position = position;
                };
                /**
                 * @brief Sets the target of the camera.
                 * @param target Target of the camera.
                 */
                void setTarget(Vector3 target)
                {
                    _camera.target = target;
                };
                /**
                 * @brief Sets the up vector of the camera.
                 * @param up Up vector of the camera.
                 */
                void setUp(Vector3 up)
                {
                    _camera.up = up;
                };

                /**
                 * @brief Set the Mode object
                 *
                 * @param mode
                 */
                void setMode(int mode)
                {
                    _camera.projection = mode;
                }

                /**
                 * @brief Set the Fov object
                 *
                 * @param fov
                 */
                void setFov(float fov)
                {
                    _camera.fovy = fov;
                }

                /**
                 * @brief Gets the position of the camera.
                 * @return Position of the camera.
                 */
                Vector3 getPosition() const
                {
                    return _camera.position;
                };
                /**
                 * @brief Gets the target of the camera.
                 * @return Target of the camera.
                 */
                Vector3 getTarget() const
                {
                    return _camera.target;
                };
                /**
                 * @brief Gets the up vector of the camera.
                 * @return Up vector of the camera.
                 */
                Vector3 getUp() const
                {
                    return _camera.up;
                };

                /**
                 * @brief Get the Camera object
                 *
                 * @return Camera
                 */
                [[nodiscard]] Camera getCamera() const
                {
                    return _camera;
                }

                [[nodiscard]] CameraID getCameraID() const
                {
                    return _id;
                }

                /**
                 * @brief Get the Mode object
                 *
                 * @return int
                 */
                [[nodiscard]] int getMode(void) const
                {
                    return _camera.projection;
                }

                /**
                 * @brief Get the Fov object
                 *
                 * @return float
                 */
                [[nodiscard]] float getFov(void) const
                {
                    return _camera.fovy;
                }

                [[nodiscard]] const RenderTexture &getRenderTexture() const
                {
                    return _viewTexture;
                }

                /**
                 * @brief Reload the render texture to a new size
                 * @param width The new width of the render texture
                 * @param height The new height of the render texture
                 * @note This unloads the current render texture and creates a new one
                 */
                void updateRenderTextureSize(int width, int height)
                {
                    UnloadRenderTexture(_viewTexture);
                    _viewTexture = LoadRenderTexture(width, height);
                }

                Vector2 getRenderTextureSize(void) const
                {
                    return Vector2{static_cast<float>(_viewTexture.texture.width), static_cast<float>(_viewTexture.texture.height)};
                }

                /**
                 * @brief Gets the view matrix of the camera.
                 * @return The view matrix of the camera.
                 */
                [[nodiscard]] Matrix getViewMatrix() const
                {
                    return GetCameraMatrix(_camera);
                }

                /**
                 * @brief Gets the projection matrix of the camera.
                 * @param aspect The aspect ratio of the camera.
                 * @param nearPlane The near plane of the camera.
                 * @param farPlane The far plane of the camera.
                 * @return The projection matrix of the camera.
                 */
                [[nodiscard]] Matrix getProjectionMatrix(double aspect, double nearPlane, double farPlane) const
                {
                    return MatrixPerspective(_camera.fovy * DEG2RAD, aspect, nearPlane, farPlane);
                }

                /**
                 * @brief Set the view matrix of the camera.
                 * @param matrix The view matrix of the camera.
                 */
                void setViewMatrix(Matrix matrix)
                {
                    math::ExtractCameraViewComponents(matrix, _camera.position, _camera.target, _camera.up);
                }

                void setShaderEnabled(ecs::components::shader::ShaderType shaderType, bool enabled) 
                {
                    if (shaderType < ecs::components::shader::SHADER_COUNT) {
                        _shaderStatus[shaderType] = enabled;
                    }
                }

                bool isShaderEnabled(ecs::components::shader::ShaderType shaderType) const 
                {
                    if (shaderType < ecs::components::shader::SHADER_COUNT) {
                        return _shaderStatus[shaderType];
                    }
                    return false;
                }

                bool isOn = true;

                virtual void update(void) {};

            protected:
                CameraID _id;
                Camera _camera;
                RenderTexture _viewTexture;
                bool _shaderStatus[ecs::components::shader::SHADER_COUNT] = { false };


        };

        class MovableCamera : public EngineCamera {
            public:
                MovableCamera(
                    CameraID id,
                    Vector3 position = {0, 0, 0},
                    Vector3 target = {0, 0, 0},
                    Vector3 up = {0, 1, 0},
                    int mode = CAMERA_PERSPECTIVE,
                    float fov = 90.0f,
                    int renderTextureWidth = 1920,
                    int renderTextureHeight = 1080
                    ) : EngineCamera(id, position, target, up, mode, fov, renderTextureWidth, renderTextureHeight)
                {
                };

                void update(void) override;
        };
    }
}
