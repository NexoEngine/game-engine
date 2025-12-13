//// Application.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the main application
//
///////////////////////////////////////////////////////////////////////////////

#include <fstream>

#include "Application.hpp"
#include "SystemProfiler.hpp"
#include "renderer/Renderer3D.hpp"
#include "systems/ScriptingSystem.hpp"

#include "components/Editor.hpp"
#include "components/Name.hpp"
#include "components/Parent.hpp"
#include "components/Render.hpp"
#include "components/Uuid.hpp"

std::unique_ptr<nexo::Application> nexo::Application::_instance          = nullptr;
std::shared_ptr<nexo::ecs::Coordinator> nexo::Application::m_coordinator = nullptr;

namespace nexo {

    int Application::shutdownScripting() const
    {
        return m_scriptingSystem->shutdown();
    }

    Application::Application()
    {
        m_window       = renderer::NxWindow::create();
        m_eventManager = std::make_shared<event::EventManager>();
        registerAllDebugListeners();
        registerSignalListeners();

        // Debug flags
        // m_eventDebugFlags |= DEBUG_LOG_KEYBOARD_EVENT;

        m_coordinator = std::make_shared<ecs::Coordinator>();

        ecs::System::coord = m_coordinator;

        LOG(NEXO_DEV, "Application created");
    }

    void Application::displayProfileResults() const
    {
        for (auto &result : m_profilesResults) {
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(3) << result.time;
            std::string label = stream.str() + "ms" + " " + result.name;
            LOG(NEXO_DEV, "{}", label);
        }
    }

    void Application::beginFrame()
    {
        const auto time             = glfwGetTime();
        m_worldState.time.deltaTime = time - m_worldState.time.totalTime;
        m_worldState.time.totalTime = time;
        m_worldState.stats.frameCount += 1;
    }

    void Application::run(const SceneInfo &sceneInfo)
    {
        PROFILE_SYSTEM("Application::run", 0);
        static bool areVideoLoaded = false;
        auto &renderContext        = m_coordinator->getSingletonComponent<components::RenderContext>();

        if (isInPlayMode()) {
            m_scriptingSystem->update();
        }

        if (!m_isMinimized) {
            renderContext.sceneRendered = static_cast<int>(sceneInfo.id);
            renderContext.sceneType     = sceneInfo.sceneType;
            if (sceneInfo.isChildWindow) {
                renderContext.isChildWindow     = true;
                renderContext.viewportBounds[0] = sceneInfo.viewportBounds[0];
                renderContext.viewportBounds[1] = sceneInfo.viewportBounds[1];
            }
            if (m_SceneManager.getScene(sceneInfo.id).isRendered()) {
                m_transformSystem->update();
                m_cameraContextSystem->update();
                m_lightSystem->update(renderContext);
                m_renderCommandSystem->update();
                m_renderBillboardSystem->update();

                //m_aabbdebugSystem->update();
                if (!areVideoLoaded) {
                    m_renderVideoSystem->update();
                    areVideoLoaded = true;
                }
                {
                    PROFILE_SYSTEM("CameraPipeline", static_cast<size_t>(renderContext.cameras.size()));
                    for (auto &camera : renderContext.cameras) {
                        camera.pipeline.execute();
                    }
                }
                // We have to unbind after the whole pipeline since multiple passes can use the same textures
                // but we cant bind everything beforehand since a resize can be triggered and invalidate the whole state
                renderer::NxRenderer3D::get().unbindTextures();
                m_renderVideoSystem->update();

                if (isInPlayMode()) {
                    m_physicsSystem->update();
                }
            }
            if (m_SceneManager.getScene(sceneInfo.id).isActive()) {
                m_perspectiveCameraControllerSystem->update(m_worldState.time.deltaTime);
            }
        }

        // Update (swap buffers and poll events)
        if (sceneInfo.renderingType == RenderingType::WINDOW) m_window->onUpdate();
        m_eventManager->dispatchEvents();
        renderContext.reset();
        if (m_displayProfileResult) displayProfileResults();
    }

    void Application::endFrame() const
    {
        m_eventManager->clearEvents();
    }

    ecs::Entity Application::createEntity()
    {
        return m_coordinator->createEntity();
    }

