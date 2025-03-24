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
            explicit FileNotFoundException(const std::string &filePath,
                                           const std::source_location loc = std::source_location::current())
                : Exception("File not found: " + filePath, loc) {}
    };

    class WindowNotRegistered final : public Exception {
    	public:
     		explicit WindowNotRegistered(std::type_index windowTypeIndex, const std::source_location loc = std::source_location::current())
                : Exception(std::format("Window not registered: {}. Make sure the window is registered in the WindowRegistry before accessing it.", windowTypeIndex.name()), loc) {}
    };

    class BackendRendererApiNotSupported final : public Exception {
        public:
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
