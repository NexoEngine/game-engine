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

    class IEntityProperty {
       public:
        virtual ~IEntityProperty() = default;

        /**
         * @brief Displays the properties of the specified entity in the inspector window.
         *
         * This pure virtual function must be implemented by derived classes to define how
         * the properties of the given entity are presented in the inspector window.
         *
         * @param entity The entity whose properties are to be displayed.
         */
        virtual void show(ecs::Entity entity) = 0;
    };

    class AEntityProperty : public IEntityProperty {
       public:
        /**
         * @brief Constructs an AEntityProperty instance.
         *
         * Initializes the entity property by storing a reference to the provided InspectorWindow,
         * which is used for displaying and managing entity properties in the editor.
         *
         * @param inspector Reference to the InspectorWindow associated with this property.
         */
        explicit AEntityProperty(InspectorWindow &inspector) : m_inspector(inspector){};

       protected:
        InspectorWindow &m_inspector;
    };
}; // namespace nexo::editor
