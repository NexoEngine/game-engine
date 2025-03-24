//// Exceptions.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the editor exceptions
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Exception.hpp"

#include <source_location>
#include <typeindex>
#include <format>

namespace nexo::editor {

    class FileNotFoundException final : public Exception {
        public:
            /**
                 * @brief Constructs a FileNotFoundException.
                 *
                 * This exception is used to indicate that a file could not be located. It creates an error message by prefixing
                 * the file path with "File not found: ".
                 *
                 * @param filePath The path of the file that was not found.
                 * @param loc The source location where the exception is instantiated (defaults to the current source location).
                 */
                explicit FileNotFoundException(const std::string &filePath,
                                           const std::source_location loc = std::source_location::current())
                : Exception("File not found: " + filePath, loc) {}
    };

    class WindowNotRegistered final : public Exception {
    	public:
     		/**
                 * @brief Constructs a WindowNotRegistered exception.
                 *
                 * This exception is thrown when an attempt is made to access a window that has not been registered in the WindowRegistry.
                 * The error message is generated using the provided window type index, which identifies the unregistered window.
                 *
                 * @param windowTypeIndex The type index of the unregistered window.
                 * @param loc Optional source location for debugging purposes, defaulting to the current location.
                 */
                explicit WindowNotRegistered(std::type_index windowTypeIndex, const std::source_location loc = std::source_location::current())
                : Exception(std::format("Window not registered: {}. Make sure the window is registered in the WindowRegistry before accessing it.", windowTypeIndex.name()), loc) {}
    };

    class BackendRendererApiNotSupported final : public Exception {
        public:
            /**
                 * @brief Constructs an exception indicating an unsupported backend render API.
                 *
                 * This exception is thrown when the provided backend render API is not supported.
                 * It formats an error message including the name of the unsupported API.
                 *
                 * @param backendApiName The name of the backend render API that is not supported.
                 * @param loc The source location where the exception was thrown (defaults to the current location).
                 */
                explicit BackendRendererApiNotSupported(const std::string &backendApiName,
                                                    const std::source_location loc = std::source_location::current())
                : Exception("Backend render API not supported: " + backendApiName, loc) {}
    };

    class BackendRendererApiInitFailed final : public Exception {
        public:
            explicit BackendRendererApiInitFailed(const std::string &backendApiName,
                                                  const std::source_location loc = std::source_location::current())
                : Exception("Backend render API init failed: " + backendApiName, loc) {}
    };

    class BackendRendererApiFontInitFailed final : public Exception {
        public:
            explicit BackendRendererApiFontInitFailed(const std::string &backendApiName,
                                                      const std::source_location loc = std::source_location::current())
                : Exception("Backend render API font init failed: " + backendApiName, loc) {}
    };

    class BackendRendererApiFatalFailure final : public Exception {
        public:
            explicit BackendRendererApiFatalFailure(const std::string &backendApiName, const std::string &message,
                                                    const std::source_location loc = std::source_location::current())
                : Exception("[" + backendApiName + " FATAL ERROR]" + message, loc) {}
    };

}
