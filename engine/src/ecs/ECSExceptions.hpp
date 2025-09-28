//// ECSExceptions.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        12/11/2024
//  Description: Header file for the ecs exceptions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Definitions.hpp"
#include "Exception.hpp"

#include <format>
#include <source_location>

namespace nexo::ecs {

    class InternalError final : public Exception {
       public:
        /**
         * @brief Exception thrown for internal errors in the ECS
         *
         * @param message The error message describing the internal error.
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit InternalError(const std::string& message,
                               const std::source_location loc = std::source_location::current())
            : Exception(std::format("Internal error: {}", message), loc)
        {}
    };

    class ComponentNotFound final : public Exception {
       public:
        /**
         * @brief Exception thrown when a component is not found for a given entity.
         *
         * @param entity The entity for which the component was not found.
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit ComponentNotFound(const Entity entity,
                                   const std::source_location loc = std::source_location::current())
            : Exception(std::format("Component not found for: {}", entity), loc)
        {}
    };

    class OverlappingGroupsException final : public Exception {
       public:
        /**
         * @brief Exception thrown when attempting to create a group with overlapping owned components.
         *
         * @param existingGroup The name of the existing group that has the conflicting component.
         * @param newGroup The name of the new group being created.
         * @param conflictingComponent The component type that is causing the overlap.
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit OverlappingGroupsException(const std::string& existingGroup, const std::string& newGroup,
                                            ComponentType conflictingComponent,
                                            const std::source_location loc = std::source_location::current())
            : Exception(
                  std::format(
                      "Cannot create group {} because it has overlapping owned component #{} with existing group {}",
                      newGroup, conflictingComponent, existingGroup),
                  loc)
        {}
    };

    class GroupNotFound final : public Exception {
       public:
        /**
         * @brief Exception thrown when a requested group is not found.
         *
         * @param groupKey The key identifying the group that was not found.
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit GroupNotFound(const std::string& groupKey,
                               const std::source_location loc = std::source_location::current())
            : Exception(std::format("Group not found for key: {}", groupKey), loc)
        {}
    };

    class InvalidGroupComponent final : public Exception {
       public:
        /**
         * @brief Exception thrown when a component is not found in the group.
         *
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit InvalidGroupComponent(const std::source_location loc = std::source_location::current())
            : Exception("Component has not been found in the group", loc)
        {}
    };

    class ComponentNotRegistered final : public Exception {
       public:
        /**
         * @brief Exception thrown when a component type is not registered in the ECS.
         *
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit ComponentNotRegistered(const std::source_location loc = std::source_location::current())
            : Exception("Component has not been registered before use", loc)
        {}
    };

    class SingletonComponentNotRegistered final : public Exception {
       public:
        /**
         * @brief Exception thrown when a singleton component type is not registered in the ECS.
         *
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit SingletonComponentNotRegistered(const std::source_location loc = std::source_location::current())
            : Exception("Singleton component has not been registered before use", loc)
        {}
    };

    class SystemNotRegistered final : public Exception {
       public:
        /**
         * @brief Exception thrown when a system type is not registered in the ECS.
         *
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit SystemNotRegistered(const std::source_location loc = std::source_location::current())
            : Exception("System has not been registered before use", loc)
        {}
    };

    class TooManyEntities final : public Exception {
       public:
        /**
         * @brief Exception thrown when the maximum number of living entities is exceeded.
         *
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit TooManyEntities(const std::source_location loc = std::source_location::current())
            : Exception(std::format("Too many living entities, max is {}", MAX_ENTITIES), loc)
        {}
    };

    class OutOfRange final : public Exception {
       public:
        /**
         * @brief Exception thrown when an index is out of the valid range.
         *
         * @param index The out-of-range index.
         * @param loc The source location where the exception was thrown (default is current location).
         */
        explicit OutOfRange(size_t index, const std::source_location loc = std::source_location::current())
            : Exception(std::format("Index {} is out of range", index), loc)
        {}
    };
} // namespace nexo::ecs
