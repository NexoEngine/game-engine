//// Group.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        01/04/2025
//  Description: Header file for the ECS groups
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Definitions.hpp"
#include "ComponentArray.hpp"
#include "ECSExceptions.hpp"
#include "Exception.hpp"

#include <functional>
#include <span>
#include <memory>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <string>

namespace nexo::ecs {

	/**
	 * @brief Interface for ECS groups.
	 *
	 * This interface defines the minimum requirements for groups that store a set
	 * of entities along with their associated component signatures.
	 */
	class IGroup {
		public:
		    virtual ~IGroup() = default;

		    /**
		     * @brief Returns the combined signature of all components in the group.
		     *
		     * @return const Signature& Combined signature.
		     */
		    [[nodiscard]] virtual const Signature& allSignature() const = 0;
		    /**
		     * @brief Adds an entity to the group.
		     *
		     * @param e Entity to add.
		     */
		    virtual void addToGroup(Entity e) = 0;
		    /**
		     * @brief Removes an entity from the group.
		     *
		     * @param e Entity to remove.
		     */
		    virtual void removeFromGroup(Entity e) = 0;
	};

	/**
	 * @brief Helper template that always evaluates to false.
	 *
	 * This is used to trigger static_assert in template functions.
	 *
	 * @tparam T Type to test.
	 */
	template<typename T>
	struct dependent_false : std::false_type {};

	/**
	 * @brief Metafunction to check if a tuple contains a specific component type.
	 *
	 * Example: if Tuple is std::tuple<std::shared_ptr<ComponentArray<Position>>, std::shared_ptr<ComponentArray<Velocity>>>
	 * then tuple_contains_component<T, Tuple>::value is true if T is Position or Velocity.
	 *
	 * @tparam T Component type to check.
	 * @tparam Tuple Tuple type containing pointers to ComponentArray objects.
	 */
	template<typename T, typename Tuple>
	struct tuple_contains_component;

	/**
	 * @brief Specialization of tuple_contains_component for std::tuple.
	 *
	 * @tparam T Component type to check.
	 * @tparam Ptrs Pointer types stored in the tuple.
	 */
	template<typename T, typename... Ptrs>
	struct tuple_contains_component<T, std::tuple<Ptrs...>>
	    : std::disjunction<std::is_same<T, typename std::decay_t<decltype(*std::declval<Ptrs>())>::component_type>...> {};

	/**
	 * @brief Convenience variable for tuple_contains_component.
	 *
	 * @tparam T Component type to check.
	 * @tparam Tuple Tuple type.
	 */
	template<typename T, typename Tuple>
	constexpr bool tuple_contains_component_v = tuple_contains_component<T, Tuple>::value;

	/**
	 * @brief Represents a partition of entities based on a key.
	 *
	 * @tparam KeyType The type of the key used for partitioning.
	 */
	template<typename KeyType>
	struct Partition {
	    KeyType key;       ///< The partition key.
	    size_t startIndex; ///< The starting index of the partition.
	    size_t count;      ///< The number of entities in the partition.
	};

	/**
	 * @brief Alias for a function that extracts a field from a component.
	 *
	 * @tparam T Component type.
	 * @tparam FieldType Type of the extracted field.
	 */
	template<typename T, typename FieldType>
	using FieldExtractor = std::function<FieldType(const T&)>;

	/**
	 * @brief Alias for a function that extracts a key from an entity.
	 *
	 * @tparam KeyType Type of the key.
	 */
	template<typename KeyType>
	using EntityKeyExtractor = std::function<KeyType(Entity)>;

