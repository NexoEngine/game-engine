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

#include "ADocumentWindow.hpp"
#include "Exception.hpp"
#include "IDocumentWindow.hpp"
#include "exceptions/Exceptions.hpp"
#define L_DEBUG 1
#include <loguru/loguru.hpp>
#include <memory>
#include <type_traits>

#include "WindowRegistry.hpp"

namespace nexo::editor {


    class Editor {
        public:
            /**
 * @brief Default constructor for the Editor class.
 *
 * Creates an instance of Editor with default member initialization.
 */
Editor() = default;
            /**
 * @brief Default destructor for the Editor class.
 *
 * Automatically handles cleanup when an Editor instance is destroyed.
 */
~Editor() = default;

            /**
             * @brief Initializes the engine, setting up necessary components and systems.
             */
            void init() const;

            [[nodiscard]] bool isOpen() const;


            void update() const;
            void render();
            void shutdown() const;

            template<typename T>
            requires std::derived_from<T, IDocumentWindow>
            /**
             * @brief Registers a new window of type T.
             *
             * This templated function creates an instance of window type T using the editor's window registry and registers it with the registry.
             * T must be derived from IDocumentWindow.
             *
             * @tparam T The concrete window type to register, which must inherit from IDocumentWindow.
             */
            void registerWindow()
            {
            	auto window = std::make_shared<T>(m_windowRegistry);
             	m_windowRegistry.registerWindow<T>(window);
            }

            template<typename T>
            /**
             * @brief Retrieves a registered window of type T.
             *
             * This template method returns a shared pointer to a window instance of type T from the editor's window registry.
             * The window type T must be derived from IDocumentWindow. If no window of the specified type is registered, an empty
             * shared pointer is returned.
             *
             * @tparam T The type of the window to retrieve. Must derive from IDocumentWindow.
             * @return std::shared_ptr<T> A shared pointer to the registered window of type T, or an empty pointer if none exists.
             */
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
