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
#include "json.hpp"

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
        MATERIAL,
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
    constexpr const char *AssetTypeNames[] = {
        "UNKNOWN",
        "TEXTURE",
        "MATERIAL",
        "MODEL",
        "SOUND",
        "MUSIC",
        "FONT",
        "SHADER",
        "SCRIPT"
    };

    static_assert(
        static_cast<int>(AssetType::_COUNT) == std::size(AssetTypeNames),
        "AssetTypeNames array size must match AssetType enum size"
    );

    /**
     * @brief Retrieves the name corresponding to the specified asset type.
     *
     * This function returns a string literal from the AssetTypeNames array that matches the provided asset type.
     *
     * @param type The asset type value.
     * @return const char* The name of the asset type.
     */
    constexpr const char *getAssetTypeName(AssetType type) {
        return AssetTypeNames[static_cast<int>(type)];
    }

    /**
     * @brief Serializes an AssetType value to JSON.
     *
     * Converts the provided AssetType enum into its string representation using getAssetTypeName 
     * and assigns this string to the JSON object.
     *
     * @param j JSON object to receive the serialized asset type.
     * @param type The AssetType enum value to convert.
     */
    inline void to_json(nlohmann::json& j, AssetType type) {
        j = getAssetTypeName(type);
    }

    /**
     * @brief Converts a JSON value to its corresponding AssetType.
     *
     * This function iterates over the predefined asset type names and, if the JSON value matches one,
     * assigns the corresponding AssetType enum value to the output parameter. If no match is found,
     * the AssetType is set to UNKNOWN.
     *
     * @param j JSON object containing the asset type as a string.
     * @param type Output parameter to store the resulting AssetType.
     */
    inline void from_json(const nlohmann::json& j, AssetType& type) {
        for (int i = 0; i < static_cast<int>(AssetType::_COUNT); ++i) {
            if (j == AssetTypeNames[i]) {
                type = static_cast<AssetType>(i);
                return;
            }
        }
        type = AssetType::UNKNOWN;
    }

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
            //IAsset() = delete;
            virtual ~IAsset() = default;

            [[nodiscard]] virtual const AssetMetadata& getMetadata() const { return m_metadata; }
            [[nodiscard]] virtual AssetType getType() const { return getMetadata().type; }
            [[nodiscard]] virtual AssetID getID() const { return getMetadata().id; }
            [[nodiscard]] virtual AssetStatus getStatus() const { return getMetadata().status; }

            [[nodiscard]] virtual bool isLoaded() const { return getStatus() == AssetStatus::LOADED; }
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
            using AssetDataType = TAssetData;
            static constexpr AssetType TYPE = TAssetType;

            /**
             * @brief Destructor that releases the allocated asset data.
             *
             * Deletes the dynamically allocated asset data to ensure proper memory cleanup when the asset is destroyed.
             */
            virtual ~Asset() override
            {
                delete data;
            }

            TAssetData *data;

            // Implementation of IAsset virtual methods
            [[nodiscard]] void* getRawData() const override {
                return data;
            }

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

            [[nodiscard]] TAssetData* getData() const {
                return data;
            }

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
            explicit Asset() : data(nullptr)
            {
                m_metadata.type = TAssetType;
            }

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
