//// Access.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/04/2025
//  Description: Header file for access enforcement helpers
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <tuple>
#include <typeindex>
#include <type_traits>

namespace nexo::ecs {
    /**
     * @brief Access type for components in systems
     */
    enum class AccessType {
        Read,  ///< Read-only access
        Write  ///< Read-write access
    };

    /**
     * @brief Template to specify component access type
     *
     * @tparam T The component type
     * @tparam Access The access type (Read or Write)
     */
    template<typename T, AccessType Access>
    struct ComponentAccess {
        using ComponentType = T;
        static constexpr AccessType accessType = Access;
    };

    /**
     * @brief Type alias for read-only component access
     */
    template<typename T>
    using Read = ComponentAccess<T, AccessType::Read>;

    /**
     * @brief Type alias for read-write component access
     */
    template<typename T>
    using Write = ComponentAccess<T, AccessType::Write>;

    /**
     * @brief Type alias for read-only singleton component access
     */
    template<typename T>
    struct ReadSingleton {
        using ComponentType = T;
        static constexpr AccessType accessType = AccessType::Read;
    };

    /**
     * @brief Type alias for read-write singleton component access
     */
    template<typename T>
    struct WriteSingleton {
        using ComponentType = T;
        static constexpr AccessType accessType = AccessType::Write;
    };

    /**
     * @brief Type wrapper for owned components in a group system
     *
     * @tparam Components Component access types (Read<T> or Write<T>)
     */
    template<typename... Components>
    struct Owned {
        using ComponentTypes = std::tuple<Components...>;
    };

    /**
     * @brief Type wrapper for non-owned components in a group system
     *
     * @tparam Components Component access types (Read<T> or Write<T>)
     */
    template<typename... Components>
    struct NonOwned {
        using ComponentTypes = std::tuple<Components...>;
    };

    /**
     * @brief Helper to extract component types from access types
     *
     * @tparam AccessTypes Pack of component access types
     */
    template<typename... AccessTypes>
    struct ExtractComponentTypes;

    /**
     * @brief Specialization for extracting component types
     */
    template<typename... AccessTypes>
    struct ExtractComponentTypes<std::tuple<AccessTypes...>> {
        using Types = std::tuple<typename AccessTypes::ComponentType...>;
    };

    /**
     * @brief Helper to convert a tuple of component access types to a parameter pack
     */
    template<typename Tuple, typename Func, std::size_t... I>
    void tuple_for_each_impl(Tuple&& tuple, Func&& func, std::index_sequence<I...>)
    {
        (func(std::get<I>(std::forward<Tuple>(tuple))), ...);
    }

    /**
     * @brief Apply a function to each element of a tuple
     */
    template<typename Tuple, typename Func>
    void tuple_for_each(Tuple&& tuple, Func&& func)
    {
        tuple_for_each_impl(
            std::forward<Tuple>(tuple),
            std::forward<Func>(func),
            std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{}
        );
    }

    /**
     * @brief Helper to check if a type is a ReadSingleton
     */
    template<typename T>
    struct IsReadSingleton : std::false_type {};

    template<typename T>
    struct IsReadSingleton<ReadSingleton<T>> : std::true_type {};

    /**
     * @brief Helper to check if a type is a WriteSingleton
     */
    template<typename T>
    struct IsWriteSingleton : std::false_type {};

    template<typename T>
    struct IsWriteSingleton<WriteSingleton<T>> : std::true_type {};

    /**
     * @brief Helper to check if a type is any kind of singleton component
     */
    template<typename T>
    struct IsSingleton : std::bool_constant<IsReadSingleton<T>::value || IsWriteSingleton<T>::value> {};
}
