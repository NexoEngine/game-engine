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

#include "Exception.hpp"

namespace nexo::core {
    class FileNotFoundException final : public Exception {
        public:
            explicit FileNotFoundException(const std::string &filePath, const char *file = __FILE__,
                                           const int line = __LINE__)
                : Exception("File not found: " + filePath, file, line) {}
    };

    class LoadModelException final : public Exception {
        public:
            explicit LoadModelException(const std::string &filePath, const std::string &errorStr,
                                        const char *file = __FILE__, const int line = __LINE__) : Exception(
                "Failure to load model : " + filePath + " : " + errorStr, file, line) {};
    };


}
