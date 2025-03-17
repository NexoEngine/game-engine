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

    const std::array<std::string, static_cast<int>(AssetType::_COUNT)> AssetTypeNames = {
        "Texture",
        "Model",
        "Sound",
        "Music",
        "Font",
        "Shader",
        "Script"
    };

    using AssetID = boost::uuids::uuid;

    enum class AssetStatus {
        UNLOADED,
        LOADED,
        ERROR
    };

    class AssetManager;
    class AssetCatalog;
    class AssetImporter;

    struct AssetMetadata {
        AssetType type;              //< Asset type
        AssetStatus status;          //< Asset status
        uint64_t referenceCount;     //< Number of references to the asset
        AssetID id;                  //< Unique identifier
        AssetManager *manager;       //< Pointer to the asset manager
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
                    .manager = nullptr,
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
        friend class AssetManager;
        friend class AssetCatalog;

        friend class AssetRef<TAssetData>;
        public:
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
