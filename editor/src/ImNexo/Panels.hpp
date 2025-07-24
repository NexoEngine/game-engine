//// NxUiPanels.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Header file for ui panels
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "components/Render.hpp"
#include "core/scene/SceneManager.hpp"

#include <imgui.h>

namespace ImNexo {
    /**
    * @brief Draws a material inspector widget for editing material properties.
    *
    * This function displays controls for shader selection, rendering mode, and textures/colors
    * for material properties such as albedo and specular components.
    *
    * @param material Reference to the components::Material to be inspected and modified.
    * @return true if any material property was modified; false otherwise.
    */
    bool MaterialInspector(nexo::components::Material &material);

    /**
     * @brief Displays a camera creation and configuration dialog.
     *
     * Creates a modal window with a split layout:
     * - Left panel: Camera property inspector with fields for name, camera parameters,
     *   transform values, and optional components
     * - Right panel: Real-time preview of the camera's view
     *
     * The dialog includes an animated "Add Component" dropdown that allows adding
     * optional camera components (Camera Target or Camera Controller). At the bottom,
     * OK and Cancel buttons allow confirming or aborting camera creation.
     *
     * When OK is clicked, the camera name is validated. If valid, the camera is added
     * to the specified scene with the configured parameters. If Cancel is clicked or
     * the dialog is otherwise closed, any temporary camera is deleted.
     *
     * @param sceneId The ID of the scene where the camera will be created
     * @param sceneViewportSize The size of the scene viewport for proper camera aspect ratio
     * @return true if the dialog was closed (either by confirming or canceling), false if still open
     */
    bool CameraInspector(nexo::scene::SceneId sceneId);
}
