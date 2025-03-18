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
         * @brief Acquires a shared pointer to the asset.
         *
         * Locks the internal weak pointer to obtain a shared pointer that references the asset.
         * If the asset has expired, this function returns a null shared pointer.
         *
         * @return std::shared_ptr<IAsset> A shared pointer to the asset, or nullptr if the asset is no longer available.
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
         * @brief Requests the associated asset to be loaded.
         *
         * If the asset reference is valid, this function attempts to initiate the loading
         * process through the AssetCatalog. Currently, the actual loading functionality
         * is not implemented, and a warning is logged instead. If the asset reference is
         * expired, the function performs no action.
         */
        void load() {
            if (auto ptr = lock()) {
                // TODO: Implement reloadAsset in AssetCatalog
                // Example: AssetCatalog::getInstance().reloadAsset(ptr);
                LOG(NEXO_WARN, "AssetRef::reload() not implemented");
            }
        }

        /**
         * @brief Requests the AssetCatalog to unload the asset while retaining its reference.
         *
         * If the asset is valid, this method intends to initiate an unload request via the AssetCatalog.
         * Currently, the unload operation is not implemented and a warning is logged.
         *
         * @note Implementation of AssetCatalog::unloadAsset is required to enable asset unloading.
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
 * Constructs a new GenericAssetRef instance as a copy of an existing reference, using the default member-wise copy.
 */
        GenericAssetRef(const GenericAssetRef&) = default;
        /**
 * @brief Default copy assignment operator.
 *
 * Copies the internal state from another GenericAssetRef so that both objects refer to the same asset.
 *
 * @return A reference to this instance.
 */
GenericAssetRef& operator=(const GenericAssetRef&) = default;
        /**
 * @brief Move constructs a new GenericAssetRef by transferring resources from the given reference.
 *
 * The move constructor transfers ownership of the underlying weak pointer, leaving the source in a valid but unspecified state.
 */
GenericAssetRef(GenericAssetRef&&) noexcept = default;
        /**
 * @brief Move-assignment operator.
 *
 * Transfers the asset reference from the given temporary instance to this object,
 * leaving the moved-from object in a valid but unspecified state. This operator
 * is noexcept and uses the default, member-wise move assignment.
 *
 * @param rhs The GenericAssetRef to move from.
 * @return A reference to this instance.
 */
GenericAssetRef& operator=(GenericAssetRef&&) noexcept = default;
        /**
 * @brief Virtual destructor for GenericAssetRef.
 *
 * Declared as default, it enables safe polymorphic deletion of derived asset reference objects.
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
         * @brief Determines whether the referenced asset is fully loaded.
         *
         * This function attempts to obtain a shared pointer from the asset reference. If successful,
         * it returns the result of the asset's isLoaded() method. If the asset reference is expired,
         * it returns false.
         *
         * Note: Assumes that the underlying asset type implements an isLoaded() method.
         *
         * @return true if the asset is valid and fully loaded, false otherwise.
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
     * @brief Casts the generic asset reference to a typed asset reference.
     *
     * Attempts to lock the underlying weak asset pointer and casts it to the specified type
     * using a dynamic pointer cast. If the asset is no longer valid (i.e. the pointer has expired),
     * returns a null typed asset reference.
     *
     * @tparam TAsset The target asset type.
     * @return AssetRef<TAsset> A typed asset reference if the asset is valid; otherwise, a null reference.
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
     * @brief Returns a shared pointer to the asset cast to type TAsset.
     *
     * Attempts to lock the underlying generic asset reference and casts it to TAsset.
     * If the asset is not available, returns a null pointer.
     *
     * @return std::shared_ptr<TAsset> A shared pointer to the asset if valid; otherwise, nullptr.
     */
    std::shared_ptr<TAsset> AssetRef<TAsset>::lock() const noexcept
    {
        return std::static_pointer_cast<TAsset>(GenericAssetRef::lock());
    }
} // namespace nexo::assets
