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
//  Description: Header for the nexo exceptions abstract class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <source_location>

#include "Exception.hpp"

namespace nexo::core {
    class FileNotFoundException final : public Exception {
        public:
            explicit FileNotFoundException(const std::string &filePath,
                                           const std::source_location loc = std::source_location::current())
                : Exception("File not found: " + filePath, loc) {}
    };

    class LoadModelException final : public Exception {
        public:
            explicit LoadModelException(const std::string &filePath, const std::string &errorStr,
                                        const std::source_location loc = std::source_location::current()) : Exception(
                "Failure to load model : " + filePath + " : " + errorStr, loc) {};
    };

    class SceneManagerLifecycleException : public Exception {
    	public:
     		explicit SceneManagerLifecycleException(const std::string &message,
                                           const std::source_location loc = std::source_location::current())
                : Exception(message, loc) {}
    };
}
