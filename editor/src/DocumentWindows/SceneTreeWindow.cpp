//// SceneTreeWindow.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        13/11/2024
//  Description: Source for the scene tree document window
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "DocumentWindows/InspectorWindow.hpp"
#include "Primitive.hpp"

#include <imgui_internal.h>
#include <random>

#include "EditorScene.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/Render.hpp"
#include "components/SceneComponents.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "context/Selector.hpp"

namespace nexo::editor {

    SceneTreeWindow::~SceneTreeWindow() = default;

    void SceneTreeWindow::setup()
    {}

    void SceneTreeWindow::shutdown()
    {}

    void SceneTreeWindow::handleRename(SceneObject &obj)
    {
        ImGui::BeginGroup();
        ImGui::TextUnformatted(ObjectTypeToIcon.at(obj.type).c_str());
        ImGui::SameLine();

        char buffer[256];
        const std::string editableName = obj.uiName.substr(ObjectTypeToIcon.at(obj.type).size());
        buffer[sizeof(buffer) - 1] = '\0';
        strncpy(buffer, editableName.c_str(), sizeof(buffer));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); // Remove border
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f); // No rounding
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

        if (ImGui::InputText("##Rename", buffer, sizeof(buffer),
                             ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            obj.uiName = ObjectTypeToIcon.at(obj.type) + std::string(buffer);
            auto &selector = Selector::get();
            selector.setUiHandle(obj.uuid, obj.uiName);
            if (obj.type == SelectionType::SCENE)
            {
                auto &app = getApp();
                app.getSceneManager().getScene(obj.data.sceneProperties.sceneId).setName(obj.uiName);
            }
            m_renameTarget.reset();
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            m_renameTarget.reset();
        ImGui::PopStyleVar(3);
        ImGui::EndGroup();
    }

    bool SceneTreeWindow::handleSelection(const SceneObject &obj, const std::string &uniqueLabel,
                                          const ImGuiTreeNodeFlags baseFlags) const
    {
        const bool nodeOpen = ImGui::TreeNodeEx(uniqueLabel.c_str(), baseFlags);
        if (!nodeOpen)
            return nodeOpen;
        if (ImGui::IsItemClicked())
        {
	        auto &selector = Selector::get();
        	selector.setSelectedEntity(obj.uuid, obj.data.entity);
         	selector.setSelectionType(obj.type);
          	selector.setSelectedScene(obj.data.sceneProperties.sceneId);
        }
        return nodeOpen;
    }

    void SceneTreeWindow::sceneSelected([[maybe_unused]] const SceneObject &obj) const
    {
    	//TODO: Delete scene
    }

    void SceneTreeWindow::lightSelected(const SceneObject &obj) const
    {
        auto &app = Application::getInstance();
        auto &selector = Selector::get();
        if (ImGui::MenuItem("Delete Light"))
        {
        	selector.unselectEntity();
        	app.deleteEntity(obj.data.entity);
        }
    }

    void SceneTreeWindow::cameraSelected(const SceneObject &obj) const
    {
    	auto &app = Application::getInstance();
    	auto &selector = Selector::get();
        if (ImGui::MenuItem("Delete Camera"))
        {
         	const auto &scenes = m_windowRegistry.getWindows<EditorScene>();
          	for (const auto &scene : scenes)
           		scene->deleteCamera(obj.data.entity);
        	selector.unselectEntity();
        	app.deleteEntity(obj.data.entity);
        }
    }

    void SceneTreeWindow::entitySelected(const SceneObject &obj) const
    {
        if (ImGui::MenuItem("Delete Entity"))
        {
        	auto &selector = Selector::get();
         	selector.unselectEntity();
            auto &app = nexo::getApp();
            app.deleteEntity(obj.data.entity);
        }
    }

    void SceneTreeWindow::showNode(SceneObject &object)
    {
        ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                       ImGuiTreeNodeFlags_SpanAvailWidth;
        // Checks if the object is at the end of a tree
        const bool leaf = object.children.empty();
        if (leaf)
            baseFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        // Checks if the object is selected
        if (auto const &selector = Selector::get(); selector.isEntitySelected() && object.uuid == selector.getSelectedUuid())
            baseFlags |= ImGuiTreeNodeFlags_Selected;

        bool nodeOpen = false;
        const std::string uniqueLabel = object.uiName;

        // If the user wishes to rename handle the rename, else handle the selection
        if (m_renameTarget && m_renameTarget->first == object.type && m_renameTarget->second == object.uuid)
            handleRename(object);
        else
            nodeOpen = handleSelection(object, uniqueLabel, baseFlags);

        // Handles the right click on each different type of object
        if (object.type != SelectionType::NONE && ImGui::BeginPopupContextItem(uniqueLabel.c_str()))
        {
            // Renaming works on every object excepts entities and cameras
            if (ImGui::MenuItem("Rename"))
            {
                m_renameTarget = {object.type, object.uuid};
                m_renameBuffer = object.uiName;
            }
            if (object.type == SelectionType::SCENE)
                sceneSelected(object);
            else if (object.type == SelectionType::DIR_LIGHT || object.type == SelectionType::POINT_LIGHT || object.type == SelectionType::SPOT_LIGHT)
                lightSelected(object);
            else if (object.type == SelectionType::CAMERA)
                cameraSelected(object);
            else if (object.type == SelectionType::ENTITY)
                entitySelected(object);
            ImGui::EndPopup();
        }

        // Go further into the tree
        if (nodeOpen && !leaf)
        {
            for (auto &child: object.children)
            {
                showNode(child);
            }
            ImGui::TreePop();
        }
    }

    void SceneTreeWindow::sceneContextMenu()
    {
        if (m_popupManager.showPopup("Scene Tree Context Menu"))
        {
            if (ImGui::MenuItem("Create Scene"))
                m_popupManager.openPopup("Create New Scene");
            m_popupManager.closePopup();
        }
    }

    void SceneTreeWindow::sceneCreationMenu()
    {
        if (m_popupManager.showPopupModal("Create New Scene"))
        {
            static char sceneNameBuffer[256] = "";

            ImGui::Text("Enter Scene Name:");
            ImGui::InputText("##SceneName", sceneNameBuffer, sizeof(sceneNameBuffer));

            if (ImGui::Button("Create"))
            {
                if (const std::string newSceneName(sceneNameBuffer); !newSceneName.empty())
                {
                	//TOOD: create scene
                    auto newScene = std::make_shared<EditorScene>(sceneNameBuffer, m_windowRegistry);
                    newScene->setDefault();
                    newScene->setup();
                    m_windowRegistry.registerWindow<EditorScene>(newScene);
                    memset(sceneNameBuffer, 0, sizeof(sceneNameBuffer));

                    m_popupManager.closePopupInContext();
                } else
                    LOG(NEXO_WARN, "Scene name is empty !");
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
                m_popupManager.closePopupInContext();

            m_popupManager.closePopup();
        }
    }

    void SceneTreeWindow::show()
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 40), ImGuiCond_FirstUseEver);

        if (ImGui::Begin(ICON_FA_SITEMAP "Scene Tree" "###" NEXO_WND_USTRID_SCENE_TREE, &m_opened, ImGuiWindowFlags_NoCollapse))
        {
	        firstDockSetup(NEXO_WND_USTRID_SCENE_TREE);
            // Opens the right click popup when no items are hovered
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(
                    ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered())
                m_popupManager.openPopup("Scene Tree Context Menu");
            if (!root_.children.empty()) {
                for (auto &node : root_.children)
                    showNode(node);
            }
            sceneContextMenu();
            sceneCreationMenu();


        }
        ImGui::End();
    }

    SceneObject SceneTreeWindow::newSceneNode(const std::string &sceneName, const scene::SceneId sceneId, const WindowId uiId) const
    {
        SceneObject sceneNode;
        const std::string uiName = ObjectTypeToIcon.at(SelectionType::SCENE) + sceneName;
        sceneNode.data.sceneProperties = SceneProperties{sceneId, uiId};
        sceneNode.data.entity = sceneId;
        sceneNode.type = SelectionType::SCENE;
        auto &app = Application::getInstance();
        auto &selector = Selector::get();
        sceneNode.uuid = app.getSceneManager().getScene(sceneId).getUuid();
        sceneNode.uiName = selector.getUiHandle(sceneNode.uuid, uiName);
        return sceneNode;
    }

    void SceneTreeWindow::newLightNode(SceneObject &lightNode, const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity, const std::string &uiName) const
    {
      	const SceneProperties sceneProperties{sceneId, uiId};
       	lightNode.data.sceneProperties = sceneProperties;
        lightNode.data.entity = lightEntity;
        auto &selector = Selector::get();
        const auto entityUuid = Application::m_coordinator->tryGetComponent<components::UuidComponent>(lightEntity);
        if (entityUuid)
        {
       		lightNode.uuid = entityUuid->get().uuid;
         	lightNode.uiName = selector.getUiHandle(entityUuid->get().uuid, uiName);
        } else
        	lightNode.uiName = uiName;
    }

    SceneObject SceneTreeWindow::newAmbientLightNode(const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity) const
    {
    	SceneObject lightNode;
    	lightNode.type = SelectionType::AMBIENT_LIGHT;
        const std::string uiName = std::format("{}Ambient light ", ObjectTypeToIcon.at(lightNode.type));
      	newLightNode(lightNode, sceneId, uiId, lightEntity, uiName);
       	return lightNode;
    }

    SceneObject SceneTreeWindow::newDirectionalLightNode(const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity)
    {
   		SceneObject lightNode;
   		lightNode.type = SelectionType::DIR_LIGHT;
        const std::string uiName = std::format("{}Directional light {}", ObjectTypeToIcon.at(lightNode.type), ++m_nbDirLights);
      	newLightNode(lightNode, sceneId, uiId, lightEntity, uiName);
       	return lightNode;
    }

    SceneObject SceneTreeWindow::newSpotLightNode(const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity)
    {
  		SceneObject lightNode;
  		lightNode.type = SelectionType::SPOT_LIGHT;
    	const std::string uiName = std::format("{}Spot light {}", ObjectTypeToIcon.at(lightNode.type), ++m_nbSpotLights);
     	newLightNode(lightNode, sceneId, uiId, lightEntity, uiName);
      	return lightNode;
    }

    SceneObject SceneTreeWindow::newPointLightNode(const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity)
    {
  		SceneObject lightNode;
  		lightNode.type = SelectionType::POINT_LIGHT;
    	const std::string uiName = std::format("{}Point light {}", ObjectTypeToIcon.at(lightNode.type), ++m_nbPointLights);
     	newLightNode(lightNode, sceneId, uiId, lightEntity, uiName);
      	return lightNode;
    }

    SceneObject SceneTreeWindow::newCameraNode(const scene::SceneId sceneId, const WindowId uiId,
                                               const ecs::Entity cameraEntity) const
    {
        SceneObject cameraNode;
        const std::string uiName = ObjectTypeToIcon.at(SelectionType::CAMERA) + std::string("Camera");
        cameraNode.type = SelectionType::CAMERA;
        const SceneProperties sceneProperties{sceneId, uiId};
        cameraNode.data.sceneProperties = sceneProperties;
        cameraNode.data.entity = cameraEntity;
        auto &selector = Selector::get();
        const auto entityUuid = nexo::Application::m_coordinator->tryGetComponent<components::UuidComponent>(cameraEntity);
        if (entityUuid)
        {
       		cameraNode.uuid = entityUuid->get().uuid;
         	cameraNode.uiName = selector.getUiHandle(entityUuid->get().uuid, uiName);
        } else
        	cameraNode.uiName = uiName;
        return cameraNode;
    }

    SceneObject SceneTreeWindow::newEntityNode(const scene::SceneId sceneId, const WindowId uiId,
                                               const ecs::Entity entity) const
    {
     	auto &selector = Selector::get();
        SceneObject entityNode;
        const std::string uiName = std::format("{}{}", ObjectTypeToIcon.at(SelectionType::ENTITY), entity);
        entityNode.type = SelectionType::ENTITY;
        entityNode.data.sceneProperties = SceneProperties{sceneId, uiId};
        entityNode.data.entity = entity;
        const auto entityUuid = nexo::Application::m_coordinator->tryGetComponent<components::UuidComponent>(entity);
        if (entityUuid)
        {
       		entityNode.uuid = entityUuid->get().uuid;
         	entityNode.uiName = selector.getUiHandle(entityUuid->get().uuid, uiName);
        }
        else
        	entityNode.uiName = uiName;
        return entityNode;
    }

    void SceneTreeWindow::update()
    {
        root_.uiName = "Scene Tree";
        root_.data.entity = -1;
        root_.type = SelectionType::NONE;
        root_.children.clear();
        m_nbPointLights = 0;
        m_nbDirLights = 0;
        m_nbSpotLights = 0;

        // Retrieves the scenes that are displayed on the GUI
        const auto &scenes = m_windowRegistry.getWindows<EditorScene>();
        std::map<scene::SceneId, SceneObject> sceneNodes;
        for (const auto &scene : scenes)
        {
        	sceneNodes[scene->getSceneId()] = newSceneNode(scene->getWindowName(), scene->getSceneId(), windowId);
        }

        generateNodes<components::AmbientLightComponent, components::SceneTag>(
        	sceneNodes,
    	    [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return this->newAmbientLightNode(sceneId, uiId, entity);
         });
        generateNodes<components::DirectionalLightComponent, components::SceneTag>(
        	sceneNodes,
    	    [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return this->newDirectionalLightNode(sceneId, uiId, entity);
         });
        generateNodes<components::PointLightComponent, components::SceneTag>(
        	sceneNodes,
    	    [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return this->newPointLightNode(sceneId, uiId, entity);
         });
        generateNodes<components::SpotLightComponent, components::SceneTag>(
        	sceneNodes,
    	    [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return this->newSpotLightNode(sceneId, uiId, entity);
         });

        generateNodes<components::CameraComponent, components::SceneTag>(
        	sceneNodes,
    	    [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return this->newCameraNode(sceneId, uiId, entity);
         });

        generateNodes<components::RenderComponent, components::TransformComponent, components::SceneTag>(
        	sceneNodes,
    	    [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return this->newEntityNode(sceneId, uiId, entity);
         });

        for (const auto &[_, sceneNode] : sceneNodes)
        {
        	root_.children.push_back(sceneNode);
        }
    }
}
