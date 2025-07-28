//// Nexo.cpp /////////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
        app.init();
        LOG(NEXO_INFO, "Nexo engine initialized");
        return app;
    }

    Application &getApp()
    {
        return Application::getInstance();
    }

    void runEngine(const Application::SceneInfo &sceneInfo)
    {
        Application &app = Application::getInstance();
        app.run(sceneInfo);
    }

}
