//// RenderVideoSystem.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        03/09/2025
//  Description: Source file for the Video System functions
//
///////////////////////////////////////////////////////////////////////////////

#include "RenderVideoSystem.hpp"
#include "Application.hpp"
#include "components/Video.hpp"

namespace nexo::system {

    void RenderVideoSystem::update()
    {
        auto &renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1) return;

        const auto sceneRendered  = static_cast<unsigned int>(renderContext.sceneRendered);
        const SceneType sceneType = renderContext.sceneType;

        const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
            [](const components::SceneTag &tag) { return tag.id; });
        const auto *partition        = scenePartition.getPartition(sceneRendered);
        auto &app                    = Application::getInstance();
        const std::string &sceneName = app.getSceneManager().getScene(sceneRendered).getName();
        if (!partition) {
            LOG_ONCE(NEXO_WARN, "Nothing to render in scene {}, skipping", sceneName);
            return;
        }
        Logger::resetOnce(NEXO_LOG_ONCE_KEY("Nothing to render in scene {}, skipping", sceneName));

        auto videoSpan                                = get<components::VideoComponent>();
        const std::span<const ecs::Entity> entitySpan = m_group->entities();

        for (size_t i = 0; i < videoSpan.size(); ++i) {
            auto &videoComponent = videoSpan[i];
            if ((coord->entityHasComponent<components::MaterialComponent>(entitySpan[i]) &&
                 videoComponent.frames.empty()) ||
                videoComponent.path.empty())
                continue;
            if (videoComponent.frames.empty() && !videoComponent.path.empty() &&
                !videoComponent.loadVideoFrames(videoComponent.path)) {
                LOG_ONCE(NEXO_ERROR, "Failed to load video frames from path: {}", videoComponent.path);
                continue;
            }

            // update material to current frame
            auto &materialComponent =
                coord->getComponent<components::MaterialComponent>(entitySpan[i]);
            auto &currentFrameMaterial = videoComponent.frames[videoComponent.currentFrameIndex++];
            materialComponent.material = currentFrameMaterial;
        }
    }
} // namespace nexo::system
