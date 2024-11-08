/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** Rendering
*/

#include "Rendering.hpp"
#include "GameEngine.hpp"

#include <rlgl.h>

namespace engine {

    void Engine::startRendering(ecs::SceneID sceneId, core::CameraID cameraId)
    {
        if (_rendering) {
            //TODO: Add error mesage
            std::cout << "Engine already rendering" << std::endl;
            return;
        }
        _rendering = true;
        bool isSceneActive = _coordinator->isSceneActive(sceneId);
        if (!isSceneActive)
            activateScene(sceneId);
        auto &camera = _coordinator->getCamera(sceneId, cameraId);
        auto screenTexture = _coordinator->getCamera(sceneId, cameraId)->getRenderTexture();
        camera->update();
        float cameraPos[3] = { camera->getPosition().x, camera->getPosition().y, camera->getPosition().z };
        SetShaderValue(ecs::components::shader::defaultLightingShader, ecs::components::shader::defaultLightingShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        BeginTextureMode(screenTexture);
        _window->clear(Color{41, 41, 41, 255});
        BeginMode3D(_coordinator->getCamera(sceneId, cameraId)->getCamera());
    }

    void startRendering(ecs::SceneID sceneID, core::CameraID cameraID)
    {
        Engine::getInstance()->startRendering(sceneID, cameraID);
    }

    void Engine::renderGrid(ecs::SceneID sceneId, core::CameraID cameraId)
    {
        if (!_rendering) {
            std::cout << "Engine not rendering, call the startRendering function first" << std::endl;
            //TODO: add error message
            return;
        }
        auto &engineCamera = _coordinator->getCamera(sceneId, cameraId);
        Camera camera = engineCamera->getCamera();
        Vector2 screenSize = engineCamera->getRenderTextureSize();
        float screenWidth = screenSize.x;
        float screenHeight = screenSize.y;

        static Shader shader = LoadShader("src/game_engine/ressources/shaders/grid_fading.vs", 
                                          "src/game_engine/ressources/shaders/grid_fading.fs");
        static int matViewLoc = GetShaderLocation(shader, "matView");
	    static int matProjectionLoc = GetShaderLocation(shader, "matProjection");

        Matrix view = GetCameraMatrix(camera);
        Matrix projection = MatrixPerspective(camera.fovy * DEG2RAD, screenWidth / screenHeight, 0.01f, 1000.0f);
        SetShaderValueMatrix(shader, matViewLoc, view);
	    SetShaderValueMatrix(shader, matProjectionLoc, projection);
        rlDisableDepthTest();
            BeginShaderMode(shader);
                rlPushMatrix();
                    rlTranslatef(0.0f, 0.0f, 0.0f);
                    rlScalef(1000.0f, 1.0f, 1000.0f);

                    rlBegin(RL_TRIANGLES);
                        rlVertex3f(-1, 1, -0);
                        rlVertex3f(-1, -1, 0);
                        rlVertex3f(1, -1, 0);
                        rlVertex3f(1, 1, 0);
                        rlVertex3f(-1, 1, 0);
                        rlVertex3f(1, -1, 0);
					rlEnd();
				rlPopMatrix();
            EndShaderMode();
		rlEnableDepthTest();
    }

    void renderGrid(ecs::SceneID sceneID, core::CameraID cameraID)
    {
        Engine::getInstance()->renderGrid(sceneID, cameraID);
    }

    void Engine::renderAllEntities(ecs::SceneID sceneId, core::CameraID cameraId)
    {
        if (!_rendering) {
            //TODO: Add error message
            std::cout << "Engine not rendering, call the startRendering function first" << std::endl;
            return;
        }
        if (_debug)
            _renderSystemDebug->render(sceneId, _coordinator->getCamera(sceneId, cameraId));
        _renderSystem->render();
    }

    void renderAllEntities(ecs::SceneID sceneId, core::CameraID cameraId)
    {
        Engine::getInstance()->renderAllEntities(sceneId, cameraId);
    }

    void Engine::endRendering(ecs::SceneID sceneId)
    {
        if (!_rendering) {
            std::cout << "Engine not rendering, call the startRendering function first" << std::endl;
            //TODO: Add error message
        }
        _rendering = false;
        EndMode3D();
        EndTextureMode();
        bool isSceneActive = _coordinator->isSceneActive(sceneId);
        if (!isSceneActive)
            deactivateScene(sceneId);
    }

    void endRendering(ecs::SceneID sceneID)
    {
        Engine::getInstance()->endRendering(sceneID);
    }
}