    void Application::deleteEntity(const ecs::Entity entity)
    {
        // First, recursively delete all children of this entity
        deleteEntityChildren(entity);

        // Then, remove this entity from its parent's children list (if it has a parent)
        removeEntityFromParent(entity);

        // Finally, handle the scene tag and destroy the entity as in the original code
        const auto tag = m_coordinator->tryGetComponent<components::SceneTag>(entity);
        if (tag) {
            const unsigned int sceneId = tag->get().id;
            m_SceneManager.getScene(sceneId).removeEntity(entity);
        }
        m_coordinator->destroyEntity(entity);
    }

    void Application::removeEntityFromParent(const ecs::Entity entity)
    {
        // Get the parent component to find the parent entity
        auto parentComponent = m_coordinator->tryGetComponent<components::ParentComponent>(entity);
        if (!parentComponent || parentComponent->get().parent == ecs::INVALID_ENTITY) return;

        const ecs::Entity parentEntity = parentComponent->get().parent;

        // Get the parent's transform component which now stores children
        auto parentTransform = m_coordinator->tryGetComponent<components::TransformComponent>(parentEntity);
        if (parentTransform) {
            // Remove this entity from parent's children vector
            parentTransform->get().removeChild(entity);
        }
    }

    void Application::deleteEntityChildren(const ecs::Entity entity)
    {
        // Check if this entity has a transform component with children
        auto transform = m_coordinator->tryGetComponent<components::TransformComponent>(entity);
        if (!transform || transform->get().children.empty()) return;

        // Create a copy of the children vector since we'll be modifying it during iteration
        const std::vector<ecs::Entity> childrenCopy = transform->get().children;

        // Delete each child entity recursively
        for (const auto &childEntity : childrenCopy) {
            if (childEntity != ecs::INVALID_ENTITY && childEntity != entity) // Avoid circular references
                deleteEntity(childEntity);
        }

        // Clear the children list to avoid dangling references
        transform->get().children.clear();
    }

    void Application::markHierarchyDirty(ecs::Entity entity)
    {
        ecs::Entity currentEntity = entity;
        ecs::Entity rootEntity    = entity;

        while (m_coordinator->entityHasComponent<components::ParentComponent>(currentEntity)) {
            const auto &parentComp = m_coordinator->getComponent<components::ParentComponent>(currentEntity);
            currentEntity          = parentComp.parent;
            rootEntity             = currentEntity;
        }

        if (m_coordinator->entityHasComponent<components::RootComponent>(rootEntity)) {
            auto &rootComp          = m_coordinator->getComponent<components::RootComponent>(rootEntity);
            rootComp.hierarchyDirty = true;
        }
    }
    bool Application::save(const std::filesystem::path &path)
    {
        try {
            nexo::json root;
            root["component_arrays"] = nexo::json::array();

            const auto &typeMap = m_coordinator->getTypeIdToTypeIndex();
            const auto &descs = m_coordinator->getComponentDescriptions();

            for (const auto &p : typeMap) {
                const ecs::ComponentType typeId = p.first;
                const std::type_index &ti = p.second;

                nexo::json arrJson;
                const auto descIt = descs.find(typeId);
                std::string name = (descIt != descs.end() && descIt->second) ? descIt->second->name : ti.name();

                bool ok = m_coordinator->serializeComponentArray(ti, arrJson, nexo::save::SerializationContext::editor());
                if (!ok) continue; // skip types without serializer

                nexo::json entry;
                entry["typeId"] = typeId;
                entry["name"] = name;
                entry["data"] = arrJson;
                root["component_arrays"].push_back(entry);
            }

            // Write file
            std::ofstream ofs(path.string(), std::ios::out | std::ios::trunc);
            if (!ofs) {
                LOG(NEXO_ERROR, "Failed to open save file: {}", path.string());
                return false;
            }
            ofs << root.dump(4);
            ofs.close();

            std::filesystem::path binPath = path;
            binPath.replace_extension(".nexob");

            std::ofstream ofsb(binPath, std::ios::out | std::ios::binary);
            if (!ofsb) {
                LOG(NEXO_ERROR, "Failed to open binary save file: {}", binPath.string());
                return false;
            }

            std::vector<uint8_t> output_vector;
            json::to_msgpack(root, output_vector);
            ofsb.write(reinterpret_cast<const char *>(output_vector.data()), output_vector.size());
            ofsb.close();

            LOG(NEXO_INFO, "Game saved to {}", path.string());
            return true;
        } catch (const std::exception &e) {
            LOG(NEXO_ERROR, "Save failed: {}", e.what());
            return false;
        }
    }
} // namespace nexo
