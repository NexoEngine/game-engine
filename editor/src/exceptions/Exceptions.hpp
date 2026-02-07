//// Exceptions.hpp ///////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the editor exceptions
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Exception.hpp"
#include "ecs/Entity.hpp" // to not remove

#include <format>
#include <source_location>
#include <typeindex>

namespace nexo::editor {

    class FileNotFoundException final : public Exception {
       public:
        /**
         * @brief Constructs a FileNotFoundException.
         *
         * Initializes the exception with an error message indicating that the specified file was not found.
         * The error message is automatically formatted to include the given file path.
         *
         * @param filePath The path of the file that could not be located.
         * @param loc The source location where the exception is thrown (defaults to the current location).
         */
        explicit FileNotFoundException(const std::string &filePath,
                                       const std::source_location &loc = std::source_location::current())
            : Exception("File not found: " + filePath, loc)
        {}
    };

    /**
     * @brief Exception class for file read errors.
     *
     * This exception is thrown when an error occurs while reading a file.
     * It includes the file path and a specific error message to provide context about the failure.
     */
    class FileReadException final : public Exception {
       public:
        explicit FileReadException(const std::string &filePath, const std::string &message,
                                   const std::source_location &loc = std::source_location::current())
            : Exception(std::format("Error reading file {}: {}", filePath, message), loc)
        {}
    };

    /**
     * @brief Exception class for file write errors.
     *
     * This exception is thrown when an error occurs while writing to a file.
     * It includes the file path and a specific error message to provide context about the failure.
     */
    class FileWriteException final : public Exception {
       public:
        explicit FileWriteException(const std::string &filePath, const std::string &message,
                                    const std::source_location &loc = std::source_location::current())
            : Exception(std::format("Error writing to file {}: {}", filePath, message), loc)
        {}
    };

    class WindowNotRegistered final : public Exception {
       public:
        /**
         * @brief Constructs a WindowNotRegistered exception.
         *
         * Initializes an exception indicating that the specified window type is not registered in the WindowRegistry.
         *
         * @param windowTypeIndex The type index of the unregistered window.
         * @param loc The source location where the exception is thrown (defaults to the current location).
         */
        explicit WindowNotRegistered(const std::type_index windowTypeIndex,
                                     const std::source_location &loc = std::source_location::current())
            : Exception(std::format("Window not registered: {}. Make sure the window is registered in the "
                                    "WindowRegistry before accessing it.",
                                    windowTypeIndex.name()),
                        loc)
        {}
    };

    class WindowAlreadyRegistered final : public Exception {
       public:
        /**
         * @brief Constructs a WindowAlreadyRegistered exception.
         *
         * Initializes an exception indicating that the specified window type with the specified name is already
         * registered in the WindowRegistry.
         *
         * @param windowTypeIndex The type index of the unregistered window.
         * @param windowName The name of the window.
         * @param loc The source location where the exception is thrown (defaults to the current location).
         */
        explicit WindowAlreadyRegistered(const std::type_index windowTypeIndex, const std::string &windowName,
                                         const std::source_location &loc = std::source_location::current())
            : Exception(std::format("Window {} already registered as: {}. Make sure the type and name is unique.",
                                    windowName, windowTypeIndex.name()),
                        loc)
        {}
    };

    class BackendRendererApiNotSupported final : public Exception {
       public:
        /**
         * @brief Constructs a BackendRendererApiNotSupported exception.
         *
         * This exception is thrown when a backend render API is not supported.
         * The error message is generated by appending the given backend API name to a predefined message.
         *
         * @param backendApiName The name of the unsupported backend render API.
         * @param loc The source location where the exception is thrown, defaulting to the current location.
         */
        explicit BackendRendererApiNotSupported(const std::string &backendApiName,
                                                const std::source_location &loc = std::source_location::current())
            : Exception("Backend render API not supported: " + backendApiName, loc)
        {}
    };

    /**
     * @brief Exception class for backend renderer API initialization failures.
     *
     * This exception is thrown when the initialization of a backend renderer API fails.
     * It includes the name of the backend API to provide context about the failure.
     */
    class BackendRendererApiInitFailed final : public Exception {
       public:
        explicit BackendRendererApiInitFailed(const std::string &backendApiName,
                                              const std::source_location &loc = std::source_location::current())
            : Exception("Backend render API init failed: " + backendApiName, loc)
        {}
    };

