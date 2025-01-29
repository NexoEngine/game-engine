/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** TransformProperty.hpp
*/

#pragma once

#include "ADocumentWindow.hpp"
#include "AEntityProperty.hpp"

namespace nexo::editor {
    class TransformProperty : public nexo::editor::AEntityProperty {
        public:
            // using ComponentType = transform_t;

            explicit TransformProperty(const std::string& name);
            ~TransformProperty();

            void update();

            static int show(ecs::Entity entity);
            void showEnd();

    };
}
