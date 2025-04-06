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
 * @brief Constructs a GenericAssetRef from a shared asset pointer.
 *
 * Initializes the asset reference by converting the provided shared pointer into a weak pointer.
 * This allows the asset's lifetime to be monitored without owning it, enabling safe retrieval via lock().
 *
 * @param ptr The shared pointer to the asset.
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
         * @brief Locks the asset reference and returns a shared pointer.
         *
         * This function attempts to lock the weak pointer to the asset, returning a shared pointer
         * if the asset is still valid. If the asset reference has expired, it returns an empty shared pointer.
         *
         * @return A shared pointer to the asset, or an empty pointer if the asset is not valid.
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
         * @brief Requests the AssetCatalog to initiate loading of the asset.
         *
         * If the asset reference is valid (i.e., not expired), this function is intended to trigger
         * a reload or load operation via the AssetCatalog. Currently, the loading functionality
         * is not implemented, and a warning is logged instead.
         */
        void load() {
            if (auto ptr = lock()) {
                // TODO: Implement reloadAsset in AssetCatalog
                // Example: AssetCatalog::getInstance().reloadAsset(ptr);
                LOG(NEXO_WARN, "AssetRef::reload() not implemented");
            }
        }

        /**
         * @brief Requests the AssetCatalog to unload the referenced asset while retaining the reference.
         *
         * If the asset is valid, this method attempts to obtain the shared pointer (using lock()) and then issues
         * an unload request through the AssetCatalog. Currently, the unload operation is not implemented, so a warning
         * is logged instead.
         *
         * @note The actual unloading functionality via AssetCatalog is pending implementation.
         */
        void unload() {
            if (auto ptr = lock()) {
                // TODO: Implement unloadAsset in AssetCatalog
                // Example: AssetCatalog::getInstance().unloadAsset(ptr);
                LOG(NEXO_WARN, "AssetRef::unload() not implemented");
            }
        }


        /**
 * @brief Copy constructs a GenericAssetRef.
 *
 * Constructs a new GenericAssetRef by copying the internal state from the provided instance.
 */
        GenericAssetRef(const GenericAssetRef&) = default;
        /**
 * @brief Default copy assignment operator.
 *
 * Copies the asset reference from another GenericAssetRef instance.
 *
 * @param other The GenericAssetRef to copy from.
 * @return A reference to this GenericAssetRef.
 */
GenericAssetRef& operator=(const GenericAssetRef&) = default;
        /**
 * @brief Move constructs a GenericAssetRef by transferring ownership.
 *
 * Constructs a new GenericAssetRef from an rvalue reference by transferring the internal asset reference.
 * The moved-from object remains in a valid but unspecified state.
 */
GenericAssetRef(GenericAssetRef&&) noexcept = default;
        /**
 * @brief Move assignment operator for GenericAssetRef.
 *
 * Transfers ownership of the internal reference from another GenericAssetRef to this instance.
 * This operation is noexcept and leaves the moved-from object in a valid but unspecified state.
 *
 * @param other The GenericAssetRef to move from.
 * @return A reference to this instance.
 */
GenericAssetRef& operator=(GenericAssetRef&&) noexcept = default;
        /**
 * @brief Virtual destructor for GenericAssetRef.
 *
 * Ensures that destructors for derived classes are called correctly when deleting through a pointer 
 * to GenericAssetRef.
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
         * @brief Constructs an AssetRef with the given shared_ptr to asset
         * @param assetPtr Shared pointer to the asset
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
         * @brief Determines if the asset is fully loaded.
         *
         * This method locks the underlying asset reference and checks its loaded status by invoking
         * the asset's own isLoaded() method. If the asset reference has expired or is invalid, the method
         * returns false.
         *
         * @return true if the asset is valid and reported as loaded; false otherwise.
         */
        [[nodiscard]] bool isLoaded() const {
            if (auto ptr = lock()) {
                return ptr->isLoaded(); // Assumes TAsset has isLoaded() method
            }
            return false;
        }

        /**
         * @brief Returns a null asset reference.
         *
         * Creates and returns a type-specific AssetRef instance that does not reference any asset.
         * This null asset reference is equivalent to a default-constructed instance and will evaluate as false.
         *
         * @return A null AssetRef instance.
         */
        [[nodiscard]] static AssetRef<TAsset> null() {
            return AssetRef<TAsset>();
        }
    };

    template<typename TAsset>
    /**
     * @brief Converts a GenericAssetRef into a type-safe AssetRef.
     *
     * This function attempts to lock the underlying weak pointer to obtain a shared pointer
     * to the asset. If successful, it performs a dynamic cast to convert the asset pointer
     * to type TAsset and returns an AssetRef<TAsset> encapsulating it. Otherwise, it returns
     * a null AssetRef<TAsset>.
     *
     * @tparam TAsset The asset type to cast to.
     * @return AssetRef<TAsset> A type-safe reference to the asset if available, or a null reference otherwise.
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
     * @brief Locks the asset reference and returns a type-safe shared pointer.
     *
     * This method attempts to obtain a shared pointer from the underlying generic asset reference
     * and casts it to the specified asset type TAsset. If the asset reference is expired or invalid,
     * the returned pointer will be nullptr.
     *
     * @return std::shared_ptr<TAsset> A shared pointer to the asset, or nullptr if the reference is not valid.
     */
    std::shared_ptr<TAsset> AssetRef<TAsset>::lock() const noexcept
    {
        return std::static_pointer_cast<TAsset>(GenericAssetRef::lock());
    }
} // namespace nexo::assets
