//// SaveExceptions.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        09/11/2024
//  Description: Exceptions for the SaveFile and SaveFolder classes
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdexcept>

namespace engine::save {

    class SaveFileException : public std::runtime_error {
    public:
        explicit SaveFileException(const std::string& message) : std::runtime_error(message) {}
    };

    class SaveFolderException : public std::runtime_error {
    public:
        explicit SaveFolderException(const std::string& message) : std::runtime_error(message) {}
    };

    class SaveFileNotRegisteredException : public SaveFileException {
    public:
        explicit SaveFileNotRegisteredException(const std::string& message) : SaveFileException(message) {}
    };

    class SaveFileAlreadyRegisteredException : public SaveFileException {
    public:
        explicit SaveFileAlreadyRegisteredException(const std::string& message) : SaveFileException(message) {}
    };

} // namespace engine::save
