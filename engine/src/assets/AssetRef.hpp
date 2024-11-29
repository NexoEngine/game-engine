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
    class Asset;
    class AssetManager;

    /**
     * @brief Base class for all AssetRef classes, providing a static reference to the AssetManager.
     *
     * This class maintains a single static instance of the AssetManager pointer that is shared
     * across all AssetRef template instantiations. It provides protected access to the AssetManager
     * and handles the initialization of the manager reference.
     */
    class AssetRefBase {
        friend class AssetManager;
        protected:
            /** @brief Single static instance of AssetManager used by all asset references */
            inline static AssetManager* s_assetManager;

        private:
            /**
             * @brief Sets the global AssetManager instance
             * @param assetManager Pointer to the AssetManager instance
             * @warning Will log an error if attempting to set when an AssetManager already exists
             */
            static void setAssetManager(AssetManager* assetManager);
    };

    /**
     * @brief Template class representing a lightweight reference to an asset.
     * @tparam TAssetData The type of asset data being referenced
     *
     * AssetRef provides a lightweight wrapper around asset data that is managed by the AssetManager.
     * It offers convenient access to the asset data while ensuring proper resource management.
     * The class focuses on making the common case fast by providing direct pointer access to
     * asset data, while still maintaining safety through the AssetManager.
     */
    template<DerivedFromAsset TAssetData>
    class AssetRef {
        public:
            /**
             * @brief Constructs an AssetRef with the given asset data pointer
             * @param data Pointer to the asset data
             */
            explicit AssetRef(TAssetData* data) : m_data(data)
            {

            }

            /** @brief Copy constructor */
            AssetRef(const AssetRef& other) = default;

            /** @brief Copy assignment operator */
            AssetRef& operator=(const AssetRef& other) = default;

            /** @brief Move constructor */
            AssetRef(AssetRef&& other) noexcept = default;

            /** @brief Move assignment operator */
            AssetRef& operator=(AssetRef&& other) noexcept = default;

            /** @brief Destructor */
            ~AssetRef() = default;

            /**
             * @brief Gets the raw pointer to the asset data
             * @return Pointer to the asset data
             */
            [[nodiscard]] TAssetData* get() const
            {
                return m_data;
            }

            /**
             * @brief Arrow operator for accessing asset data members
             * @return Pointer to the asset data
             */
            TAssetData* operator->() const
            {
                return m_data;
            }

            /**
             * @brief Checks if the asset reference is valid
             * @return true if the reference points to valid data, false otherwise
             */
            [[nodiscard]] bool isValid() const
            {
                return m_data != nullptr;
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
            [[nodiscard]] static AssetRef<TAssetData> null()
            {
                return AssetRef<TAssetData>(nullptr);
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
            TAssetData* m_data; /**< Pointer to the actual asset data */
    };

    template<DerivedFromAsset TAssetData>
    bool AssetRef<TAssetData>::isLoaded() const
    {
        //m_data->getMetadata().status == AssetStatus::LOADED;
    }

    template<DerivedFromAsset TAssetData>
    void AssetRef<TAssetData>::reload()
    {
    }

    template<DerivedFromAsset TAssetData>
    void AssetRef<TAssetData>::unload()
    {
    }
} // namespace nexo::assets
