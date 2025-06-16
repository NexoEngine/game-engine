//// Widgets.cpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/02/2025
//  Description: Source file for the widgets components
//
///////////////////////////////////////////////////////////////////////////////

#include "Widgets.hpp"

#include <Editor.hpp>
#include <EntityFactory3D.hpp>
#include <imgui.h>
#include <context/ActionManager.hpp>
#include <DocumentWindows/PopupManager.hpp>

#include "IconsFontAwesome.h"
#include "Nexo.hpp"
#include "ImNexo.hpp"

namespace ImNexo {

	bool ColorEditor(
			const std::string &label,
			glm::vec4 *selectedEntityColor,
			ImGuiColorEditFlags *colorPickerMode,
			bool *showPicker,
			const ImGuiColorEditFlags colorButtonFlags
	) {
		const ImGuiStyle &style = ImGui::GetStyle();
        const ImVec2 contentAvailable = ImGui::GetContentRegionAvail();
        bool colorModified = false;

        const std::string colorButton = std::string("##ColorButton") + label;

		const ImVec2 cogIconSize = ImGui::CalcTextSize(ICON_FA_COG);
		const ImVec2 cogIconPadding = style.FramePadding;
		const ImVec2 itemSpacing = style.ItemSpacing;

        // Color button
        ColorButton(
           	colorButton,
           	ImVec2(contentAvailable.x - cogIconSize.x - cogIconPadding.x * 2 - itemSpacing.x, 0), // Make room for the cog button
           	ImVec4(selectedEntityColor->x, selectedEntityColor->y, selectedEntityColor->z, selectedEntityColor->w),
            showPicker,
            colorButtonFlags
        );

        ImGui::SameLine();

		const std::string pickerSettings = std::string("##PickerSettings") + label;
        const std::string colorPickerPopup = std::string("##ColorPickerPopup") + label;

		// Cog button
        if (Button(std::string(ICON_FA_COG) + pickerSettings)) {
           	ImGui::OpenPopup(colorPickerPopup.c_str());
        }

        if (ImGui::BeginPopup(colorPickerPopup.c_str()))
        {
            ImGui::Text("Picker Mode:");
            if (ImGui::RadioButton("Hue Wheel", *colorPickerMode == ImGuiColorEditFlags_PickerHueWheel))
                *colorPickerMode = ImGuiColorEditFlags_PickerHueWheel;
            if (ImGui::RadioButton("Hue bar", *colorPickerMode == ImGuiColorEditFlags_PickerHueBar))
                *colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
            ImGui::EndPopup();
        }

        const std::string colorPickerInline = std::string("##ColorPickerInline") + label;
        if (*showPicker)
        {
            ImGui::Spacing();
            colorModified = ImGui::ColorPicker4(colorPickerInline.c_str(),
                                reinterpret_cast<float*>(selectedEntityColor), *colorPickerMode);
            if (ImGui::IsItemActive())
                setItemActive();
            if (ImGui::IsItemActivated())
                setItemActivated();
            if (ImGui::IsItemDeactivated())
                setItemDeactivated();
        }
        return colorModified;
	}

