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
 * @brief Default constructs an Editor instance.
 *
 * This constructor uses the compiler-generated default initialization for all members.
 */
Editor() = default;
            /**
 * @brief Default destructor for the Editor class.
 *
 * This destructor is defaulted, relying on the compiler-generated cleanup of resources.
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
             * @brief Registers a new document window of type T.
             *
             * This templated method creates a shared pointer instance of T, passing the current window registry
             * as a constructor argument, and subsequently registers the window with the registry. The template
             * parameter T must derive from IDocumentWindow.
             *
             * @tparam T The type of document window to create and register.
             */
            void registerWindow()
            {
            	auto window = std::make_shared<T>(m_windowRegistry);
             	m_windowRegistry.registerWindow<T>(window);
            }

            template<typename T>
            /**
             * @brief Retrieves a registered document window instance.
             *
             * This templated method accesses the window registry to fetch a document window of the specified type.
             * The window must have been previously registered via the corresponding registration method.
             *
             * @tparam T The document window type to retrieve (must derive from IDocumentWindow).
             * @return std::shared_ptr<T> A shared pointer to the document window instance, or nullptr if no such window is registered.
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
