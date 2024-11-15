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
#include <renderer/Buffer.hpp>

#include "core/scene/SceneManager.hpp"
#include "Nexo.hpp"

namespace nexo::editor {
    struct CameraProperties {
        scene::SceneId sceneId;
        std::string layerName;
        std::shared_ptr<camera::Camera> camera;
    };

    using VariantData = std::variant<std::monostate, CameraProperties, std::string>;

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
        SceneManagerBridge(const SceneManagerBridge&) = delete;
        SceneManagerBridge& operator=(const SceneManagerBridge&) = delete;

        // Singleton accessor
        static SceneManagerBridge& getInstance()
        {
            static SceneManagerBridge instance;
            return instance;
        }

        scene::SceneManager &getSceneManager() const {return getApp().getSceneManager();};

        [[nodiscard]] std::vector<scene::SceneId> getSceneIDs() const;
        [[nodiscard]] const layer::LayerStack& getSceneLayers(scene::SceneId sceneId) const;
        [[nodiscard]] std::shared_ptr<camera::Camera> getCameraLayer(scene::SceneId sceneId, const std::string& layerName) const;

        [[nodiscard]] const std::string getSceneName(scene::SceneId sceneId) const;
        [[nodiscard]] std::set<ecs::Entity> getLayerEntities(scene::SceneId sceneId, const std::string& layerName) const;
        [[nodiscard]] std::vector<ecs::Entity> getSceneEntities(scene::SceneId sceneId) const;
        [[nodiscard]] std::set<ecs::Entity> getSceneGlobalEntities(scene::SceneId sceneId) const;
        [[nodiscard]] std::vector<ecs::Entity> getAllEntities() const;
        [[nodiscard]] int getSelectedEntity() const;
        [[nodiscard]] SelectionType getSelectionType() const;
        void setSceneActiveStatus(scene::SceneId sceneId, bool status) const;
        [[nodiscard]] bool isEntitySelected() const;

        void deactivateAllScenes() const;

        void setSelectedEntity(ecs::Entity entity);
        void setData(const VariantData& data) { m_selectionData = data; }
        void setSelectionType(SelectionType type) { m_selectionType = type; }
        void unselectEntity();
        void renameObject(int id, SelectionType type, VariantData& data, const std::string& newName);

    private:
        SceneManagerBridge()  = default;
        ~SceneManagerBridge() = default;

        VariantData m_selectionData{};
        SelectionType m_selectionType = SelectionType::NONE;
        int m_selectedEntity = -1;
        bool m_isEntitySelected = false;
    };

}