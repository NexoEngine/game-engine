//// Action.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/04/2025
//  Description: Header file for the action interface
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

namespace nexo::editor {

    /**
    * Base Action interface for all undoable operations
    */
    class Action {
        public:
            virtual ~Action() = default;

            virtual void redo() = 0;

            virtual void undo() = 0;
    };

}
