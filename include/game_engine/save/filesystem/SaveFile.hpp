//// SaveFile.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Definition of engine file wrapper.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <format>
#include <cerrno>

#include "SaveExceptions.hpp"
#include "ISaveBase.hpp"

namespace engine::save {

    class SaveFolder;

    class SaveFile : public ISaveBase {
    public:
        virtual ~SaveFile() = default;

        void save() override {
            if (m_filePath.empty()) {
                throw SaveFileException("SaveFile has no path set."
                    " You could set it manually or use a SaveFolder "
                    "to set it automatically.");
            }
            std::ofstream outFile(m_filePath);
            _saveImpl(outFile);
            outFile.close();
            if (outFile.fail()) {
                char buffer[256];
            #ifdef _WIN32
                strerror_s(buffer, sizeof(buffer), errno);
            #else
                strerror_r(errno, buffer, sizeof(buffer));
            #endif
                throw SaveFileException(std::format("Failed to save file '{}': {}", m_filePath.string(), buffer));
            }
        }

        void load() override {
            if (m_filePath.empty()) {
                throw SaveFileException("SaveFile has no path set."
                    " You could set it manually or use a SaveFolder "
                    "to set it automatically.");
            }
            std::ifstream inFile(m_filePath);
            _loadImpl(inFile);
            inFile.close();
        }

        [[nodiscard]] const std::filesystem::path& getPath() const {
            return m_filePath;
        }

        void setPath(std::filesystem::path path) override {
            this->m_filePath = std::move(path);
        }

        void resetPath() override {
            m_filePath.clear();
        }

    protected:
        void _setSaveOpenMode(std::ios::openmode mode) {
            m_saveOpenMode = mode;
        }

        void _setLoadOpenMode(std::ios::openmode mode)
        {
            m_loadOpenMode = mode;
        }

        virtual void _saveImpl(std::ofstream& outFile) const = 0;
        virtual void _loadImpl(std::ifstream& inFile) = 0;
    private:
        std::filesystem::path m_filePath;
        std::ios::openmode m_saveOpenMode = std::ios::out;
        std::ios::openmode m_loadOpenMode = std::ios::in;
    };

    /**
     * @brief A SaveFile that writes nothing to the file. But still creates it.
     */
    class EmptySaveFile : public SaveFile {
    public:
        EmptySaveFile() = default;

    private:
        void _saveImpl(std::ofstream& outFile) const override
        {
            // Do nothing
        }

        void _loadImpl(std::ifstream& inFile) override
        {
            // Do nothing
        }
    };

    class PlaceholderSaveFile : public SaveFile {
    public:
        explicit PlaceholderSaveFile(std::string content) : m_content(std::move(content)) {}

    private:
        void _saveImpl(std::ofstream& outFile) const override {
            outFile << m_content;
        }

        void _loadImpl(std::ifstream& inFile) override {
            // Do nothing
        }

        std::string m_content;
    };

} // namespace engine::save
