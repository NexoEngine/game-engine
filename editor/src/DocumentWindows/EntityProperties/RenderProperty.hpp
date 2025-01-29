/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** RenderProperty.hpp
*/

#pragma once

#include "ADocumentWindow.hpp"

#include "AEntityProperty.hpp"

namespace nexo::editor {
    class RenderProperty : public nexo::editor::AEntityProperty {
        public:
            // using ComponentType = ecs::components::render::render_t;

            explicit RenderProperty(const std::string& name);
            ~RenderProperty() override;

            void update() final;

            static int show(ecs::Entity entity);
            void showEnd() final;

    };
}
