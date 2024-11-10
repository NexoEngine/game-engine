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
        std::cout << "ca passe la" << std::endl;
        app.init();
        std::cout << "mais pas la" << std::endl;
        app.registerAllDebugListeners();
        std::cout << "ah si en fait" << std::endl;
        return app;
    }

    void run()
    {
        Application &app = Application::getInstance();
        app.run();
    }

}