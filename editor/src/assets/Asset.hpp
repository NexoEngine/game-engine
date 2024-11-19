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
#include <boost/uuid/uuid.hpp>

namespace nexo::editor {

    enum class AssetType {
        TEXTURE,
        MODEL,
        SOUND,
        MUSIC,
        FONT,
        SHADER,
        SCRIPT
    };

    using AssetUUID = boost::uuids::uuid;

    class Asset {
        public:
            virtual ~Asset() = default;

            [[nodiscard]] AssetType getType() const;
            [[nodiscard]] AssetUUID getUUID() const;
            void setID(AssetUUID id);

        protected:
            explicit Asset(AssetType type) : m_type(type) {}

            const AssetType m_type;
            AssetUUID m_uuid;
    };

} // namespace nexo::editor
