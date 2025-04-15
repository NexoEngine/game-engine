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
#include <format>

#include "Exception.hpp"
#include "components/Light.hpp"

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

    class TooManyPointLightsException : public Exception {
    	public:
     		explicit TooManyPointLightsException(unsigned int sceneRendered, size_t nbPointLights,
                                           const std::source_location loc = std::source_location::current())
                : Exception(std::format("Too many point lights ({} > {}) in scene [{}]", nbPointLights, MAX_POINT_LIGHTS, sceneRendered), loc) {}
    };

    class TooManySpotLightsException : public Exception {
    	public:
     		explicit TooManySpotLightsException(unsigned int sceneRendered, size_t nbSpotLights,
                                           const std::source_location loc = std::source_location::current())
       : Exception(std::format("Too many spot lights ({} > {}) in scene [{}]", nbSpotLights, MAX_SPOT_LIGHTS, sceneRendered), loc) {}
    };

    class TooManyDirectionalLightsException : public Exception {
    	public:
     		explicit TooManyDirectionalLightsException(unsigned int sceneRendered, size_t nbDirectionalLights,
                                           const std::source_location loc = std::source_location::current())
       : Exception(std::format("Too many directional lights ({} > {}) in scene [{}]", nbDirectionalLights, MAX_DIRECTIONAL_LIGHTS, sceneRendered), loc) {}
    };
}
