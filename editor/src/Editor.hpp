//// Editor.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/11/2024
//  Description: Header file for the main editor class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define L_DEBUG 1
#include <loguru/loguru.hpp>
#include <memory>

#include "WindowRegistry.hpp"

namespace nexo::editor {


    class Editor {
        public:
            Editor();
            ~Editor() = default;

            /**
             * @brief Initializes the engine, setting up necessary components and systems.
             */
            void init();

            [[nodiscard]] bool isOpen() const;


            void update();
            void render();
            void shutdown();

            template<typename T>
            void registerWindow()
            {
            	auto window = std::make_shared<T>(m_windowRegistry);
             	m_windowRegistry.registerWindow<T>(window);
            }

            template<typename T>
            std::shared_ptr<T> getWindow()
            {
            	return m_windowRegistry.getWindow<T>();
            }
        private:
            void setupEngine() const;
            void setupStyle() const;
            void setupFonts(float scaleFactorX, float scaleFactorY) const;
            void buildDockspace();
            void drawMenuBar();

            bool m_quit = false;
            bool m_showDemoWindow = false;
            WindowRegistry m_windowRegistry;
    };
}
