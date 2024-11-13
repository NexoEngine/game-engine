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

namespace nexo::editor {

    class FileNotFoundException final : public Exception {
        public:
            explicit FileNotFoundException(const std::string &filePath, const char *file = __FILE__,
                                           const int line = __LINE__)
                : Exception("File not found: " + filePath, file, line) {}
    };

    class BackendRendererApiNotSupported final : public Exception {
        public:
            explicit BackendRendererApiNotSupported(const std::string &backendApiName, const char *file = __FILE__,
                                                    const int line = __LINE__)
                : Exception("Backend render API not supported: " + backendApiName, file, line) {}
    };

    class BackendRendererApiInitFailed final : public Exception {
        public:
            explicit BackendRendererApiInitFailed(const std::string &backendApiName, const char *file = __FILE__,
                                                  const int line = __LINE__)
                : Exception("Backend render API init failed: " + backendApiName, file, line) {}
    };

    class BackendRendererApiFontInitFailed final : public Exception {
        public:
            explicit BackendRendererApiFontInitFailed(const std::string &backendApiName, const char *file = __FILE__,
                                                      const int line = __LINE__)
                : Exception("Backend render API font init failed: " + backendApiName, file, line) {}
    };

    class BackendRendererApiFatalFailure final : public Exception {
        public:
            explicit BackendRendererApiFatalFailure(const std::string &backendApiName, const std::string &message,
                                                    const char *file = __FILE__,
                                                    const int line = __LINE__)
                : Exception("[" + backendApiName + " FATAL ERROR]" + message, file, line) {}
    };

}
