//// Asset.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        18/11/2024
//  Description: Header file for the Asset class, base class for all assets
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <filesystem>
#include <Texture.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/basic_random_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/random_generator.hpp>

#include "AssetLocation.hpp"
#include "AssetRef.hpp"

namespace nexo::assets {

    constexpr unsigned short ASSET_MAX_DEPENDENCIES = 10000;

    /**
     * @brief Asset types
     * @note The order of the enum is important, as it is used to index into the AssetTypeNames array.
     *       Make sure to update the array if you add new asset types.
     */
    enum class AssetType {
        UNKNOWN,
        TEXTURE,
        MODEL,
        SOUND,
        MUSIC,
        FONT,
        SHADER,
        SCRIPT,
        _COUNT
    };

    /**
     * @brief Array of asset type names
     * @note The order of the array must match the order of the AssetType enum.
     */
    const std::array<std::string, static_cast<int>(AssetType::_COUNT)> AssetTypeNames = {

        "Texture",
        "Model",
        "Sound",
        "Music",
        "Font",
        "Shader",
        "Script"
    };

    /**
     * @brief Asset ID type
     * @note This is a UUID that uniquely identifies an asset. Alias of boost::uuids::uuid.
     */
    using AssetID = boost::uuids::uuid;

    enum class AssetStatus {
        UNLOADED,
        LOADED,
        ERROR
    };

    class AssetCatalog;
    class AssetImporter;

    struct AssetMetadata {
        AssetType type;              //< Asset type
        AssetStatus status;          //< Asset status
        uint64_t referenceCount;     //< Number of references to the asset
        AssetID id;                  //< Unique identifier
        AssetLocation location;      //< Location of the asset
    };

    class IAsset {
        friend class AssetCatalog;
        friend class AssetImporter;
        public:
            /**
 * @brief Virtual default destructor for the asset interface.
 *
 * Ensures proper cleanup of derived asset classes during polymorphic destruction.
 */
            virtual ~IAsset() = default;

            /**
 * @brief Retrieves the asset's metadata.
 *
 * Returns a constant reference to the asset's metadata, which contains information
 * such as asset type, status, and unique identifier.
 *
 * @return const AssetMetadata& A constant reference to the asset's metadata.
 */
[[nodiscard]] virtual const AssetMetadata& getMetadata() const { return m_metadata; }
            /**
 * @brief Retrieves the type of the asset.
 *
 * This function returns the asset type from its metadata, indicating the specific category of the asset.
 *
 * @return AssetType The type of the asset.
 */
[[nodiscard]] virtual AssetType getType() const { return getMetadata().type; }
            /**
 * @brief Retrieves the unique identifier of the asset.
 *
 * Accesses the asset's metadata and returns its unique ID.
 *
 * @return AssetID The unique identifier of the asset.
 */
[[nodiscard]] virtual AssetID getID() const { return getMetadata().id; }
            /**
 * @brief Retrieves the asset's current status.
 *
 * This function returns the asset's loading status by accessing its metadata,
 * providing insight into whether the asset is loaded, unloaded, or in an error state.
 *
 * @return AssetStatus representing the current status of the asset.
 */
[[nodiscard]] virtual AssetStatus getStatus() const { return getMetadata().status; }

            /**
 * @brief Determines if the asset is loaded.
 *
 * This method returns true if the asset's status is set to LOADED, indicating that it has been successfully loaded.
 *
 * @return bool True if the asset is loaded; otherwise, false.
 */
[[nodiscard]] virtual bool isLoaded() const { return getStatus() == AssetStatus::LOADED; }
            /**
 * @brief Determines whether the asset is in an error state.
 *
 * This function returns true if the asset's current status is set to AssetStatus::ERROR, indicating
 * that an error has occurred with the asset.
 *
 * @return bool True if the asset is errored, false otherwise.
 */
[[nodiscard]] virtual bool isErrored() const { return getStatus() == AssetStatus::ERROR; }

            /**
             * @brief Get the asset data pointer
             * @return Raw pointer to the asset data
             */
            [[nodiscard]] virtual void* getRawData() const = 0;

