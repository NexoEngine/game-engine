//// ThumbnailCache.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/06/2025
//  Description: Header file for the thumbnail cache
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Framebuffer.hpp"
#include "assets/AssetRef.hpp"
#include "assets/Assets/Material/Material.hpp"
#include "utils/ScenePreview.hpp"

namespace nexo::editor {
    class ThumbnailCache {
    public:
        // Singleton instance getter
        static ThumbnailCache& getInstance();

        // Delete copy and move constructors/operators
        ThumbnailCache(const ThumbnailCache&) = delete;
        ThumbnailCache& operator=(const ThumbnailCache&) = delete;
        ThumbnailCache(ThumbnailCache&&) = delete;
        ThumbnailCache& operator=(ThumbnailCache&&) = delete;

        unsigned int getThumbnail(const assets::GenericAssetRef& assetRef,
                               const glm::vec2& size = {128, 128});

        // Get thumbnail for material (creates if not exists)
        unsigned int getMaterialThumbnail(const assets::AssetRef<assets::Material>& materialRef,
                                         const glm::vec2& size = {128, 128});

        // Get thumbnail for texture (creates if not exists)
        unsigned int getTextureThumbnail(const assets::AssetRef<assets::Texture>& textureRef,
                                        const glm::vec2& size = {128, 128});

        // Clear all cached thumbnails
        void clearCache();

        // Remove a specific thumbnail
        void removeThumbnail(const boost::uuids::uuid& assetId);

        // Check if thumbnail exists
        bool hasThumbnail(const boost::uuids::uuid& assetId) const;

    private:
        // Private constructor for singleton
        ThumbnailCache() = default;
        ~ThumbnailCache();

        // Helper methods to create thumbnails
        unsigned int createMaterialThumbnail(const assets::AssetRef<assets::Material>& materialRef,
                                          const glm::vec2& size);

        unsigned int createTextureThumbnail(const assets::AssetRef<assets::Texture>& textureRef,
                                         const glm::vec2& size);

        // Cache storage
        struct ThumbnailInfo {
            std::shared_ptr<renderer::NxFramebuffer> framebuffer;
            unsigned int textureId;
            glm::vec2 size;
            utils::ScenePreviewOut previewInfo;
            bool isScenePreview; // true if using a scene preview, false if direct texture
        };

        // Map of asset ID to thumbnail info
        std::unordered_map<boost::uuids::uuid, ThumbnailInfo> m_thumbnailCache;

        // Mutex for thread safety
        mutable std::mutex m_cacheMutex;
    };
}
