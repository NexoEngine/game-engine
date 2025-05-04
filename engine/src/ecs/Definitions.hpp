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

	/**
	* @brief Entity identifier type
	*
	* Used to uniquely identify entities in the ECS.
	*/
	using Entity = std::uint32_t;

	/**
	* @brief Maximum number of entities that can exist simultaneously
	*/
	constexpr Entity MAX_ENTITIES = 500000;

	/**
	* @brief Special value representing an invalid or non-existent entity
	*/
	constexpr Entity INVALID_ENTITY = std::numeric_limits<Entity>::max();

	// Component type definitions

	/**
	* @brief Component type identifier
	*
	* Used to uniquely identify different component types.
	*/
	using ComponentType = std::uint8_t;

	/**
	* @brief Maximum number of different component types in the system
	*/
	constexpr ComponentType MAX_COMPONENT_TYPE = 32;

	/**
	* @brief Global counter for generating unique component type IDs
	*/
	inline ComponentType globalComponentCounter = 0;

	/**
	* @brief Gets a unique ID for a component type
	*
	* Returns a statically allocated ID for each unique component type T.
	* The first call for a type T will assign a new ID; subsequent calls
	* for the same type will return the previously assigned ID.
	*
	* @tparam T Component type
	* @return ComponentType Unique ID for the type
	*/
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

    /**
     * @brief Gets the component type ID, removing const/volatile/reference qualifiers
     *
     * @tparam T Component type
     * @return ComponentType ID for the component type
     */
	template<typename T>
	ComponentType getComponentTypeID()
	{
	  return getUniqueComponentTypeID<std::remove_cvref_t<T>>();
	}

	// Group type definition

	/**
	* @brief Group identifier type
	*
	* Used to uniquely identify different entity groups.
	*/
	using GroupType = std::uint8_t;

	/**
	* @brief Maximum number of groups that can exist simultaneously
	*/
	constexpr GroupType MAX_GROUP_NUMBER = 32;

	/**
	* @brief Signature type for component composition
	*
	* A bitset where each bit represents whether an entity has a specific component type.
	*/
	using Signature = std::bitset<MAX_COMPONENT_TYPE>;

}
