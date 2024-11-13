//// Nexo.cpp /////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Main source file for nexo
//
///////////////////////////////////////////////////////////////////////////////
#include "Nexo.hpp"

namespace nexo {

    Application &init()
    {
        Application &app = Application::getInstance();
        LOG(NEXO_INFO, "Nexo engine initialized");
        return app;
    }

    Application &getApp()
    {
        return Application::getInstance();
    }

    void runEngine(bool renderToFrameBuffer)
    {
        Application &app = Application::getInstance();
        app.run(renderToFrameBuffer);
    }

}