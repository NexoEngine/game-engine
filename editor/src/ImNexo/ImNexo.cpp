//// ImNexo.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        01/05/2025
//  Description: Source file for the ImNexo functions
//
///////////////////////////////////////////////////////////////////////////////

#include "ImNexo.hpp"

namespace ImNexo {
    bool isItemActive()
    {
        return g_isItemActive;
    }

    void resetItemActiveState()
    {
        g_isItemActive = false;
    }

    void itemIsActive()
    {
        g_isItemActive = true;
    }

    bool isItemActivated()
    {
        return g_isItemActivated;
    }

    void resetItemActivatedState()
    {
        g_isItemActivated = false;
    }

    void itemIsActivated()
    {
        g_isItemActivated = true;
    }

    bool isItemDeactivated()
    {
        return g_isItemDeactivated;
    }

    void resetItemDeactivatedState()
    {
        g_isItemDeactivated = false;
    }

    void itemIsDeactivated()
    {
        g_isItemDeactivated = true;
    }

    void resetItemStates()
    {
        resetItemActivatedState();
        resetItemActiveState();
        resetItemDeactivatedState();
    }

}
