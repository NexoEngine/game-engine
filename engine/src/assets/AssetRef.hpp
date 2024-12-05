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

#include "AssetConcepts.hpp"

namespace nexo::assets {

    template<typename TAssetData>
    class Asset;

    class AssetManager;

    /**
     * @brief Template class representing a lightweight reference to an asset.
     * @tparam TAsset The type of asset data being referenced
     *
     * AssetRef provides a lightweight wrapper around asset data that is managed by the AssetManager.
     * It offers convenient access to the asset data while ensuring proper resource management.
     * The class focuses on making the common case fast by providing direct pointer access to
     * asset data, while still maintaining safety through the AssetManager.
     */
    template<typename TAssetData>
    class AssetRef {
        using TAsset = Asset<TAssetData>;

        public:
            /**
             * @brief Constructs an AssetRef with the given asset data pointer
             * @param data Pointer to the asset data
             */
            explicit AssetRef(TAsset* data) : m_asset(data)
            {
                if (!data)
                    return;
                ++m_asset->getMetadata().referenceCount;
            }

            ~AssetRef()
            {
                if (!m_asset)
                    return;
                --m_asset->getMetadata().referenceCount;
            }

            /** @brief Copy constructor */
            AssetRef(const AssetRef& other)
            {
                m_asset = other.m_asset;
                ++m_asset->m_metadata.referenceCount;
            }

            /** @brief Copy assignment operator */
            AssetRef& operator=(const AssetRef& other)
            {
                if (this == &other)
                    return *this;

                if (m_asset)
                    --m_asset->m_metadata.referenceCount;

                m_asset = other.m_asset;
                ++m_asset->m_metadata.referenceCount;

                return *this;
            }

            /** @brief Move constructor */
            AssetRef(AssetRef&& other) noexcept
            {
                m_asset = other.m_asset;
                other.m_asset = nullptr;
            }

            /** @brief Move assignment operator */
            AssetRef& operator=(AssetRef&& other) noexcept
            {
                if (this == &other)
                    return *this;

                m_asset = other.m_asset;
                other.m_asset = nullptr;

                return *this;
            }

            /**
             * @brief Gets the raw pointer to the asset data
             * @return Pointer to the asset data
             */
            [[nodiscard]] TAssetData* get() const noexcept
            {
                return m_asset;
            }

            /**
             * @brief Arrow operator for accessing asset data members
             * @return Pointer to the asset data
             */
            TAsset* operator->() const noexcept
            {
                return this->get();
            }

            /**
             * @brief Dereference operator for accessing asset data members
             * @return Reference to the asset data
             */
            TAsset& operator*() const noexcept
            {
                return *this->get();
            }

            /**
             * @brief Checks if the asset reference is valid
             * @return true if the reference points to valid data, false otherwise
             */
            [[nodiscard]] bool isValid() const
            {
                return m_asset != nullptr;
            }

            /**
             * @brief Checks if the asset is fully loaded
             * @return true if the asset is loaded, false otherwise
             */
            [[nodiscard]] bool isLoaded() const;

            /** @brief Requests the AssetManager to reload the asset */
            void reload();

            /** @brief Requests the AssetManager to unload the asset but maintain the reference */
            void unload();

            /**
             * @brief Creates a null asset reference
             * @return An AssetRef instance with a null data pointer
             */
            [[nodiscard]] static AssetRef<TAsset> null()
            {
                return AssetRef<TAsset>(nullptr);
            }

            /**
             * @brief Boolean conversion operator
             * @return true if the reference is valid, false otherwise
             */
            explicit operator bool() const
            {
                return isValid();
            }

        private:
            TAsset* m_asset; /**< Pointer to the actual asset */
    };

    class GenericAssetRef : public AssetRef<void> {};

} // namespace nexo::assets
