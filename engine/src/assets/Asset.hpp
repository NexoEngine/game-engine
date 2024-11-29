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

namespace nexo::assets {

    enum class AssetType {
        TEXTURE,
        MODEL,
        SOUND,
        MUSIC,
        FONT,
        SHADER,
        SCRIPT
    };

    using AssetID = boost::uuids::uuid;

    enum class AssetStatus {
        LOADED,
        UNLOADED,
        ERROR
    };

    class AssetManager;

    struct AssetMetadata {
        AssetType type;              //< Asset type
        AssetStatus status;          //< Asset status
        uint64_t referenceCount;     //< Number of references to the asset
        AssetID id;                  //< Unique identifier
        AssetManager *manager;       //< Pointer to the asset manager
    };

    class Asset {
        friend class AssetManager;
        friend class AssetRefBase;
        public:
            virtual ~Asset() = default;

            [[nodiscard]] const AssetMetadata& getMetadata() const { return m_metadata; }
            [[nodiscard]] AssetType getType() const { return getMetadata().type; }
            [[nodiscard]] AssetID getID() const { return getMetadata().id; }
            [[nodiscard]] AssetStatus getStatus() const { return getMetadata().status; }
        protected:
            explicit Asset(AssetType type)
                : m_metadata({
                    .type = type,
                    .status = AssetStatus::UNLOADED,
                    .referenceCount = 0,
                    .id = boost::uuids::nil_uuid(),
                    .manager = nullptr
                })
            {
            }

        private:
            AssetMetadata m_metadata;

            /**
             * @brief Get the metadata of the asset (for modification)
             */
            [[nodiscard]] AssetMetadata& getMetadata() { return m_metadata; }

            virtual AssetStatus load() = 0;
            virtual AssetStatus unload() = 0;

    };

    class TextureAsset : public Asset {
        public:
            TextureAsset() : Asset(AssetType::TEXTURE)
            {
            }

        private:

    };

} // namespace nexo::editor
