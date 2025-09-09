//// SingletonComponent.hpp ///////////////////////////////////////////////////
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
//  Date:        08/11/2024
//  Description: Header file for the singleton component class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unordered_map>
#include <memory>

#include "Definitions.hpp"
#include "Logger.hpp"
#include "ECSExceptions.hpp"

namespace nexo::ecs {

	/**
	* @brief Base interface for singleton components.
	*
	* All singleton components must derive from this interface. It ensures proper polymorphic destruction.
	*/
	class ISingletonComponent {
		public:
			virtual ~ISingletonComponent() = default;
	};

    /**
     * @brief Template class representing a singleton component.
     *
     * This class wraps an instance of type T as a singleton component.
     *
     * @tparam T The type of the singleton component.
     */
	template <typename T>
	class SingletonComponent final : public ISingletonComponent {
		public:
			static_assert(!std::is_copy_constructible_v<T>,
				"Singleton component types must have a deleted copy constructor");
			/**
			* @brief Templated constructor that perfectly forwards arguments to construct the instance.
			*
			* This constructor is only enabled when T is constructible from the provided arguments
			* and none of the arguments (after decaying) is of type T (to avoid accidental copying or moving
			* of an object of the same type).
			*
			* @tparam Args The types of constructor arguments.
			* @param args Arguments forwarded to T's constructor.
			*/
			template<typename... Args>
			requires (std::is_constructible_v<T, Args...> && (!std::is_same_v<std::decay_t<Args>, T> && ...))
			explicit SingletonComponent(Args&&... args)
				: _instance(std::forward<Args>(args)...)
			{
			}

			SingletonComponent() = default;
			~SingletonComponent() override = default;

			// Prevent copying
			SingletonComponent(const SingletonComponent&) = delete;
			SingletonComponent& operator=(const SingletonComponent&) = delete;

			// Allow moving
			SingletonComponent(SingletonComponent&&) noexcept = default;
			SingletonComponent& operator=(SingletonComponent&&) noexcept = default;

			/**
			* @brief Gets the singleton component instance
			*
			* @return T& Reference to the instance of the singleton component
			*/
			T &getInstance() { return _instance; }

		private:
			T _instance;
	};

    /**
     * @brief Manager for singleton components in the ECS.
     *
     * The SingletonComponentManager is responsible for registering, retrieving, and unregistering
     * singleton components. Singleton components are globally unique and accessed via their type.
     */
	class SingletonComponentManager {
		public:
			/**
			* @brief Registers a singleton component in place by forwarding constructor arguments.
			*
			* This method constructs the singleton directly in the manager using the provided arguments.
			*
			* @tparam T The type of the singleton component.
			* @tparam Args The types of the constructor arguments.
			* @param args Arguments to construct an instance of T.
			*/
			template <typename T, typename... Args>
			void registerSingletonComponent(Args&&... args)
			{
			    ComponentType typeName = getUniqueComponentTypeID<T>();
				if (m_singletonComponents.contains(typeName)) {
					LOG(NEXO_WARN, "ECS::SingletonComponentManager::registerSingletonComponent: trying to register a singleton component more than once");
					return;
				}
				m_singletonComponents.insert(
					{typeName,
					std::make_shared<SingletonComponent<T>>(std::forward<Args>(args)...)}
				);
			}

			/**
			* @brief Retrieves a singleton component instance.
			*
			* @tparam T The type of the singleton component.
			* @return T& A reference to the registered singleton component.
			* @throws SingletonComponentNotRegistered if the component is not registered.
			*/
			template <typename T>
			T &getSingletonComponent()
			{
			    const ComponentType typeName = getUniqueComponentTypeID<T>();
				if (!m_singletonComponents.contains(typeName))
					THROW_EXCEPTION(SingletonComponentNotRegistered);

				auto componentPtr = dynamic_cast<SingletonComponent<T>*>(m_singletonComponents[typeName].get());

				return componentPtr->getInstance();
			}

			/**
			* @brief Retrieves a singleton component pointer (internal use only).
			*
			* @tparam T The type of the singleton component.
			* @return std::shared_ptr<ISingletonComponent> A shared pointer to the registered singleton component.
			* @throws SingletonComponentNotRegistered if the component is not registered.
			*/
			template <typename T>
			std::shared_ptr<ISingletonComponent> getRawSingletonComponent()
			{
			    const ComponentType typeName = getUniqueComponentTypeID<T>();
				if (!m_singletonComponents.contains(typeName))
					THROW_EXCEPTION(SingletonComponentNotRegistered);

				return m_singletonComponents[typeName];
			}

			/**
			* @brief Unregisters a singleton component.
			*
			* Removes the singleton component of type T from the manager.
			*
			* @tparam T The type of the singleton component.
			* @throws SingletonComponentNotRegistered if the component is not registered.
			*/
			template <typename T>
			void unregisterSingletonComponent()
			{
				const ComponentType typeName = getUniqueComponentTypeID<T>();
				if (!m_singletonComponents.contains(typeName))
					THROW_EXCEPTION(SingletonComponentNotRegistered);

				m_singletonComponents.erase(typeName);
			}
		private:
			std::unordered_map<ComponentType, std::shared_ptr<ISingletonComponent>> m_singletonComponents{};
	};
}