	void ButtonDropDown(const ImVec2& buttonPos, const ImVec2 buttonSize, const std::vector<ButtonProps> &buttonProps, bool &closure, DropdownOrientation orientation)
	{
	    constexpr float buttonSpacing = 5.0f;
        constexpr float padding = 10.0f;

        // Calculate menu dimensions
        const float menuWidth = buttonSize.x + padding;  // Add padding
        const float menuHeight =static_cast<float>(buttonProps.size()) * buttonSize.y +
                                (static_cast<float>(buttonProps.size()) - 1.0f) * buttonSpacing + 2 * buttonSpacing;

        // Calculate menu position based on orientation
        ImVec2 menuPos;
        switch (orientation) {
            case DropdownOrientation::DOWN:
                menuPos = ImVec2(buttonPos.x - padding / 2.0f, buttonPos.y + buttonSize.y);
                break;
            case DropdownOrientation::UP:
                menuPos = ImVec2(buttonPos.x - padding / 2.0f, buttonPos.y - menuHeight);
                break;
            case DropdownOrientation::RIGHT:
                menuPos = ImVec2(buttonPos.x + buttonSize.x, buttonPos.y - padding / 2.0f);
                break;
            case DropdownOrientation::LEFT:
                menuPos = ImVec2(buttonPos.x - menuWidth, buttonPos.y - padding / 2.0f);
                break;
        }

        // Adjust layout for horizontal orientations
        bool isHorizontal = (orientation == DropdownOrientation::LEFT ||
                            orientation == DropdownOrientation::RIGHT);

        // For horizontal layouts, swap width and height
        ImVec2 menuSize = isHorizontal ?
                         ImVec2(menuHeight, buttonSize.y + 10.0f) :
                         ImVec2(menuWidth, menuHeight);

        ImGui::SetNextWindowPos(menuPos);
        ImGui::SetNextWindowSize(menuSize);
        ImGui::SetNextWindowBgAlpha(0.2f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, buttonSpacing));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                           isHorizontal ? ImVec2(buttonSpacing, 0) : ImVec2(0, buttonSpacing));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        if (ImGui::Begin("##PrimitiveMenuOverlay", nullptr,
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
        {
            for (const auto &button : buttonProps)
            {
                // Strangely here the clicked inside here does not seem to work
                IconGradientButton(button.uniqueId, button.icon, ImVec2(buttonSize.x, buttonSize.y), button.buttonGradient);
                // So we rely on IsItemClicked from imgui
                if (button.onClick && ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    button.onClick();
                    closure = false;
                }
                if (button.onRightClick && ImGui::IsItemClicked(ImGuiMouseButton_Right))
                {
                    button.onRightClick();
                }
                if (!button.tooltip.empty() && ImGui::IsItemHovered())
                    ImGui::SetTooltip("%s", button.tooltip.c_str());
            }
        }
        // Check for clicks outside to close menu
        if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered())
        {
            closure = false;
        }
        ImGui::End();

        ImGui::PopStyleVar(3);
	}

	void PrimitiveCustomizationMenu(const int sceneId, const nexo::Primitives primitive)
	{
		auto &app = nexo::Application::getInstance();
		auto &sceneManager = app.getSceneManager();

		static int value = primitive == nexo::Primitives::SPHERE ? 1 : 8;
		const int min = primitive == nexo::Primitives::SPHERE ? 0 : 3;
		const int max = primitive == nexo::Primitives::SPHERE ? 8 : 100;

		const char* title = primitive == nexo::Primitives::SPHERE ? "Subdivision" : "Segments";

		ImGui::SliderScalar(title, ImGuiDataType_U32, &value, &min, &max, "%u");

		if (ImGui::Button("Create"))
		{
			const nexo::ecs::Entity newPrimitive = primitive == nexo::Primitives::SPHERE
												 ? nexo::EntityFactory3D::createSphere(
													 {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
													 {0.0f, 0.0f, 0.0f},
													 {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f},
													 value)
												 : nexo::EntityFactory3D::createCylinder(
													 {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
													 {0.0f, 0.0f, 0.0f},
													 {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f},
													 value);
			sceneManager.getScene(sceneId).addEntity(newPrimitive);
			auto createAction = std::make_unique<nexo::editor::EntityCreationAction>(newPrimitive);
			nexo::editor::ActionManager::get().recordAction(std::move(createAction));
		}
		ImGui::EndPopup();
	}

    void PrimitiveSubMenu(const int sceneId)
	{
		auto &app = nexo::Application::getInstance();
		auto &sceneManager = app.getSceneManager();

	    if (ImGui::BeginMenu("Primitives")) {
	        if (ImGui::MenuItem("Cube")) {
	            const nexo::ecs::Entity newCube = nexo::EntityFactory3D::createCube({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
	                                                                                {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
	            sceneManager.getScene(sceneId).addEntity(newCube);
	            auto createAction = std::make_unique<nexo::editor::EntityCreationAction>(newCube);
	            nexo::editor::ActionManager::get().recordAction(std::move(createAction));
	        }
	        if (ImGui::MenuItem("Sphere")) {
	   			// nexo::editor::Editor &editor = nexo::editor::Editor::getInstance();
	   			// const auto primitiveWindow = editor.getWindow<nexo::editor::PrimitiveWindow>(NEXO_WND_USTRID_PRIMITIVE_WINDOW).lock();
	   			// primitiveWindow->setSelectedPrimitive(nexo::Primitives::SPHERE);
				// primitiveWindow->setOpened(true);

	        	// PrimitiveCustomizationMenu(sceneId, nexo::Primitives::SPHERE);
	        }
	        if (ImGui::MenuItem("Cylinder")) {
				// PrimitiveCustomizationMenu(sceneId, nexo::Primitives::CYLINDER);
	        }
	        if (ImGui::MenuItem("Pyramid")) {
	            const nexo::ecs::Entity newPyramid = nexo::EntityFactory3D::createPyramid({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
                                                                       {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
	            sceneManager.getScene(sceneId).addEntity(newPyramid);
	            auto createAction = std::make_unique<nexo::editor::EntityCreationAction>(newPyramid);
	            nexo::editor::ActionManager::get().recordAction(std::move(createAction));
	        }
	        if (ImGui::MenuItem("Tetrahedron")) {
	            const nexo::ecs::Entity newTetrahedron = nexo::EntityFactory3D::createTetrahedron({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
	                                                                                              {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
	            sceneManager.getScene(sceneId).addEntity(newTetrahedron);
	            auto createAction = std::make_unique<nexo::editor::EntityCreationAction>(newTetrahedron);
	            nexo::editor::ActionManager::get().recordAction(std::move(createAction));
	        }
	        ImGui::EndMenu();
	    }
	}

}
