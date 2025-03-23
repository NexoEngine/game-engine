//// AEntityProperty.hpp //////////////////////////////////////////////////////
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

namespace nexo::editor {

	class InterfaceEntityProperty {
		public:
			virtual ~InterfaceEntityProperty() = default;
			virtual int show(ecs::Entity selectedEntity) = 0;
	};

    class IEntityProperty {
        public:
            virtual ~IEntityProperty() = default;

            virtual void update() = 0;
            virtual void showEnd() = 0;
    };

    class AEntityProperty : public IEntityProperty {
        public:
            ~AEntityProperty() override;

            void update() override;
            void showEnd() override;
            static int show(ecs::Entity entity);

        protected:
            explicit AEntityProperty(std::string  name);

            std::string _name;

    };

};
