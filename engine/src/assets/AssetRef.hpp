/// AssetRef.hpp /////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
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

        [[nodiscard]] bool operator==(const GenericAssetRef& other) const noexcept {
            return m_weakPtr.lock() == other.m_weakPtr.lock();
        }

        [[nodiscard]] bool operator!=(const GenericAssetRef& other) const noexcept {
            return !(*this == other);
        }

        [[nodiscard]] bool operator==(const std::nullptr_t) const noexcept {
            return !isValid();
        }

        [[nodiscard]] bool operator!=(const std::nullptr_t) const noexcept {
            return isValid();
        }

        /**
         * @brief Get a shared_ptr to the referenced asset
         * @return A shared_ptr to the asset, or nullptr if expired
         */
        // ReSharper disable once CppHiddenFunction
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
         * @brief Requests the AssetCatalog to load the asset
         */
        void load() const {
            if (auto ptr = lock()) {
                // TODO: Implement reloadAsset in AssetCatalog
                // Example: AssetCatalog::getInstance().reloadAsset(ptr);
                LOG(NEXO_WARN, "AssetRef::reload() not implemented");
            }
        }

        /**
         * @brief Requests the AssetCatalog to unload the asset but maintain the reference
         */
        void unload() const {
            if (auto ptr = lock()) {
                // TODO: Implement unloadAsset in AssetCatalog
                // Example: AssetCatalog::getInstance().unloadAsset(ptr);
                LOG(NEXO_WARN, "AssetRef::unload() not implemented");
            }
        }


        // Standard copy/move operations
        GenericAssetRef(const GenericAssetRef&) = default;
        GenericAssetRef& operator=(const GenericAssetRef&) = default;
        GenericAssetRef(GenericAssetRef&&) noexcept = default;
        GenericAssetRef& operator=(GenericAssetRef&&) noexcept = default;
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

        explicit(false) AssetRef(std::nullptr_t) : GenericAssetRef(nullptr) {}

        /**
         * @brief Locks the asset reference, providing safe access
         * @return A shared_ptr to the asset, or empty shared_ptr if expired
         */
        // ReSharper disable once CppHidingFunction
        [[nodiscard]] std::shared_ptr<TAsset> lock() const noexcept;

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
         * @brief Creates a null asset reference
         * @return An empty AssetRef instance
         */
        [[nodiscard]] static AssetRef<TAsset> null() {
            return AssetRef<TAsset>();
        }
    };

    template<typename TAsset>
    AssetRef<TAsset> GenericAssetRef::as() const {
        const auto ptr = m_weakPtr.lock();
        if (!ptr) {
            return AssetRef<TAsset>::null();
        }
        return AssetRef<TAsset>(std::dynamic_pointer_cast<TAsset>(ptr));
    }

    template<typename TAsset>
    std::shared_ptr<TAsset> AssetRef<TAsset>::lock() const noexcept
    {
        return std::static_pointer_cast<TAsset>(GenericAssetRef::lock());
    }
} // namespace nexo::assets
