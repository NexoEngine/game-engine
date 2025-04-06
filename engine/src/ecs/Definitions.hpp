//// Definitions.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        31/03/2025
//  Description: Header file containing type definitions and constants
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <limits>
#include <bitset>
#include <cassert>

namespace nexo::ecs {
	// Entity type definition
	using Entity = std::uint32_t;
	constexpr Entity MAX_ENTITIES = 500000;
	constexpr Entity INVALID_ENTITY = std::numeric_limits<Entity>::max();

	// Component type definitions
	using ComponentType = std::uint8_t;
	constexpr ComponentType MAX_COMPONENT_TYPE = 32;

	inline ComponentType globalComponentCounter = 0;

    template<typename T>
    ComponentType getUniqueComponentTypeID()
    {
        // This static variable is instantiated once per type T,
        // but it will be assigned a unique value from the shared global counter.
        static const ComponentType id = []() {
            assert(globalComponentCounter < MAX_COMPONENT_TYPE && "Maximum number of component types exceeded");
            return globalComponentCounter++;
        }();
        return id;
    }

	template<typename T>
	ComponentType getComponentTypeID()
	{
	  return getUniqueComponentTypeID<std::remove_cvref_t<T>>();
	}

	// Group type definition
	using GroupType = std::uint8_t;
	constexpr GroupType MAX_GROUP_NUMBER = 32;

	using Signature = std::bitset<MAX_COMPONENT_TYPE>;

}