	/**
	 * @brief Group class for a view over entities with both owned and non‑owned components.
	 *
	 * Two tuple types are taken:
	 *   - OwnedTuple: std::tuple<Owned...>
	 *   - NonOwnedTuple: std::tuple<NonOwning...>
	 *
	 * @tparam OwnedTuple Tuple of pointers (or smart pointers) to owned component arrays.
	 * @tparam NonOwnedTuple Tuple of pointers to non‑owned component arrays.
	 */
	template<typename OwnedTuple, typename NonOwnedTuple>
	class Group final : public IGroup {
		public:
			/**
			 * @brief Constructs a new Group.
			 *
			 * @tparam NonOwning Variadic template parameters for non‑owned components.
			 * @param ownedArrays Tuple of pointers to owned component arrays.
			 * @param nonOwnedArrays Tuple of pointers to non‑owned component arrays.
			 *
			 * The constructor computes the owned and non‑owned signatures and their combination.
			 */
			template<typename... NonOwning>
	            requires (std::tuple_size_v<OwnedTuple> > 0) // Ensure at least one owned component for Group
			Group(OwnedTuple ownedArrays, NonOwnedTuple nonOwnedArrays)
			    : m_ownedArrays(std::move(ownedArrays))
			    , m_nonOwnedArrays(std::move(nonOwnedArrays))
			{
				m_ownedSignature = std::apply([]([[maybe_unused]] auto&&... arrays) {
				    Signature signature;
				    ((signature.set(getComponentTypeID<typename std::decay_t<decltype(*arrays)>::component_type>())), ...);
				    return signature;
				}, m_ownedArrays);

				const Signature nonOwnedSignature = std::apply([]([[maybe_unused]] auto&&... arrays) {
				    Signature signature;
				    ((signature.set(getComponentTypeID<typename std::decay_t<decltype(*arrays)>::component_type>())), ...);
				    return signature;
				}, m_nonOwnedArrays);

				m_allSignature = m_ownedSignature | nonOwnedSignature;
			}

			// =======================================
			// Core Group API
			// =======================================

			/**
			 * @brief Returns the number of entities in the group.
			 *
			 * @return std::size_t Number of entities.
			 */
			[[nodiscard]] std::size_t size() const
			{
			    auto firstArray = std::get<0>(m_ownedArrays);
			    if (!firstArray) {
			        THROW_EXCEPTION(InternalError, "Component array is null");
			    }
			    return firstArray->groupSize();
			}

		    /**
		     * @brief Checks if sorting has been invalidated.
		     *
		     * @return true If sorting is invalidated.
		     * @return false Otherwise.
		     */
			[[nodiscard]] bool sortingInvalidated() const { return m_sortingInvalidated; }

		    /**
		     * @brief Returns the signature for owned components.
		     *
		     * @return const Signature& Owned signature.
		     */
		    [[nodiscard]] const Signature& ownedSignature() const { return m_ownedSignature; }

		    /**
		     * @brief Returns the overall signature for both owned and non‑owned components.
		     *
		     * @return const Signature& Combined signature.
		     */
		    [[nodiscard]] const Signature& allSignature()   const override { return m_allSignature; }

		    /**
		     * @brief Iterator for Group.
		     *
		     * Allows iterating over entities along with their owned and non‑owned components.
		     */
		    class GroupIterator {
			    public:
			        using iterator_category = std::forward_iterator_tag;
			        /// The type returned by dereferencing the iterator.
			        using value_type = decltype(std::declval<Group>().dereference(0));
			        using reference = value_type;
			        using difference_type = std::ptrdiff_t;
			        using pointer = void;

			        /**
			         * @brief Constructs a GroupIterator.
			         *
			         * @param view Pointer to the Group.
			         * @param index Starting index.
			         */
			        GroupIterator(const Group* view, std::size_t index)
			            : m_view(view), m_index(index) {}

			        /**
			         * @brief Dereferences the iterator to get the entity and its components.
			         *
			         * @return reference Tuple containing the entity and its component data.
			         */
			        reference operator*() const
					{
						if (m_index >= m_view->size())
							THROW_EXCEPTION(OutOfRange, m_index);

			            return m_view->dereference(m_index);
			        }

			        /**
			         * @brief Pre-increment operator.
			         *
			         * @return GroupIterator& Reference to the iterator after increment.
			         */
			        GroupIterator& operator++()
					{
					   ++m_index;
						return *this;
					}

			        /**
			         * @brief Post-increment operator.
			         *
			         * @return GroupIterator Iterator before increment.
			         */
			        GroupIterator operator++(int)
					{
                        GroupIterator tmp = *this;
                        ++(*this);
                        return tmp;
					}

			        /**
			         * @brief Equality operator.
			         *
			         * @param other Another iterator.
			         * @return true If both iterators are equal.
			         * @return false Otherwise.
			         */
			        bool operator==(const GroupIterator& other) const
					{
			            return m_index == other.m_index && m_view == other.m_view;
			        }

			    private:
			        const Group* m_view; ///< Pointer to the group.
			        std::size_t m_index; ///< Current index in the group.
			};

			/**
			* @brief Returns an iterator to the beginning of the group
			*
			* @return GroupIterator Iterator pointing to the first entity
			*/
			GroupIterator begin() const { return GroupIterator(this, 0); }

