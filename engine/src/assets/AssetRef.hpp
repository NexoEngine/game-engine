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
//               by the AssetManager, and the AssetRef is a lightweight
//               reference to the asset.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Logger.hpp>
#include <memory>

namespace nexo::assets {
    class IAsset;

    template<typename TAssetData>
    class Asset;

    class AssetManager;

    /**
     * @brief Template class representing a lightweight reference to an asset.
     * @tparam TAsset The type of asset data being referenced
     *
     * AssetRef provides a thread-safe wrapper around asset data that is managed by the AssetManager.
     * This class never provides direct access to the underlying asset, ensuring thread-safety
     * and memory safety by requiring explicit locking for all operations.
     */
    template<typename TAsset>
    class AssetRef {
    public:
        /**
         * @brief Constructs an AssetRef with the given shared_ptr to asset
         * @param assetPtr Shared pointer to the asset
         */
        explicit AssetRef(const std::shared_ptr<TAsset>& assetPtr)
            : m_weakPtr(assetPtr) {}

        /**
         * @brief Default constructor creates a null reference
         */
        AssetRef() = default;

        /**
         * @brief Locks the asset reference, providing safe access
         * @return A shared_ptr to the asset, or empty shared_ptr if expired
         *
         * This is the only method for safely accessing an asset in a multithreaded environment.
         * The returned shared_ptr will keep the asset alive for as long as it exists.
         */
        [[nodiscard]] std::shared_ptr<TAsset> lock() const noexcept {
            return m_weakPtr.lock();
        }

        /**
         * @brief Checks if the asset reference is valid
         * @return true if the reference points to valid data, false otherwise
         *
         * Note: This is only a momentary check and may change in multithreaded
         * environments. Always use lock() for actual asset access.
         */
        [[nodiscard]] bool isValid() const noexcept {
            return !m_weakPtr.expired();
        }

        /**
         * @brief Checks if the asset is fully loaded
         * @return true if the asset is loaded, false otherwise
         */
        [[nodiscard]] bool isLoaded() const {
            if (auto ptr = lock()) {
                return ptr->isLoaded(); // Assumes TAsset has isLoaded() method
            }
            return false;
        }

        /**
         * @brief Requests the AssetCatalog to reload the asset
         */
        void reload() {
            if (auto ptr = lock()) {
                // TODO: Implement reloadAsset in AssetCatalog
                // Example: AssetCatalog::getInstance().reloadAsset(ptr);
                LOG(NEXO_WARN, "AssetRef::reload() not implemented");
            }
        }

        /**
         * @brief Requests the AssetCatalog to unload the asset but maintain the reference
         */
        void unload() {
            if (auto ptr = lock()) {
                // TODO: Implement unloadAsset in AssetCatalog
                // Example: AssetCatalog::getInstance().unloadAsset(ptr);
                LOG(NEXO_WARN, "AssetRef::unload() not implemented");
            }
        }

        /**
         * @brief Boolean conversion operator
         * @return true if the reference is valid, false otherwise
         *
         * Note: This is only a momentary check and may change in multithreaded
         * environments. Always use lock() for actual asset access.
         */
        explicit operator bool() const noexcept {
            return isValid();
        }

        /**
         * @brief Creates a null asset reference
         * @return An empty AssetRef instance
         */
        [[nodiscard]] static AssetRef<TAsset> null() {
            return AssetRef<TAsset>();
        }

        // Standard copy/move operations
        AssetRef(const AssetRef&) = default;
        AssetRef& operator=(const AssetRef&) = default;
        AssetRef(AssetRef&&) noexcept = default;
        AssetRef& operator=(AssetRef&&) noexcept = default;
        ~AssetRef() = default;

    private:
        std::weak_ptr<TAsset> m_weakPtr;
    };

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
         * @brief Construct from any asset reference type
         * @param ref The typed asset reference to convert
         */
        template<typename TAsset>
        explicit GenericAssetRef(const AssetRef<TAsset>& ref) : m_weakPtr(ref.lock()) {}

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
         * @brief Cast to a typed asset reference
         * @tparam TAsset The asset type to cast to
         * @return A typed AssetRef
         */
        template<typename TAsset>
        [[nodiscard]] AssetRef<TAsset> as() const {
            auto ptr = m_weakPtr.lock();
            if (!ptr) {
                return AssetRef<TAsset>::null();
            }
            return AssetRef<TAsset>(std::dynamic_pointer_cast<TAsset>(ptr));
        }

        /**
         * @brief Creates a null asset reference
         * @return An empty GenericAssetRef instance
         */
        [[nodiscard]] static GenericAssetRef null() {
            return {};
        }

    private:
        std::weak_ptr<void> m_weakPtr;
    };

} // namespace nexo::assets
