//// ImNexo.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
