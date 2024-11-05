/*** SaveManager.hpp **********************************************************
*
*  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
*  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
*  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
*  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
*  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
*
*  Author:      Guillaume Hein
*  Date:        19/10/2024
*  Description: Class used to manage the saving and loading of the game state
*               and objects.
*
******************************************************************************/

#pragma once

#include <cstdint>
#include <typeindex>
#include <unordered_map>
#include <stdexcept>
#include <format>

namespace engine::save {

	class SaveException : public std::runtime_error {
	public:
		explicit SaveException(const std::string& message) : std::runtime_error(message) {}
	};

	class SaveExceptionTypeNotRegistered : public SaveException {};

	class SaveExceptionTypeAlreadyRegistered : public SaveException {
	public:
		explicit SaveExceptionTypeAlreadyRegistered(const std::string& message) : SaveException(message) {}
	};


	/**
	 * @class SaveManager
	 * @brief Class used to manage the saving and loading of the game state and objects.
	 *
	 */
	class SaveManager {
	public:
		/**
		 * @brief Default constructor.
		 *
		 */
		SaveManager() = default;

		/**
		 * @brief Default destructor.
		 *
		 */
		~SaveManager() = default;

		/**
		 * @brief Save the current state of the game.
		 *
		 */
		void save() const;

		/**
		 * @brief Load the game state from a file.
		 *
		 */
		void load() const;

		/**
		 * @brief Register a type with a unique name.
		 *
		 * @tparam T Type to register.
		 * @param typeUniqueName Unique name to register the type with.
		 *
		 * @note The unique name must be unique across all types. It is used for long term saving of the type.
		 */
		template <typename T>
		void registerType(std::uint64_t typeUniqueName)
		{
			if (_typeUniqueNameToIndex.find(typeUniqueName) != _typeUniqueNameToIndex.end())
				throw std::runtime_error(std::format(
					"Type unique name {} already registered, tried to register type {}", typeUniqueName, typeid(T).name));
			if (_typeIndexToUniqueName.find(typeid(T)) != _typeIndexToUniqueName.end())
				throw std::runtime_error(std::format(
					"Type {} already registered, tried to register type unique name {}", typeid(T).name, typeUniqueName));
			_typeUniqueNameToIndex[typeUniqueName] = typeid(T);
			_typeIndexToUniqueName[typeid(T)] = typeUniqueName;
		}

		/**
		 * @brief Get the unique name of a type.
		 *
		 * @tparam T Type to get the unique name of.
		 * @return std::uint64_t The unique name of the type.
		 */
		template <typename T>
		[[nodiscard]] std::uint64_t getTypeUniqueName() const
		{
			if (_typeIndexToUniqueName.find(typeid(T)) == _typeIndexToUniqueName.end())
				throw SaveExceptionTypeNotRegistered(std::format("Type {} not registered", typeid(T).name));
			return _typeIndexToUniqueName.at(typeid(T));
		}

		/**
		 * @brief Get the type_index of a type's unique name.
		 *
		 * @param typeUniqueName Unique name of the type to get.
		 * @return std::type_index The type of the unique name.
		 */
		[[nodiscard]] std::type_index getTypeIndex(std::uint64_t typeUniqueName) const
		{
			if (_typeUniqueNameToIndex.find(typeUniqueName) == _typeUniqueNameToIndex.end())
				throw std::runtime_error(std::format("Type unique name {} not registered", typeUniqueName));
			return _typeUniqueNameToIndex.at(typeUniqueName);
		}

	private:
		std::unordered_map<std::uint64_t, std::type_index> _typeUniqueNameToIndex;
		std::unordered_map<std::type_index, std::uint64_t> _typeIndexToUniqueName;
	};

} // namespace engine::save
