//// InspectorWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie Giacomel
//  Date:        23/11/2024
//  Description: Header file file for AEntityProperty
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"

#include <imgui.h>

namespace nexo::editor {
    class IEntityProperty {
        public:
            virtual ~IEntityProperty() = default;

            virtual void update() = 0;
            virtual void showEnd() = 0;
    };

    class AEntityProperty : public IEntityProperty {
        public:
            ~AEntityProperty() override;

            virtual void update() override;
            virtual void showEnd() override;
            static int show(ecs::Entity entity);

        protected:
            explicit AEntityProperty(const std::string& name);

            std::string _name;

    };

};