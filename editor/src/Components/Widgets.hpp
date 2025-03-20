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

#include "components/Render3D.hpp"
#include "renderer/Texture.hpp"

namespace nexo::editor {
	class Widgets {
		public:
			static bool drawColorEditor(
				const std::string &label,
				glm::vec4 *selectedEntityColor,
				ImGuiColorEditFlags *colorPickerMode,
				bool *showPicker,
				ImGuiColorEditFlags colorButtonFlags = ImGuiColorEditFlags_None);
			static bool drawTextureButton(const std::string &label, std::shared_ptr<renderer::Texture2D> &texture);
			static bool drawMaterialInspector(components::Material *material);
	};
}