			/**
			* @brief Returns an iterator to the end of the group
			*
			* @return GroupIterator Iterator pointing beyond the last entity
			*/
			GroupIterator end()   const { return GroupIterator(this, size()); }

			/**
			* @brief Returns an iterator to the beginning of the group (non-const version)
			*
			* @return GroupIterator Iterator pointing to the first entity
			*/
			GroupIterator begin() { return GroupIterator(this, 0); }

			/**
			* @brief Returns an iterator to the end of the group (non-const version)
			*
			* @return GroupIterator Iterator pointing beyond the last entity
			*/
			GroupIterator end()   { return GroupIterator(this, size()); }

		    /**
		     * @brief Iterates over each entity in the group.
		     *
		     * The callable 'func' must accept parameters of the form:
		     * (Entity, Owned&..., NonOwned&...).
		     *
		     * @tparam Func Callable type.
		     * @param func Function to call for each entity.
		     */
		    template<typename Func>
		    void each(Func func) const
			{
				auto firstArray = std::get<0>(m_ownedArrays);
				if (!firstArray)
					THROW_EXCEPTION(InternalError, "Component array is null");

				for (std::size_t i = 0; i < firstArray->groupSize(); ++i) {
					Entity e = firstArray->getEntityAtIndex(i);
					callFunc(func, e,
						std::make_index_sequence<std::tuple_size_v<OwnedTuple>>{},
						std::make_index_sequence<std::tuple_size_v<NonOwnedTuple>>{});
				}
		    }

		    /**
		     * @brief Iterates over a sub-range of entities in the group.
		     *
		     * @tparam Func Callable type.
		     * @param startIndex Starting index.
		     * @param count Number of entities to process.
		     * @param func Function to call for each entity in range.
		     */
		    template<typename Func>
			void eachInRange(size_t startIndex, const size_t count, Func func) const
			{
				auto firstArray = std::get<0>(m_ownedArrays);
				if (!firstArray)
					THROW_EXCEPTION(InternalError, "Component array is null");

				if (startIndex >= firstArray->groupSize())
					return; // Nothing to iterate

				const size_t endIndex = std::min(startIndex + count, firstArray->groupSize());

				for (size_t i = startIndex; i < endIndex; i++) {
					Entity e = firstArray->getEntityAtIndex(i);
					callFunc(func, e,
						std::make_index_sequence<std::tuple_size_v<OwnedTuple>>{},
						std::make_index_sequence<std::tuple_size_v<NonOwnedTuple>>{});
				}
			}

		    /**
		     * @brief Adds an entity to the group.
		     *
		     * This method calls addToGroup(e) on every owned component array.
		     *
		     * @param e Entity to add.
		     */
		    void addToGroup(Entity e) override
		    {
				std::apply([e](auto&&... arrays) {
					((arrays->addToGroup(e)), ...);
				}, m_ownedArrays);

				m_sortingInvalidated = true;
				invalidatePartitions();
		    }

		    /**
		     * @brief Removes an entity from the group.
		     *
		     * This method calls removeFromGroup(e) on every owned component array.
		     *
		     * @param e Entity to remove.
		     */
		    void removeFromGroup(Entity e) override
		    {
				std::apply([e](auto&&... arrays) {
					((arrays->removeFromGroup(e)), ...);
				}, m_ownedArrays);

				m_sortingInvalidated = true;
				invalidatePartitions();
		    }

		    /**
		     * @brief Retrieves a span of entity IDs corresponding to the group.
		     *
		     * This is taken from the first owned component array's entities() span,
		     * restricted to its group region.
		     *
		     * @return std::span<const Entity> Span of entity IDs.
		     */
			[[nodiscard]] std::span<const Entity> entities() const
			{
				const std::span<const Entity> entities = std::get<0>(m_ownedArrays)->entities();
				return entities.subspan(0, std::get<0>(m_ownedArrays)->groupSize());
			}

			/**
			 * @brief Retrieves the component array data for a given component type.
			 *
			 * This overload returns a const view of the data.
			 *
			 * @tparam T Component type.
			 * @return auto Span over component data.
			 */
			template<typename T>
			auto get() const
			{
				if constexpr (tuple_contains_component_v<T, OwnedTuple>) {
					auto compArray = getOwnedImpl<T>();  // internal lookup in owned tuple
					if (!compArray)
						THROW_EXCEPTION(InternalError, "Component array is null");

					return compArray->getAllComponents().subspan(0, compArray->groupSize());
				} else if constexpr (tuple_contains_component_v<T, NonOwnedTuple>)
					return getNonOwnedImpl<T>();         // internal lookup in non‑owned tuple
				else
					static_assert(dependent_false<T>::value, "Component type not found in group");
			}

