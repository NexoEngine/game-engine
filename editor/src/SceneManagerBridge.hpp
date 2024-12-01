//// SceneManagerBridge.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the scene manager bridge class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <variant>

#include "core/scene/SceneManager.hpp"
#include "Nexo.hpp"

namespace nexo::editor {

    using WindowId = unsigned int;

    inline WindowId nextWindowId = 0;

    struct SceneProperties {
        scene::SceneId sceneId;
        WindowId windowId;
    };

    struct LayerProperties {
        SceneProperties sceneProps;
        int layerId;
    };

    struct CameraProperties {
        LayerProperties layerProps;
        std::shared_ptr<camera::Camera> camera;
    };

    struct EntityProperties {
        LayerProperties layerProps;
        ecs::Entity entity;
    };

    using VariantData = std::variant<std::monostate, EntityProperties, LayerProperties, CameraProperties, SceneProperties>;

    enum class SelectionType {
        NONE,
        ENTITY,
        CAMERA,
        LAYER,
        SCENE,
        FOLDER,
        UNKNOWN
    };

    class SceneManagerBridge {
    public:
        SceneManagerBridge()  = default;

        [[nodiscard]] static scene::SceneManager &getSceneManager() {return getApp().getSceneManager();};

        [[nodiscard]] const layer::LayerStack& getSceneLayers(scene::SceneId sceneId) const;
        [[nodiscard]] std::shared_ptr<camera::Camera> getCameraLayer(scene::SceneId sceneId, scene::LayerId id) const;

        [[nodiscard]] std::string getSceneName(scene::SceneId sceneId) const;
        [[nodiscard]] std::set<ecs::Entity> getLayerEntities(scene::SceneId sceneId, scene::LayerId id) const;
        [[nodiscard]] std::vector<ecs::Entity> getSceneEntities(scene::SceneId sceneId) const;
        [[nodiscard]] std::vector<ecs::Entity> getSceneRenderedEntities(scene::SceneId sceneId) const;
        [[nodiscard]] std::set<ecs::Entity> getSceneGlobalEntities(scene::SceneId sceneId) const;
        [[nodiscard]] std::vector<ecs::Entity> getAllEntities() const;
        [[nodiscard]] int getSelectedEntity() const;
        [[nodiscard]] SelectionType getSelectionType() const;
        static bool isSceneRendered(const scene::SceneId id) { return getApp().getSceneManager().isSceneRendered(id); };
        void setSceneActiveStatus(scene::SceneId sceneId, bool status) const;
        void setLayerRenderStatus(scene::SceneId sceneId, scene::LayerId id, bool status) const;
        [[nodiscard]] bool isEntitySelected() const;

        void deactivateAllScenes() const;

        void setSelectedEntity(ecs::Entity entity);
        void setData(const VariantData& data) { m_selectionData = data; }
        [[nodiscard]] const VariantData &getData() const {return m_selectionData; };
        void setSelectionType(const SelectionType type) { m_selectionType = type; }
        void unselectEntity();
        void renameObject(SelectionType type, VariantData &data, const std::string &newName) const;

        ~SceneManagerBridge() = default;
    private:
        std::vector<SceneProperties> m_openScenes{};

        VariantData m_selectionData{};
        SelectionType m_selectionType = SelectionType::NONE;
        int m_selectedEntity = -1;
        bool m_isEntitySelected = false;
    };

}