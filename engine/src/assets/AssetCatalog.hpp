//// AssetCatalog.hpp /////////////////////////////////////////////////////////
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
//  Date:        03/12/2024
//  Description: Header file for the AssetCatalog class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <unordered_map>

#include "Asset.hpp"
#include "AssetImporter.hpp"
#include "AssetLocation.hpp"
#include "assets/AssetRef.hpp"

namespace nexo::assets {

    /**
    * @class AssetCatalog
    *
    * @brief Singleton class that holds all the assets in the engine.
    */
    class AssetCatalog {
        protected:
            // Singleton: protected constructor and destructor
            /**
             * @brief Default constructor for AssetCatalog.
             *
             * Constructs an AssetCatalog instance using the default initializer.
             * The constructor is protected to allow instantiation by derived classes,
             * while still supporting the singleton pattern.
             */
            AssetCatalog() = default;

            /**
             * @brief Default destructor for AssetCatalog.
             *
             * The default destructor relies on compiler-generated behavior to clean up the instance,
             * ensuring that all managed assets are properly released.
             */
            ~AssetCatalog() = default;

        public:
            // Singleton: Meyers' Singleton Pattern
            static AssetCatalog& getInstance()
            {
                static AssetCatalog s_instance;
                return s_instance;
            }

            // Singleton: delete copy constructor and assignment operator
            AssetCatalog(AssetCatalog const&)   = delete;
            void operator=(AssetCatalog const&) = delete;

            /**
             * @brief Removes the asset associated with the given ID from the catalog.
             *
             * Checks if an asset with the specified ID exists in the catalog and, if so,
             * deletes it.
             *
             * @param id The unique identifier of the asset to be removed.
             */
            void deleteAsset(AssetID id);

            /**
             * @brief Delete an asset from the catalog.
             * @param asset The asset to delete.
             */
            void deleteAsset(const GenericAssetRef& asset);

            /**
             * @brief Moves an asset to another location.
             * @param asset The asset to move.
             * @param path The new location for the asset.
             */
            void moveAsset(const GenericAssetRef &asset, const std::string &path) const;

            /**
             * @brief Moves an asset to another location.
             * @param id The ID of the asset to move.
             * @param path The new location for the asset.
             */
            void moveAsset(AssetID id, const std::string &path) const;


            /**
             * @brief Get an asset by its ID.
             * @param id The ID of the asset to get.
             * @return A reference to the asset, or a null reference if the asset does not exist.
             */
            [[nodiscard]] GenericAssetRef getAsset(AssetID id) const;

            /**
             * @brief Get an asset by its location.
             * @param location The location of the asset to get.
             * @return A reference to the asset, or a null reference if the asset does not exist.
             */
            [[nodiscard]] GenericAssetRef getAsset(const AssetLocation& location) const;

            /**
             * @brief Retrieves all asset references registered in the catalog.
             *
             * Iterates through the stored assets and collects them into a vector of GenericAssetRef objects.
             *
             * @return A vector containing a GenericAssetRef for each managed asset.
             */
            [[nodiscard]] std::vector<GenericAssetRef> getAssets() const;

            /**
             * @brief Retrieves a view of all assets in the catalog.
             *
             * This function returns a view of the asset collection, where each asset is transformed into a
             * GenericAssetRef using C++20 ranges. This lightweight view facilitates efficient iteration over
             * the registered assets.
             *
             * @return A view of the assets with each element represented as a GenericAssetRef.
             */
            [[nodiscard]] auto getAssetsView() const
            {
                return m_assets
                    | std::views::values
                    | std::views::transform([](const auto& asset) {
                        return GenericAssetRef(asset);
                    });
            }

            /**
             * @brief Get all assets of a specific type in the catalog.
             * @tparam AssetType The type of asset to get. (e.g. Model, Texture)
             * @return A vector of all assets of the specified type in the catalog.
             */
            template <IsAsset AssetType>
            [[nodiscard]] std::vector<AssetRef<AssetType>> getAssetsOfType() const;

            /**
             * @brief Get all assets of a specific type in the catalog as a view.
             * @tparam AssetType The type of asset to get. (e.g. Model, Texture)
             * @return A view of all assets of the specified type in the catalog.
             */
            template <IsAsset AssetType>
            [[nodiscard]] decltype(auto) getAssetsOfTypeView() const;

            /**
             * @brief Registers an asset in the catalog.
             *
             * This method verifies that the provided asset pointer is valid, then creates a shared pointer
             * to the asset. It updates the asset's metadata by setting its location and assigning a new unique
             * identifier if one is not already set. The asset is stored in the catalog and a reference to the
             * asset is returned.
             *
             * @warning Once registered, the memory for the asset is managed by the catalog. Do not delete the asset.
             *
             * @param location The asset's location metadata.
             * @param asset Pointer to the asset to be registered.
             * @return GenericAssetRef A reference to the registered asset, or a null reference if the asset pointer was null.
             */
            GenericAssetRef registerAsset(const AssetLocation& location, std::unique_ptr<IAsset> asset);

            /**
             * @brief Creates and registers a new asset in the catalog.
             *
             * This method creates a new asset of the specified type, registers it in the catalog, and returns a reference to it.
             *
             * @tparam AssetType The type of asset to create. Must be derived from IAsset.
             * @param location The asset's location metadata.
             * @param args Constructor arguments for the asset.
             * @return AssetRef<AssetType> A reference to the created and registered asset.
             */
            template <IsAsset AssetType, typename... Args>
            AssetRef<AssetType> createAsset(const AssetLocation& location, Args&& ...args)
            {
                auto asset = std::make_unique<AssetType>(std::forward<Args>(args)...);
                auto assetRef = registerAsset(location, std::move(asset));
                return assetRef.template as<AssetType>();
            }

            template <IsAsset AssetType>
            AssetRef<AssetType> createAsset(const AssetLocation& location, std::unique_ptr<typename AssetType::AssetDataType> assetData)
            {
                auto asset = std::make_unique<AssetType>();
                asset->setData(std::move(assetData));
                auto assetRef = registerAsset(location, std::move(asset));
                return assetRef.template as<AssetType>();
            }



        private:
            std::unordered_map<AssetID, std::shared_ptr<IAsset>> m_assets;
    };

    template<IsAsset AssetType>
    std::vector<AssetRef<AssetType>> AssetCatalog::getAssetsOfType() const
    {
        // TODO: AssetType::TYPE is not a thing, need to find a way to get the type of the asset
        static_assert(true, "Filtering not implemented yet");
        std::vector<AssetRef<AssetType>> assets;
        for (const auto& asset: m_assets | std::views::values) {
            if (asset->getType() == AssetType::TYPE)
                assets.emplace_back(std::static_pointer_cast<AssetType>(asset));
        }
        return assets;
    }

    template<IsAsset AssetType>
    decltype(auto) AssetCatalog::getAssetsOfTypeView() const
    {
        // TODO: AssetType::TYPE is not a thing, need to find a way to get the type of the asset
        static_assert(true, "Filtering not implemented yet");
        return m_assets
               | std::views::values
               | std::views::filter([](const auto& asset) {
                   return asset->getType() == AssetType::TYPE;
               })
               | std::views::transform([](const auto& asset) {
                   return std::static_pointer_cast<AssetType>(asset);
               });
    }

} // namespace nexo::assets