			/**
			 * @brief Retrieves the component array data for a given component type.
			 *
			 * This overload returns a mutable view of the data.
			 *
			 * @tparam T Component type.
			 * @return auto Span over component data.
			 */
			template<typename T>
			auto get()
			{
				if constexpr (tuple_contains_component_v<T, OwnedTuple>) {
					auto compArray = getOwnedImpl<T>();  // internal lookup in owned tuple
					if (!compArray)
						THROW_EXCEPTION(InternalError, "Component array is null");

					return compArray->getAllComponents().subspan(0, compArray->groupSize());
				} else if constexpr (tuple_contains_component_v<T, NonOwnedTuple>)
					return getNonOwnedImpl<T>();         // internal lookup in non‑owned tuple
				else
					static_assert(dependent_false<T>::value, "Component type not found in group");
			}

			// =======================================
			// Sorting API
			// =======================================

			/**
			* @brief Marks the group's sorting as invalidated
			* Should be called when modifying a component that can affect the sorting
			*
			* When sorting is invalidated, the next call to sortBy() will perform a full resort.
			*/
			void invalidateSorting()
			{
				m_sortingInvalidated = true;
			}

			/**
			 * @brief Sorts the group by a specified component field.
			 *
			 * The sorting is only performed if the sorting is invalidated.
			 *
			 * @tparam CompType Component type to sort by.
			 * @tparam FieldType Field type to compare.
			 * @param extractor Function to extract the field value.
			 * @param ascending Set to true for ascending order (default true).
			 */
			template<typename CompType, typename FieldType>
			void sortBy(FieldExtractor<CompType, FieldType> extractor, bool ascending = true)
			{
				SortingOrder sortingOrder = ascending ? SortingOrder::ASCENDING : SortingOrder::DESCENDING;

				if (sortingOrder != m_sortingOrder) {
					m_sortingOrder = sortingOrder;
					m_sortingInvalidated = true;
				}

				if (!m_sortingInvalidated)
					return;

				std::shared_ptr<ComponentArray<CompType>> compArray;

				if constexpr (tuple_contains_component_v<CompType, OwnedTuple>) {
					compArray = getOwnedImpl<CompType>();
				} else if constexpr (tuple_contains_component_v<CompType, NonOwnedTuple>) {
					compArray = getNonOwnedImpl<CompType>();
				} else {
				    static_assert(dependent_false<CompType>::value, "Component type not found in group");
			    }

				if (!compArray)
					THROW_EXCEPTION(InternalError, "Component array is null");

			    auto drivingArray = std::get<0>(m_ownedArrays);
			    const size_t groupSize = drivingArray->groupSize();

			    std::vector<Entity> entities;
			    entities.reserve(groupSize);

			    // Add all entities currently in the group
			    for (size_t i = 0; i < groupSize; i++)
			        entities.push_back(drivingArray->getEntityAtIndex(i));

			    // Sort entities based on the extracted field from the component
				std::ranges::sort(entities,
                    [&](Entity a, Entity b) {
                        const auto& compA = compArray->get(a);
                        const auto& compB = compArray->get(b);
                        if (ascending)
                            return extractor(compA) < extractor(compB);
                        return extractor(compA) > extractor(compB);
                });

			    reorderGroup(entities);
				m_sortingInvalidated = false;
			}

			// =======================================
			// Partitioning API
			// =======================================

			/**
			 * @brief A view over a partition of entities based on a key.
			 *
			 * @tparam KeyType The type of the partition key.
			 */
			template<typename KeyType>
			class PartitionView {
				public:
					/**
					* @brief Constructs a PartitionView.
					*
					* @param group Pointer to the group.
					* @param partitions Reference to a vector of Partition objects.
					*/
					PartitionView(Group* group, const std::vector<Partition<KeyType>>& partitions)
						: m_group(group), m_partitions(partitions) {}

					/**
					* @brief Retrieves a partition by key.
					*
					* @param key Key to search for.
					* @return const Partition<KeyType>* Pointer to the partition if found; nullptr otherwise.
					*/
					const Partition<KeyType>* getPartition(const KeyType& key) const
					{
						for (const auto& partition : m_partitions) {
							if (partition.key == key)
								return &partition;
						}
						return nullptr;
					}

