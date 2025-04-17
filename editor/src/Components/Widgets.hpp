//// Widgets.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/02/2025
//  Description: Header file for the widgets components
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <glm/glm.hpp>
#include <string>

#include "components/Camera.hpp"
#include "components/Render3D.hpp"
#include "components/Transform.hpp"
#include "renderer/Texture.hpp"
#include "core/scene/SceneManager.hpp"

namespace nexo::editor {

	/**
     * @brief A collection of custom ImGui widget drawing functions.
     *
     * Provides utility functions for drawing color editors, texture buttons, and a material inspector,
     * which can be used to simplify UI code for rendering material properties.
     */
	class Widgets {
		public:

			/**
             * @brief Draws a color editor with a button and an optional inline color picker.
             *
             * Displays a custom color button (with a cog icon for picker settings) and, if enabled,
             * an inline color picker. The function returns true if the color was modified.
             *
             * @param label A unique label identifier for the widget.
             * @param selectedEntityColor Pointer to the glm::vec4 representing the current color.
             * @param colorPickerMode Pointer to the ImGuiColorEditFlags for the picker mode.
             * @param showPicker Pointer to a boolean that determines if the inline color picker is visible.
             * @param colorButtonFlags Optional flags for the color button (default is none).
             * @return true if the color was modified; false otherwise.
             */
			static bool drawColorEditor(
				const std::string &label,
				glm::vec4 *selectedEntityColor,
				ImGuiColorEditFlags *colorPickerMode,
				bool *showPicker,
				ImGuiColorEditFlags colorButtonFlags = ImGuiColorEditFlags_None);

			/**
             * @brief Draws a texture button that displays a texture preview.
             *
             * When clicked, opens a file dialog to select a new texture. If a new texture is loaded,
             * the passed texture pointer is updated and the function returns true.
             *
             * @param label A unique label identifier for the button.
             * @param texture A shared pointer to the renderer::Texture2D that holds the texture.
             * @return true if the texture was modified; false otherwise.
             */
			static bool drawTextureButton(const std::string &label, std::shared_ptr<renderer::Texture2D> &texture);

			/**
             * @brief Draws a material inspector widget for editing material properties.
             *
             * This function displays controls for shader selection, rendering mode, and textures/colors
             * for material properties such as albedo and specular components.
             *
             * @param material Pointer to the components::Material to be inspected and modified.
             * @return true if any material property was modified; false otherwise.
             */
			static bool drawMaterialInspector(components::Material *material);

			static void drawTransformProperties(components::TransformComponent &transformComponent, glm::vec3 &lastDisplayedEuler);
			static void drawCameraProperties(components::CameraComponent &cameraComponent);

			static bool drawCameraCreator(const scene::SceneId sceneId, ImVec2 sceneViewportSize);
	};
}
