/// AssetRef.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/11/2024
//  Description: Header file for the AssetRef class.
//               An AssetRef is a reference to an asset, allowing for easy
//               access to the asset's data. The AssetData memory is handled
//               by the AssetCatalog, and the AssetRef is a lightweight
//               reference to the asset.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Logger.hpp>
#include <memory>

namespace nexo::assets {
    class IAsset;

    enum class AssetType;

    template <typename TAssetData, AssetType TAssetType>
    class Asset;

    template <typename TAsset>
    class AssetRef;

    /**
     * @brief A non-templated asset reference for generic asset storage
     */
    class GenericAssetRef {
    public:
        /**
         * @brief Default constructor creates a null reference
         */
        GenericAssetRef() = default;

        /**
         * @brief Construct from a shared_ptr to an asset
         * @param ptr The shared pointer to the asset
         */
        explicit GenericAssetRef(const std::shared_ptr<IAsset>& ptr) : m_weakPtr(ptr) {}

        /**
         * @brief Check if the reference is valid
         * @return true if valid, false if expired
         */
        [[nodiscard]] bool isValid() const noexcept {
            return !m_weakPtr.expired();
        }

        /**
         * @brief Get a shared_ptr to the referenced asset
         * @return A shared_ptr to the asset, or nullptr if expired
         */
        /**
         * @brief Acquires a shared pointer to the referenced asset.
         *
         * This function attempts to lock the underlying weak pointer to obtain a shared pointer
         * to the asset. If the asset is still available, a valid shared pointer is returned;
         * otherwise, an empty shared pointer is provided.
         *
         * @return std::shared_ptr<IAsset> A shared pointer to the asset if available, or an empty pointer if not.
         */
        [[nodiscard]] std::shared_ptr<IAsset> lock() const noexcept {
            return m_weakPtr.lock();
        }

        /**
         * @brief Cast to a typed asset reference
         * @tparam TAsset The asset type to cast to
         * @return A typed AssetRef
         */
        template<typename TAsset>
        [[nodiscard]] class AssetRef<TAsset> as() const;  // Implemented below after AssetRef definition

        /**
         * @brief Boolean conversion operator
         * @return true if the reference is valid, false otherwise
         */
        explicit operator bool() const noexcept {
            return isValid();
        }

        /**
         * @brief Creates a null asset reference
         * @return An empty GenericAssetRef instance
         */
        [[nodiscard]] static GenericAssetRef null() {
            return {};
        }

        /**
         * @brief Requests that the AssetCatalog load the referenced asset.
         *
         * This function attempts to lock the asset pointer and, if the asset is valid,
         * initiates a load request via the AssetCatalog. Note that the actual loading
         * functionality is currently pending implementation.
         */
        void load() {
            if (auto ptr = lock()) {
                // TODO: Implement reloadAsset in AssetCatalog
                // Example: AssetCatalog::getInstance().reloadAsset(ptr);
                LOG(NEXO_WARN, "AssetRef::reload() not implemented");
            }
        }

        /**
         * @brief Unloads the asset while retaining its reference.
         *
         * If the asset is valid (i.e., can be locked), this method attempts to initiate an unload
         * operation via the AssetCatalog. Currently, the unload functionality is not implemented,
         * and a warning is logged instead.
         *
         * @note The asset reference is maintained regardless of the unload operation.
         */
        void unload() {
            if (auto ptr = lock()) {
                // TODO: Implement unloadAsset in AssetCatalog
                // Example: AssetCatalog::getInstance().unloadAsset(ptr);
                LOG(NEXO_WARN, "AssetRef::unload() not implemented");
            }
        }


