//// SaveFolder.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Definition of engine folder wrapper.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <initializer_list>
#include <memory>
#include <variant>
#include <concepts>
#include <format>

#include "SaveExceptions.hpp"
#include "SaveFile.hpp"
#include "ISaveBase.hpp"

namespace engine::save {

    class SaveFolder : public ISaveBase {
    private:
        class InputSaveElement {
        public:
            // Constructors for SaveFiles and SaveFolders (derived classes)
            template <typename T, typename = std::enable_if_t<std::is_base_of_v<SaveFile, T> || std::is_base_of_v<SaveFolder, T>>>
            InputSaveElement(std::string name, const T& element)
                : m_element(std::make_shared<T>(element)), m_name(std::move(name)) {}

            template <typename T, typename = std::enable_if_t<std::is_base_of_v<SaveFile, T> || std::is_base_of_v<SaveFolder, T>>>
            InputSaveElement(std::string name, std::shared_ptr<T> element)
                : m_element(std::move(element)), m_name(std::move(name)) {}


            // Constructors for SaveFolders
            InputSaveElement(std::string name, std::shared_ptr<SaveFolder> folder)
                : m_element(folder), m_name(std::move(name)) {}
            InputSaveElement(std::string name, const SaveFolder& folder)
                : m_element(std::make_shared<SaveFolder>(folder)), m_name(std::move(name)) {}

            [[nodiscard]] bool isFile() const {
                return std::holds_alternative<std::shared_ptr<SaveFile>>(m_element);
            }

            [[nodiscard]] bool isFolder() const {
                return std::holds_alternative<std::shared_ptr<SaveFolder>>(m_element);
            }

            [[nodiscard]] std::shared_ptr<SaveFile> getFile() const {
                return std::get<std::shared_ptr<SaveFile>>(m_element);
            }

            [[nodiscard]] std::shared_ptr<SaveFolder> getFolder() const {
                return std::get<std::shared_ptr<SaveFolder>>(m_element);
            }

            [[nodiscard]] const std::string& getName() const {
                return m_name;
            }

        private:
            std::variant<std::shared_ptr<SaveFile>, std::shared_ptr<SaveFolder>> m_element;
            std::string m_name;
        };
    public:
        virtual ~SaveFolder() = default;

        SaveFolder() = default;

        explicit SaveFolder(std::filesystem::path folderPath) : m_folderPath(std::move(folderPath)) {}

        // Constructor for adding files and folders directly
        SaveFolder(const std::initializer_list<InputSaveElement> elements)
        {
            _registerSaveElements(elements);
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SaveFile, T>>>
        void addFile(const std::string& name, std::shared_ptr<T> file) {
            file->setPath(m_folderPath / name);
            m_saves[name] = file;
			m_orderedSaves.push_back(file);
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SaveFile, T>>>
        void addFile(const std::string& name, const T& file) {
            auto filePtr = std::make_shared<T>(file);
			addFile(name, filePtr);
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SaveFolder, T>>>
        void addFolder(const std::string& name, std::shared_ptr<T> folder) {
            folder->setPath(m_folderPath / name);
            m_saves[name] = folder;
			m_orderedSaves.push_back(folder);
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SaveFolder, T>>>
        void addFolder(const std::string& name, const T& folder) {
            auto folderPtr = std::make_shared<T>(folder);
			addFolder(name, folderPtr);
        }

        /**
         * @brief Recursively saves the folder and all its contents
         * @warning Be careful of infinite recursion if there are circular references
         */
		virtual void save() override
        {
            _createFolder();

			for (const auto& [_, file] : m_saves) {
				file->save();
			}
        }

        virtual void load() override
        {
			for (const auto& [_, file] : m_saves) {
				file->load();
			}
        }

        void setPath(std::filesystem::path path) override {
            m_folderPath = std::move(path);
            _updatePaths();
        }

		void resetPath() override {
			m_folderPath.clear();
			for (const auto& [_, save] : m_saves) {
				save->resetPath();
			}
		}

        [[nodiscard]] const std::filesystem::path& getPath() const {
            return m_folderPath;
        }

    private:
        void _registerSaveElements(const std::initializer_list<InputSaveElement>& elements) {
            for (const auto& element : elements) {
                const auto name = element.getName();
                if (element.isFile()) {
                    addFile(name, element.getFile());
                }
                else {
                    addFolder(name, element.getFolder());
                }
            }
            if (!m_folderPath.empty())
                _updatePaths();
        }

        void _updatePaths() {
			for (auto& [name, save] : m_saves) {
				save->setPath(m_folderPath / name);
			}
        }

        void _createFolder() const {
            if (m_folderPath == std::filesystem::current_path()) {
                return;
            }
            std::error_code ec;
            std::filesystem::create_directories(m_folderPath, ec);
            if (ec) {
                throw SaveFolderException(std::format("Failed to create directories '{}': {}",
                    m_folderPath.string(), ec.message()));
            }
        }

		std::unordered_map<std::string, std::shared_ptr<ISaveBase>> m_saves;
		std::vector<std::shared_ptr<ISaveBase>> m_orderedSaves;
        std::filesystem::path m_folderPath;
    };

} // namespace engine::save
