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

namespace nexo::ecs {
    using Entity = std::uint32_t;

    class ComponentNotFound final : public Exception {
        public:
            explicit ComponentNotFound(const Entity entity, const char *file = __FILE__,
                                       const int line = __LINE__)
                : Exception("Component not found for: " + std::to_string(entity), file, line) {}
    };

    class ComponentNotRegistered final : public Exception {
        public:
            explicit ComponentNotRegistered(const char *file = __FILE__,
                                            const int line = __LINE__)
                : Exception("Component has not been registered before use", file, line) {}
    };

    class SingletonComponentNotRegistered final : public Exception {
        public:
            explicit SingletonComponentNotRegistered(const char *file = __FILE__,
                                                     const int line = __LINE__)
                : Exception("Singleton component has not been registered before use", file, line) {}
    };

    class SystemNotRegistered final : public Exception {
        public:
            explicit SystemNotRegistered(const char *file = __FILE__,
                                         const int line = __LINE__)
                : Exception("System has not been registered before use", file, line) {}
    };

    class TooManyEntities final : public Exception {
        public:
            explicit TooManyEntities(const char *file = __FILE__,
                                     const int line = __LINE__)
                : Exception("Too many living entities, max is 8191", file, line) {}
    };

    class OutOfRange final : public Exception {
        public:
            explicit OutOfRange(unsigned int index, const char *file = __FILE__,
                                const int line = __LINE__)
                : Exception("Index " + std::to_string(index) + " is out of range", file, line) {}
    };
}