        /**
 * @brief Copy constructor.
 *
 * Constructs a new GenericAssetRef by copying the internal state of the provided instance,
 * ensuring that the asset reference remains shared.
 */
        GenericAssetRef(const GenericAssetRef&) = default;
        /**
 * @brief Default copy assignment operator.
 *
 * Copies the asset reference state from the provided instance using the compiler-generated implementation.
 *
 * @param other The GenericAssetRef instance to copy from.
 * @return Reference to this updated instance.
 */
GenericAssetRef& operator=(const GenericAssetRef&) = default;
        /**
 * @brief Move constructs a new GenericAssetRef by transferring the resources from another instance.
 *
 * The moved-from object remains in a valid but unspecified state.
 *
 * @param other The GenericAssetRef instance to move from.
 */
GenericAssetRef(GenericAssetRef&&) noexcept = default;
        /**
 * @brief Move assignment operator.
 *
 * Transfers ownership of the underlying asset reference from the given rvalue to this instance.
 * This operator uses the default implementation and is marked noexcept.
 *
 * @return A reference to this object.
 */
GenericAssetRef& operator=(GenericAssetRef&&) noexcept = default;
        /**
 * @brief Virtual destructor for GenericAssetRef.
 *
 * This default virtual destructor guarantees that any resources held by
 * derived classes are correctly released upon object destruction.
 */
virtual ~GenericAssetRef() = default;

    protected:
        std::weak_ptr<IAsset> m_weakPtr;
    };

    /**
     * @brief Template class representing a lightweight reference to an asset.
     * @tparam TAsset The type of asset data being referenced
     *
     * AssetRef provides a thread-safe wrapper around asset data that is managed by the AssetCatalog.
     * This class never provides direct access to the underlying asset, ensuring thread-safety
     * and memory safety by requiring explicit locking for all operations.
     */
    template<typename TAsset>
    class AssetRef final : public GenericAssetRef {
    public:
        /**
         * @brief Default constructor creates a null reference
         */
        AssetRef() = default;

        /**
             * @brief Constructs a typed AssetRef from a shared pointer to an asset.
             *
             * This constructor initializes an AssetRef with the provided shared pointer,
             * enabling type-safe asset management. If the given pointer is null, the reference
             * will represent a null asset.
             *
             * @param assetPtr Shared pointer to an asset of type TAsset.
             */
        explicit AssetRef(const std::shared_ptr<TAsset>& assetPtr)
            : GenericAssetRef(assetPtr) {}

        /**
         * @brief Locks the asset reference, providing safe access
         * @return A shared_ptr to the asset, or empty shared_ptr if expired
         */
        // ReSharper disable once CppHidingFunction
        [[nodiscard]] std::shared_ptr<TAsset> lock() const noexcept;

        /**
         * @brief Determines whether the referenced asset is fully loaded.
         *
         * This method attempts to lock the asset reference and, if successful, returns the result
         * of the asset's isLoaded() method. If the reference is invalid, the asset is considered
         * not loaded and the function returns false.
         *
         * @return true if the asset is valid and fully loaded; false otherwise.
         */
        [[nodiscard]] bool isLoaded() const {
            if (auto ptr = lock()) {
                return ptr->isLoaded(); // Assumes TAsset has isLoaded() method
            }
            return false;
        }

        /**
         * @brief Creates a null asset reference
         * @return An empty AssetRef instance
         */
        [[nodiscard]] static AssetRef<TAsset> null() {
            return AssetRef<TAsset>();
        }
    };

    template<typename TAsset>
    /**
     * @brief Converts a generic asset reference to a typed asset reference.
     *
     * This method attempts to lock the weak pointer to obtain a shared pointer to the asset. If the asset is unavailable,
     * it returns a null typed reference; otherwise, it dynamically casts the asset pointer to the specified type.
     *
     * @tparam TAsset The asset type to which the reference is cast.
     * @return AssetRef<TAsset> A typed asset reference, or a null reference if the asset could not be locked.
     */
    AssetRef<TAsset> GenericAssetRef::as() const {
        const auto ptr = m_weakPtr.lock();
        if (!ptr) {
            return AssetRef<TAsset>::null();
        }
        return AssetRef<TAsset>(std::dynamic_pointer_cast<TAsset>(ptr));
    }

    template<typename TAsset>
    /**
     * @brief Retrieves a type-safe shared pointer to the asset.
     *
     * This method locks the underlying generic asset reference and casts the resulting
     * shared pointer to the specific asset type TAsset. If the asset reference is invalid,
     * the returned shared pointer will be null.
     *
     * @return std::shared_ptr<TAsset> Shared pointer to the asset of type TAsset, or null if unavailable.
     */
    std::shared_ptr<TAsset> AssetRef<TAsset>::lock() const noexcept
    {
        return std::static_pointer_cast<TAsset>(GenericAssetRef::lock());
    }
} // namespace nexo::assets
