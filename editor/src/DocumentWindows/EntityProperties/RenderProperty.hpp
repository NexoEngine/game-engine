/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** RenderProperty.hpp
*/

#pragma once

#include "AEntityProperty.hpp"
#include "DocumentWindows/PopupManager.hpp"

namespace nexo::editor {
    class RenderProperty : public nexo::editor::AEntityProperty {
        public:
            explicit RenderProperty(const std::string& name);
            ~RenderProperty() override;

            void update() final;

            static int show(ecs::Entity entity);
            static void createMaterialPopup(ecs::Entity entity);
            void showEnd() final;

            static bool showMaterialInspector;
            static components::Material *selectedMaterial;
            static PopupManager popupManager;
    };
}
