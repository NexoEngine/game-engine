//// ImNexo.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        01/05/2025
//  Description: Header for the ImNexo functions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

namespace ImNexo {

    inline bool g_isItemActive = false;
    inline bool g_isItemActivated = false;
    inline bool g_isItemDeactivated = false;

    bool isItemActive();
    void setItemActive();

    bool isItemActivated();
    void setItemActivated();

    bool isItemDeactivated();
    void setItemDeactivated();

    void resetItemStates();

}