    /**
     * @brief Exception class for backend renderer API font initialization failures.
     *
     * This exception is thrown when the font initialization of a backend renderer API fails.
     * It includes the name of the backend API to provide context about the failure.
     */
    class BackendRendererApiFontInitFailed final : public Exception {
       public:
        explicit BackendRendererApiFontInitFailed(const std::string &backendApiName,
                                                  const std::source_location &loc = std::source_location::current())
            : Exception("Backend render API font init failed: " + backendApiName, loc)
        {}
    };

    /**
     * @brief Exception class for fatal errors in backend renderer APIs.
     *
     * This exception is thrown when a fatal error occurs in a backend renderer API.
     * It includes the name of the backend API and a specific error message to provide context about the failure.
     */
    class BackendRendererApiFatalFailure final : public Exception {
       public:
        explicit BackendRendererApiFatalFailure(const std::string &backendApiName, const std::string &message,
                                                const std::source_location &loc = std::source_location::current())
            : Exception("[" + backendApiName + " FATAL ERROR]" + message, loc)
        {}
    };

    /**
     * @brief Exception class for invalid test file formats.
     *
     * This exception is thrown when a test file has an invalid format.
     * It includes the file path and a specific error message to provide context about the failure.
     */
    class InvalidTestFileFormat final : public Exception {
       public:
        explicit InvalidTestFileFormat(const std::string &filePath, const std::string &message,
                                       const std::source_location &loc = std::source_location::current())
            : Exception(std::format("Invalid test file protocol format {}: {}", filePath, message), loc)
        {}
    };

    /**
     * @brief Exception class for empty log file paths.
     *
     * This exception is thrown when a log file path is found to be empty.
     * It provides a default error message indicating the issue.
     */
    class LogFilePathEmptyException final : public Exception {
       public:
        explicit LogFilePathEmptyException(const std::source_location &loc = std::source_location::current())
            : Exception("Log file path is empty.", loc)
        {}
    };

    /**
     * @brief Exception class for empty resolved log file paths.
     *
     * This exception is thrown when a resolved log file path is found to be empty.
     * It provides a default error message indicating the issue.
     */
    class LogResolvedPathEmptyException final : public Exception {
       public:
        explicit LogResolvedPathEmptyException(const std::source_location &loc = std::source_location::current())
            : Exception("Resolved log file path is empty.", loc)
        {}
    };

    /**
     * @brief Exception class for asset import errors.
     *
     * This exception is thrown when an error occurs during the import of an asset.
     * It includes the asset path and a specific error message to provide context about the failure.
     */
    class AssetImportException final : public Exception {
       public:
        explicit AssetImportException(const std::string &assetPath, const std::string &message,
                                      const std::source_location &loc = std::source_location::current())
            : Exception(std::format("Exception while importing {}: {}", assetPath, message), loc)
        {}
    };

    /**
     * @brief Exception class for unsupported entity shape types.
     *
     * This exception is thrown when an unsupported shape type is used for entity creation.
     * It provides a default error message indicating the issue.
     */
    class UnsupportedEntityShapeType final : public Exception {
       public:
        explicit UnsupportedEntityShapeType(const std::source_location &loc = std::source_location::current())
            : Exception("Unsupported shape type for entity creation.", loc)
        {}
    };

    /**
     * @brief Exception class for invalid physics body IDs.
     *
     * This exception is thrown when an invalid body ID is encountered during physics body creation.
     * It provides a default error message indicating the issue.
     */
    class InvalidBodyId final : public Exception {
       public:
        explicit InvalidBodyId(const ecs::Entity entity,
                               const std::source_location &loc = std::source_location::current())
            : Exception(std::format("Failed to create physics body for entity {}", entity), loc)
        {}
    };

    /**
     * @brief Exception class for physics body creation errors.
     *
     * This exception is thrown when an error occurs during the creation of a physics body for an entity.
     * It includes the entity ID and a specific error message to provide context about the failure.
     */
    class PhysicBodyCreationException final : public Exception {
       public:
        explicit PhysicBodyCreationException(const ecs::Entity entity, const std::string &message,
                                             const std::source_location &loc = std::source_location::current())
            : Exception(std::format("Exception during physics body recreation for entity {}: {}", entity, message), loc)
        {}
    };

    /**
     * @brief Exception class for physics component creation errors.
     *
     * This exception is thrown when an error occurs during the creation of a physics component for an entity.
     * It includes the entity ID and a specific error message to provide context about the failure.
     */
    class PhysicComponentCreationException final : public Exception {
       public:
        explicit PhysicComponentCreationException(const ecs::Entity entity, const std::string &message,
                                                  const std::source_location &loc = std::source_location::current())
            : Exception(std::format("Exception during physics component creation for entity {}: {}", entity, message),
                        loc)
        {}
    };

} // namespace nexo::editor
