//// SceneTreeWindow.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/11/2024
//  Description: Source for the scene tree document window
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "Primitive.hpp"

#include <imgui_internal.h>
#include <random>

#include "SceneViewManager.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/Render.hpp"
#include "components/SceneComponents.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"

namespace nexo::editor {

    /**
     * @brief Destructor for the SceneTreeWindow.
     *
     * This default destructor cleans up the SceneTreeWindow instance upon destruction.
     */
    SceneTreeWindow::~SceneTreeWindow()
    = default;

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
            m_renameTarget.reset();
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            m_renameTarget.reset();
        ImGui::PopStyleVar(3);
        ImGui::EndGroup();
    }

    /**
     * @brief Renders a scene object as a selectable tree node.
     *
     * This function displays the scene object as a tree node using a unique label and specified flags.
     * If the node is expanded and clicked, it updates the global selection state—setting the selected 
     * entity, its type, and the associated scene.
     *
     * @param obj The scene object to be displayed.
     * @param uniqueLabel A unique identifier used for the tree node label.
     * @param baseFlags ImGui tree node flags to customize the node's appearance.
     * @return true if the tree node is open (expanded); false otherwise.
     */
    bool SceneTreeWindow::handleSelection(const SceneObject &obj, const std::string &uniqueLabel,
                                          const ImGuiTreeNodeFlags baseFlags) const
    {
        const bool nodeOpen = ImGui::TreeNodeEx(uniqueLabel.c_str(), baseFlags);
        if (!nodeOpen)
            return nodeOpen;
        if (ImGui::IsItemClicked())
        {
	        auto viewManager = m_windowRegistry.getWindow<SceneViewManager>();
	        auto &selector = Selector::get();
        	selector.setSelectedEntity(obj.uuid, obj.data.entity);
         	selector.setSelectionType(obj.type);
          	viewManager->setSelectedScene(obj.data.sceneProperties.sceneId);
        }
        return nodeOpen;
    }

    /**
     * @brief Displays a context menu option to delete a scene.
     *
     * Presents a "Delete Scene" menu item. When selected, it removes the scene associated
     * with the given scene object from the scene view manager using the scene's unique window ID.
     *
     * @param obj The scene object representing the scene to be deleted.
     */
    void SceneTreeWindow::sceneSelected(const SceneObject &obj) const
    {
        auto viewManager = m_windowRegistry.getWindow<SceneViewManager>();

        if (ImGui::MenuItem("Delete Scene"))
            viewManager->removeScene(obj.data.sceneProperties.windowId);
    }

    /**
     * @brief Displays a context menu option to delete a light node.
     *
     * When the "Delete Light" menu item is selected, this function unselects the currently selected entity
     * and deletes the light entity associated with the provided scene object.
     *
     * @param obj The scene object representing the light node to delete.
     */
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

    /**
     * @brief Displays a context menu option for deleting a camera.
     *
     * When the "Delete Camera" menu item is selected, the function retrieves the active SceneViewManager,
     * finds the scene associated with the camera's window identifier, and deletes the camera entity from that scene.
     * It also clears any current selection and notifies the application to delete the camera entity.
     *
     * @param obj The scene object representing the camera to be deleted.
     */
    void SceneTreeWindow::cameraSelected(const SceneObject &obj) const
    {
    	auto &app = Application::getInstance();
    	auto &selector = Selector::get();
        if (ImGui::MenuItem("Delete Camera"))
        {
        	auto viewManager = m_windowRegistry.getWindow<SceneViewManager>();
         	viewManager->getScene(obj.data.sceneProperties.windowId)->deleteCamera(obj.data.entity);
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

    /**
     * @brief Displays a modal popup for creating a new scene.
     *
     * Renders a popup with an input field prompting the user to enter a scene name. When the "Create"
     * button is pressed with a non-empty name, it attempts to add the new scene via the SceneViewManager
     * fetched from the window registry, logging a warning if the creation fails due to a non-unique name.
     * The popup is closed either upon successful scene creation or when the "Cancel" button is clicked.
     */
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
                    auto viewManager = m_windowRegistry.getWindow<SceneViewManager>();
                    if (!viewManager->addNewScene(sceneNameBuffer))
                        LOG(NEXO_WARN, "Failed to create scene, check if the name is unique");
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

    /**
     * @brief Renders and manages the Scene Tree window.
     *
     * Initializes and displays the Scene Tree window using ImGui by setting its initial position and size.
     * On the first opening, it performs a docking setup, and then renders the hierarchical scene nodes
     * if present. Additionally, it handles right-click actions to open the context menu and invokes
     * context and creation menus for scene management.
     */
    void SceneTreeWindow::show()
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 40), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Scene Tree", &m_opened, ImGuiWindowFlags_NoCollapse))
        {
	       	if (m_firstOpened)
	        	firstDockSetup("Scene Tree");
            // Opens the right click popup when no items are hovered
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(
                    ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered())
                m_popupManager.openPopup("Scene Tree Context Menu");
            if (!root_.children.empty())
                showNode(root_);
            sceneContextMenu();
            sceneCreationMenu();

            ImGui::End();
        }
    }

    /**
     * @brief Creates a new scene node for the scene tree.
     *
     * Constructs and initializes a SceneObject to represent a scene within the scene tree UI.
     * The node's properties are configured using the provided scene and UI identifiers: scene properties 
     * and type are set from these identifiers, the scene's unique UUID is obtained from the Scene Manager,
     * and a UI handle is generated by concatenating a scene icon with the scene name from the Scene View Manager.
     *
     * @param sceneId The unique identifier of the scene.
     * @param uiId The identifier for the scene's UI element.
     * @return SceneObject The newly created scene node with initialized properties.
     */
    SceneObject SceneTreeWindow::newSceneNode(const scene::SceneId sceneId, const WindowId uiId) const
    {
        SceneObject sceneNode;
        auto const &viewManager = m_windowRegistry.getWindow<SceneViewManager>();
        const std::string uiName = ObjectTypeToIcon.at(SelectionType::SCENE) + viewManager->getSceneName(uiId);
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

    /**
     * @brief Rebuilds the scene tree GUI by clearing old nodes and repopulating it with current scene data.
     *
     * This function resets the root node and light counters, retrieves the list of open scenes from the 
     * SceneViewManager via the window registry, and creates a new scene node for each open scene. 
     * It then generates child nodes for ambient, directional, point, and spot lights, as well as cameras 
     * and generic entities using a templated generateNodes method. Finally, it appends these nodes as children 
     * to the root node, effectively updating the scene tree display.
     */
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
        const auto &scenes = m_windowRegistry.getWindow<SceneViewManager>()->getOpenScenes();
        std::map<scene::SceneId, SceneObject> sceneNodes;
        for (const auto &[sceneId, windowId] : scenes)
        {
        	sceneNodes[sceneId] = newSceneNode(sceneId, windowId);
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
