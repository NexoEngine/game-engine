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
	class InspectorWindow;
}

namespace nexo::editor {

    class IEntityProperty {
        public:
            /**
 * @brief Virtual destructor to ensure proper cleanup of derived IEntityProperty instances.
 */
virtual ~IEntityProperty() = default;

            virtual bool show(ecs::Entity entity) = 0;
    };

    class AEntityProperty : public IEntityProperty {
    	public:
   			/**
 * @brief Constructs an AEntityProperty instance.
 *
 * Initializes the AEntityProperty with a reference to the InspectorWindow used to display entity details.
 *
 * @param inspector Reference to the InspectorWindow associated with this property.
 */
explicit AEntityProperty(InspectorWindow &inspector) : m_inspector(inspector) {};
      	protected:
       		InspectorWindow &m_inspector;
    };
};
