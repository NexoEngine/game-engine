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

#include "Exception.hpp"
#include "Definitions.hpp"

#include <source_location>
#include <format>

namespace nexo::ecs {

    class InternalError final : public Exception {
    public:
        explicit InternalError(const std::string& message,
                                const std::source_location loc = std::source_location::current())
            : Exception(std::format("Internal error: {}", message), loc) {}
    };

    class ComponentNotFound final : public Exception {
        public:
            explicit ComponentNotFound(const Entity entity,
                                        const std::source_location loc = std::source_location::current())
                : Exception(std::format("Component not found for: {}", entity), loc) {}
    };

    class OverlappingGroupsException final : public Exception {
        public:
            explicit OverlappingGroupsException(const std::string& existingGroup,
                                                const std::string& newGroup, ComponentType conflictingComponent,
                                                const std::source_location loc = std::source_location::current())
                : Exception(std::format("Cannot create group {} because it has overlapping owned component #{} with existing group {}", newGroup, conflictingComponent, existingGroup), loc) {}
    };

    class GroupNotFound final : public Exception {
        public:
            explicit GroupNotFound(const std::string &groupKey,
                                    const std::source_location loc = std::source_location::current())
                : Exception(std::format("Group not found for key: {}", groupKey), loc) {}
    };

    class InvalidGroupComponent final : public Exception {
        public:
            explicit InvalidGroupComponent(const std::source_location loc = std::source_location::current())
                : Exception("Component has not been found in the group", loc) {}
    };

    class ComponentNotRegistered final : public Exception {
        public:
            explicit ComponentNotRegistered(const std::source_location loc = std::source_location::current())
                : Exception("Component has not been registered before use", loc) {}
    };

    class SingletonComponentNotRegistered final : public Exception {
        public:
            explicit SingletonComponentNotRegistered(const std::source_location loc = std::source_location::current())
                : Exception("Singleton component has not been registered before use", loc) {}
    };

    class SystemNotRegistered final : public Exception {
        public:
            explicit SystemNotRegistered(const std::source_location loc = std::source_location::current())
                : Exception("System has not been registered before use", loc) {}
    };

    class TooManyEntities final : public Exception {
        public:
            explicit TooManyEntities(const std::source_location loc = std::source_location::current())
                : Exception(std::format("Too many living entities, max is {}", MAX_ENTITIES), loc) {}
    };

    class OutOfRange final : public Exception {
        public:
            explicit OutOfRange(unsigned int index, const std::source_location loc = std::source_location::current())
                : Exception(std::format("Index {} is out of range", index), loc) {}
    };
}
