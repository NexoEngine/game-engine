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

#include <source_location>
#include <format>

namespace nexo::ecs {
    using Entity = std::uint32_t;

    class ComponentNotFound final : public Exception {
        public:
        explicit ComponentNotFound(const Entity entity,
                                    const std::source_location loc = std::source_location::current())
            : Exception(std::format("Component not found for: {}", entity), loc) {}
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
                : Exception("Too many living entities, max is 8191", loc) {}
    };

    class OutOfRange final : public Exception {
        public:
        explicit OutOfRange(unsigned int index, const std::source_location loc = std::source_location::current())
            : Exception(std::format("Index {} is out of range", index), loc) {}
    };
}