					/**
					* @brief Iterates over entities in a specific partition.
					*
					* @tparam Func Callable type.
					* @param key Key of the partition.
					* @param func Function to apply to each entity.
					*/
					template<typename Func>
					void each(const KeyType& key, Func func) const
					{
						const auto* partition = getPartition(key);
						if (!partition)
							return;

						m_group->eachInRange(partition->startIndex, partition->count, func);
					}

					/**
					* @brief Gets all partition keys.
					*
					* @return std::vector<KeyType> Vector of partition keys.
					*/
					std::vector<KeyType> getPartitionKeys() const
					{
						std::vector<KeyType> keys;
						keys.reserve(m_partitions.size());
						for (const auto& partition : m_partitions)
							keys.push_back(partition.key);
						return keys;
					}

					/**
					* @brief Returns the number of partitions.
					*
					* @return size_t Partition count.
					*/
					[[nodiscard]] size_t partitionCount() const
					{
						return m_partitions.size();
					}

				private:
					Group* m_group; ///< Pointer to the group.
					const std::vector<Partition<KeyType>>& m_partitions; ///< Reference to partitions.
			};

			/**
			 * @brief Returns a partition view based on a component field.
			 *
			 * @tparam CompType Component type used to partition.
			 * @tparam KeyType Key type extracted from the component.
			 * @param keyExtractor Function to extract the key from the component.
			 * @return PartitionView<KeyType> View over the partitioned entities.
			 */
			template<typename CompType, typename KeyType>
			PartitionView<KeyType> getPartitionView(FieldExtractor<CompType, KeyType> keyExtractor)
			{
				std::string typeId = typeid(KeyType).name();
				typeId += "_" + std::string(typeid(CompType).name());

				EntityKeyExtractor<KeyType> entityKeyExtractor = [this, keyExtractor](Entity e) {
				if constexpr (tuple_contains_component_v<CompType, OwnedTuple>) {
					auto compArray = getOwnedImpl<CompType>();
					if (!compArray)
						THROW_EXCEPTION(InternalError, "Component array is null");

					return keyExtractor(compArray->get(e));
				} else if constexpr (tuple_contains_component_v<CompType, NonOwnedTuple>) {
					auto compArray = getNonOwnedImpl<CompType>();
					if (!compArray)
						THROW_EXCEPTION(InternalError, "Component array is null");

					return keyExtractor(compArray->get(e));
				} else
					static_assert(dependent_false<CompType>::value, "Component type not found in group");
				};

				return getEntityPartitionView<KeyType>(typeId, entityKeyExtractor);
			}

			/**
			* @brief Returns a partition view based directly on entity IDs.
			*
			* @tparam KeyType Key type.
			* @param partitionId Identifier for the partition view.
			* @param keyExtractor Function to extract the key from an entity.
			* @return PartitionView<KeyType> View over the partitioned entities.
			*/
			template<typename KeyType>
			PartitionView<KeyType> getEntityPartitionView(const std::string& partitionId,
															EntityKeyExtractor<KeyType> keyExtractor)
			{
				// Check if we already have this partition view
				auto it = m_partitionStorageMap.find(partitionId);
				if (it == m_partitionStorageMap.end()) {
					auto storage = std::make_unique<PartitionStorage<KeyType>>(this, keyExtractor);
					auto* storagePtr = storage.get();
					m_partitionStorageMap[partitionId] = std::move(storage);
					storagePtr->rebuild();

					return PartitionView<KeyType>(this, storagePtr->getPartitions());
				}

				// Get the existing storage and cast to the right type
				auto* storage = static_cast<PartitionStorage<KeyType>*>(it->second.get());

				if (storage->isDirty())
					storage->rebuild();

				return PartitionView<KeyType>(this, storage->getPartitions());
			}

			/**
			* @brief Invalidates all partition caches.
			*/
			void invalidatePartitions()
			{
				for (auto& [_, storage] : m_partitionStorageMap)
					storage->markDirty();
			}

		private:

			// =======================================
			// Internal structures and methods
			// =======================================