            /**
             * @brief Set the asset data pointer
             * @param rawData Raw pointer to the asset data
             * @note This transfers ownership of the data to the asset, which will delete it in its destructor
             */
            virtual IAsset& setRawData(void* rawData) = 0;
        protected:
            /**
             * @brief Constructs an IAsset and initializes its metadata with default values.
             *
             * This constructor sets up the asset's metadata to a known default state:
             * - Asset type is set to `AssetType::UNKNOWN`.
             * - Asset status is set to `AssetStatus::UNLOADED`.
             * - Reference count is initialized to 0.
             * - Asset ID is set to a nil UUID.
             * - Asset location is set to `"default"`.
             */
            explicit IAsset()
                : m_metadata({
                    .type = AssetType::UNKNOWN,
                    .status = AssetStatus::UNLOADED,
                    .referenceCount = 0,
                    .id = boost::uuids::nil_uuid(),
                    .location = AssetLocation("default"),
                })
            {
            }

        public:
            AssetMetadata m_metadata;

            /**
             * @brief Get the metadata of the asset (for modification)
             */
            //[[nodiscard]] AssetMetadata& getMetadata() { return m_metadata; }

            /*virtual AssetStatus load() = 0;
            virtual AssetStatus unload() = 0;*/

    };

    template<typename TAssetData, AssetType TAssetType>
    class Asset : public IAsset {
        friend class AssetCatalog;

        friend class AssetRef<TAssetData>;
        public:
            /**
             * @brief Destructor that cleans up the asset's data.
             *
             * Releases the allocated memory for the asset data by deleting the pointer.
             */
            virtual ~Asset() override
            {
                delete data;
            }

            TAssetData *data;

            /**
             * @brief Retrieves the raw data pointer associated with the asset.
             *
             * This method returns a void pointer to the underlying asset data, enabling direct access.
             *
             * @return void* Pointer to the asset's raw data.
             */
            [[nodiscard]] void* getRawData() const override {
                return data;
            }

            /**
             * @brief Sets the raw data pointer for the asset and updates its load status.
             *
             * This method replaces the current asset data with the provided raw data pointer.
             * It deletes any previously allocated asset data to prevent memory leaks and then updates
             * the asset's status: if the new pointer is null, the status is set to UNLOADED; otherwise, it
             * is set to LOADED. The raw data pointer is cast to the appropriate asset data type before assignment.
             *
             * @param rawData Pointer to the new raw asset data.
             * @return IAsset& Reference to the current asset instance, enabling method chaining.
             */
            IAsset& setRawData(void* rawData) override {
                delete data;  // Clean up existing data
                if (rawData == nullptr) {
                    m_metadata.status = AssetStatus::UNLOADED;
                } else {

                    m_metadata.status = AssetStatus::LOADED;
                }
                data = static_cast<TAssetData*>(rawData);
                return *this;
            }

            /**
             * @brief Retrieves the asset's typed data.
             *
             * This function returns the pointer to the asset data of type TAssetData held by the asset instance.
             *
             * @return TAssetData* Pointer to the asset data.
             */
            [[nodiscard]] TAssetData* getData() const {
                return data;
            }

            /**
             * @brief Sets the asset's data and updates its load status.
             *
             * Replaces the current data pointer by deleting any previously allocated asset data.
             * Updates the asset status to UNLOADED when the new data is nullptr, or to LOADED otherwise.
             *
             * @param newData Pointer to the new asset data.
             * @return Reference to the updated asset instance.
             */
            Asset& setData(TAssetData* newData) {
                delete data;
                if (newData == nullptr) {
                    m_metadata.status = AssetStatus::UNLOADED;
                } else {
                    m_metadata.status = AssetStatus::LOADED;
                }
                data = newData;
                return *this;
            }

        protected:
            /**
             * @brief Constructs an asset with no initial data.
             *
             * This constructor initializes the asset's data pointer to nullptr and sets the asset's metadata type
             * to the template parameter TAssetType.
             */
            explicit Asset() : data(nullptr)
            {
                m_metadata.type = TAssetType;
            }

            /**
             * @brief Constructs an Asset with the provided asset data.
             *
             * Initializes the asset using the given data pointer, sets the asset type to the templated value, 
             * and marks the asset's status as LOADED.
             *
             * @param data Pointer to the asset-specific data that the asset will manage.
             */
            explicit Asset(TAssetData* data) : data(data)
            {
                m_metadata.type = TAssetType;
                m_metadata.status = AssetStatus::LOADED;
            }

        private:

            /*virtual AssetStatus load() = 0;
            virtual AssetStatus unload() = 0;*/

    };

} // namespace nexo::editor
