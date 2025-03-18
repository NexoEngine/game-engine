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
 * @brief Virtual destructor for the IAsset interface.
 *
 * Ensures that derived asset objects are cleaned up properly when deleted via base class pointers.
 * This defaulted destructor supports polymorphic deletion without introducing additional cleanup logic.
 */
            virtual ~IAsset() = default;

            /**
 * @brief Retrieves the asset's metadata.
 *
 * Returns a constant reference to the asset's metadata, which includes details such as asset type, status,
 * reference count, unique identifier, and location.
 *
 * @return A constant reference to the asset's metadata.
 */
[[nodiscard]] virtual const AssetMetadata& getMetadata() const { return m_metadata; }
            /**
 * @brief Retrieves the type of the asset.
 *
 * Returns the asset type stored in the metadata.
 *
 * @return AssetType The asset's type.
 */
[[nodiscard]] virtual AssetType getType() const { return getMetadata().type; }
            /**
 * @brief Retrieves the asset's unique identifier.
 *
 * This method returns the asset ID from the asset's metadata.
 *
 * @return AssetID Unique identifier associated with the asset.
 */
[[nodiscard]] virtual AssetID getID() const { return getMetadata().id; }
            /**
 * @brief Retrieves the asset's current status.
 *
 * This function returns the asset's current status as indicated in its metadata,
 * reflecting whether the asset is unloaded, loaded, or in an error state.
 *
 * @return AssetStatus The current status of the asset.
 */
[[nodiscard]] virtual AssetStatus getStatus() const { return getMetadata().status; }

            /**
 * @brief Determines whether the asset has been successfully loaded.
 *
 * This method checks if the asset's status is set to AssetStatus::LOADED, indicating that the asset data is available.
 *
 * @return true if the asset is loaded; false otherwise.
 */
[[nodiscard]] virtual bool isLoaded() const { return getStatus() == AssetStatus::LOADED; }
            /**
 * @brief Determines whether the asset is in an error state.
 *
 * Evaluates if the asset's status equals AssetStatus::ERROR.
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
             * @brief Constructs an IAsset instance with default metadata.
             *
             * Initializes the asset's metadata to default values:
             * - Asset type is set to UNKNOWN.
             * - Asset status is set to UNLOADED.
             * - Reference count is initialized to 0.
             * - Asset ID is set to a nil UUID.
             * - Location is set to "default".
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
             * @brief Destroys the asset, releasing any allocated asset data.
             *
             * Deletes the dynamically allocated data associated with the asset to prevent memory leaks.
             */
            virtual ~Asset() override
            {
                delete data;
            }

            TAssetData *data;

            /**
             * @brief Retrieves the raw asset data.
             *
             * Returns a pointer to the underlying raw data managed by the asset.
             *
             * @return void* Pointer to the asset's raw data.
             */
            [[nodiscard]] void* getRawData() const override {
                return data;
            }

            /**
             * @brief Sets the raw asset data.
             *
             * Deletes any previously stored asset data and assigns new data from the provided pointer.
             * Updates the asset's status to UNLOADED if the pointer is null, or to LOADED if non-null.
             * Ownership of the data is transferred to the asset.
             *
             * @param rawData Pointer to the new asset data.
             * @return IAsset& A reference to the updated asset.
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
             * @brief Retrieves the underlying asset data.
             *
             * This function returns a pointer to the asset's data. The returned pointer is owned by the asset and
             * should not be deleted or altered outside of the asset's management.
             *
             * @return TAssetData* Pointer to the asset data.
             */
            [[nodiscard]] TAssetData* getData() const {
                return data;
            }

            /**
             * @brief Replaces the current asset data with a new pointer and updates the asset's status.
             *
             * Deletes the previously held asset data and assigns the new data pointer. If the new pointer is non-null,
             * the asset status is set to LOADED; otherwise, it is set to UNLOADED. Ownership of the new data pointer is transferred.
             *
             * @param newData Pointer to the new asset data; a null pointer unloads the asset.
             * @return Reference to this asset instance.
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
             * @brief Constructs an Asset instance with null asset data.
             *
             * This explicit constructor initializes the asset by setting its data pointer to nullptr and
             * assigning the asset type from the template parameter TAssetType to the asset's metadata.
             */
            explicit Asset() : data(nullptr)
            {
                m_metadata.type = TAssetType;
            }

            /**
             * @brief Constructs an asset with the provided asset data.
             *
             * This constructor initializes the asset by taking ownership of the given data pointer,
             * setting the asset type based on the compile-time template parameter, and marking the asset
             * as LOADED in its metadata.
             *
             * @param data Pointer to the asset data.
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
