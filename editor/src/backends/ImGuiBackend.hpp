//// ImGuiBackend.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the imgui backend interface class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/Window.hpp"

namespace nexo::editor {
	/**
	* @class ImGuiBackend
	* @brief Static interface class for ImGui backend operations
	*
	* This class provides a unified interface for ImGui operations across different
	* rendering backends. It delegates to the appropriate backend implementation
	* based on the defined graphics API (e.g., OpenGL).
	*/
    class ImGuiBackend {
        public:
			/**
			* @brief Initializes the ImGui backend with the specified window
			*
			* @param[in] window The application window to initialize ImGui with
			* @throws BackendRendererApiNotSupported If the current graphics API is not supported
			*/
            static void init(const std::shared_ptr<renderer::Window>& window);

            /**
             * @brief Shuts down and cleans up the ImGui backend
             *
             * @throws BackendRendererApiNotSupported If the current graphics API is not supported
             */
            static void shutdown();

            /**
             * @brief Initializes the font atlas for ImGui
             *
             * @throws BackendRendererApiNotSupported If the current graphics API is not supported
             */
            static void initFontAtlas();

            /**
             * @brief Begins a new ImGui frame
             *
             * This should be called at the beginning of each frame render cycle
             * before any ImGui UI components are drawn.
             *
             * @throws BackendRendererApiNotSupported If the current graphics API is not supported
             */
            static void begin();

            /**
             * @brief Ends the current ImGui frame and renders it to the window
             *
             * This should be called after all ImGui UI components have been defined
             * for the current frame.
             *
             * @param[in] window The application window to render ImGui to
             * @throws BackendRendererApiNotSupported If the current graphics API is not supported
             */
            static void end(const std::shared_ptr<renderer::Window>& window);

            /**
             * @brief Sets up the error callback for ImGui on the window
             *
             * @param[in] window The application window to set the error callback for
             * @throws BackendRendererApiNotSupported If the current graphics API is not supported
             */
            static void setErrorCallback(const std::shared_ptr<renderer::Window>& window);
    };
}