			/**
			* @brief Interface for type-erased partition storage.
			*
			* This allows handling partition storage for different key types uniformly.
			*/
			struct IPartitionStorage {
				virtual ~IPartitionStorage() = default;
				/**
				* @brief Checks if the partition storage is dirty (needs rebuilding).
				*
				* @return true If dirty.
				* @return false Otherwise.
				*/
				[[nodiscard]] virtual bool isDirty() const = 0;
				/**
				* @brief Marks the partition storage as dirty.
				*/
				virtual void markDirty() = 0;
				/**
				* @brief Rebuilds the partition storage.
				*/
				virtual void rebuild() = 0;
			};

			/**
			 * @brief Concrete partition storage for a specific key type.
			 *
			 * @tparam KeyType Type of the partition key.
			 */
			template<typename KeyType>
			class PartitionStorage final : public IPartitionStorage {
				public:
					/**
					* @brief Constructs PartitionStorage.
					*
					* @param group Pointer to the group.
					* @param keyExtractor Function to extract key from an entity.
					*/
					PartitionStorage(Group* group, EntityKeyExtractor<KeyType> keyExtractor)
						: m_group(group), m_keyExtractor(std::move(keyExtractor)) {}

					[[nodiscard]] bool isDirty() const override { return m_isDirty; }
					void markDirty() override { m_isDirty = true; }

					/**
					* @brief Rebuilds the partitions.
					*
					* This collects all entity keys and creates partitions. It then reorders
					* the group entities according to the new partition order.
					*/
					void rebuild() override
					{
						if (!m_isDirty)
							return;
						auto drivingArray = std::get<0>(m_group->m_ownedArrays);
						const size_t groupSize = drivingArray->groupSize();

						// Skip if no entities
						if (groupSize == 0) {
							m_partitions.clear();
							m_isDirty = false;
							return;
						}

						std::unordered_map<KeyType, std::vector<Entity>> keyToEntities;

						for (size_t i = 0; i < groupSize; i++) {
							Entity e = drivingArray->getEntityAtIndex(i);
							KeyType key = m_keyExtractor(e);
							keyToEntities[key].push_back(e);
						}

						m_partitions.clear();
						m_partitions.reserve(keyToEntities.size());

						std::vector<Entity> newOrder;
						newOrder.reserve(groupSize);

						size_t currentIndex = 0;
						for (auto& [key, entities] : keyToEntities) {
							Partition<KeyType> partition;
							partition.key = key;
							partition.startIndex = currentIndex;
							partition.count = entities.size();
							m_partitions.push_back(partition);

							// Add these entities to the new order
							newOrder.insert(newOrder.end(), entities.begin(), entities.end());

							currentIndex += entities.size();
						}

						m_group->reorderGroup(newOrder);
						m_isDirty = false;
					}

					/**
					* @brief Gets the current partitions.
					*
					* @return const std::vector<Partition<KeyType>>& Reference to the partitions.
					*/
					const std::vector<Partition<KeyType>>& getPartitions() const
					{
						return m_partitions;
					}

				private:
					Group* m_group; ///< Pointer to the group.
					EntityKeyExtractor<KeyType> m_keyExtractor; ///< Function to extract a key from an entity.
					std::vector<Partition<KeyType>> m_partitions; ///< Vector of partitions.
					bool m_isDirty = true; ///< Flag indicating if partitions need rebuilding.
			};

			/**
			* @brief Reorders the group entities based on a new order.
			*
			* @param newOrder New order of entities.
			*/
			void reorderGroup(const std::vector<Entity>& newOrder)
			{
				std::apply([&](auto&&... arrays) {
					((reorderArray(arrays, newOrder)), ...);
				}, m_ownedArrays);
			}

			/**
			* @brief Reorders a single component array based on the new entity order.
			*
			* @tparam ArrayPtr Type of the component array pointer.
			* @param array Component array pointer.
			* @param newOrder New order of entities.
			*/
			template<typename ArrayPtr>
			void reorderArray(ArrayPtr array, const std::vector<Entity>& newOrder) const
			{
				size_t groupSize = array->groupSize();
				if (newOrder.size() != groupSize)
					THROW_EXCEPTION(InternalError, "New order size doesn't match group size");

				if (groupSize == 0)
					return;

				// Create a temporary storage for components
				using CompType = typename std::decay_t<decltype(*array)>::component_type;
				std::vector<CompType> tempComponents;
				tempComponents.reserve(groupSize);

				for (Entity e : newOrder)
					tempComponents.push_back(array->get(e)); //Maybe we should not push back, does it make a copy ?

				for (size_t i = 0; i < groupSize; i++) {
					Entity e = newOrder[i];
					array->forceSetComponentAt(i, e, std::move(tempComponents[i]));
				}
		}

			/**
			 * @brief Helper to dereference an entity and its components by index.
			 *
			 * @param index Index in the group.
			 * @return auto Tuple containing the entity and its owned component data.
			 */
			auto dereference(std::size_t index) const
			{
				Entity entity = std::get<0>(m_ownedArrays)->getEntityAtIndex(index);

				// Use std::forward_as_tuple to preserve references.
				auto ownedData = std::apply([entity](auto&&... arrays) {
					return std::forward_as_tuple(arrays->get(entity)...);
				}, m_ownedArrays);
				// We still need the entity by value, so use std::make_tuple for that.
				return std::tuple_cat(std::make_tuple(entity), ownedData);
			}

		 	/**
		 	 * @brief Helper: Recursively search the non‑owned tuple for the ComponentArray with component_type == T.
		 	 *
		 	 * @tparam T Component type.
		 	 * @tparam I Current index in the tuple.
		 	 * @return std::shared_ptr<ComponentArray<T>> Pointer to the component array.
		 	 */
			template<typename T, std::size_t I = 0>
	            requires (I < std::tuple_size_v<NonOwnedTuple>) // Ensure we don't go out of bounds
			auto getNonOwnedImpl() const -> std::shared_ptr<ComponentArray<T>>
			{
				using CurrentArrayPtr = std::tuple_element_t<I, NonOwnedTuple>;
				using CurrentComponent = typename std::decay_t<decltype(*std::declval<CurrentArrayPtr>())>::component_type;
				if constexpr (std::is_same_v<CurrentComponent, T>)
					return std::get<I>(m_nonOwnedArrays);
				else
					return getNonOwnedImpl<T, I + 1>();
			}

		 	/**
		 	 * @brief Helper: Recursively search the owned tuple for the ComponentArray with component_type == T.
		 	 *
		 	 * @tparam T Component type.
		 	 * @tparam I Current index in the tuple.
		 	 * @return std::shared_ptr<ComponentArray<T>> Pointer to the component array.
		 	 */
			template<typename T, std::size_t I = 0>
	            requires (I < std::tuple_size_v<OwnedTuple>) // Ensure we don't go out of bounds
			auto getOwnedImpl() const -> std::shared_ptr<ComponentArray<T>>
			{
				using CurrentArrayPtr = std::tuple_element_t<I, OwnedTuple>;
				using CurrentComponent = typename std::decay_t<decltype(*std::declval<CurrentArrayPtr>())>::component_type;
				if constexpr (std::is_same_v<CurrentComponent, T>)
					return std::get<I>(m_ownedArrays);
				else
					return getOwnedImpl<T, I + 1>();
			}

			/**
			 * @brief Helper function to call a function with component data.
			 *
			 * This function unpacks the owned and non‑owned component arrays.
			 *
			 * @tparam Func Callable type.
			 * @tparam I Indices for the owned tuple.
			 * @tparam J Indices for the non‑owned tuple.
			 * @param func Callable to invoke.
			 * @param e Entity.
			 * @param index_sequence for owned components.
			 * @param index_sequence for non‑owned components.
			 */
			template<typename Func, std::size_t... I, std::size_t... J>
			void callFunc(Func func, Entity e, std::index_sequence<I...>, std::index_sequence<J...>) const
			{
				func(e, (std::get<I>(m_ownedArrays)->get(e))..., (std::get<J>(m_nonOwnedArrays)->get(e))...);
			}

			/**
			* @brief Defines the direction for sorting operations
			*/
			enum class SortingOrder {
				ASCENDING,
				DESCENDING
			};

		    // Member variables
		    OwnedTuple m_ownedArrays;  ///< Tuple of pointers to owned component arrays.
		    NonOwnedTuple m_nonOwnedArrays;  ///< Tuple of pointers to non‑owned component arrays.
		    Signature      m_ownedSignature{}; ///< Signature for owned components.
		    Signature      m_allSignature{};   ///< Combined signature for all components.
			bool m_sortingInvalidated = true;    ///< Flag indicating if sorting is invalidated.
			SortingOrder m_sortingOrder = SortingOrder::ASCENDING;
   			std::unordered_map<std::string, std::unique_ptr<IPartitionStorage>> m_partitionStorageMap; ///< Map storing partition data by ID.

	};
}
