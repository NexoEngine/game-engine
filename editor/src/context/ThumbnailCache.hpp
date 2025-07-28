//// ThumbnailCache.hpp ///////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
    /**
     * @class ThumbnailCache
     * @brief Singleton managing thumbnail textures for various asset types.
     *
     * Caches generated thumbnails for materials, textures, and other assets.
     * Provides methods to retrieve, update, and clear thumbnails efficiently.
     */
    class ThumbnailCache {
        public:
            /**
            * @brief Retrieve the global instance of the ThumbnailCache singleton.
            * @return Reference to the single ThumbnailCache instance.
            */
            static ThumbnailCache& getInstance();

            ThumbnailCache(const ThumbnailCache&) = delete;
            ThumbnailCache& operator=(const ThumbnailCache&) = delete;
            ThumbnailCache(ThumbnailCache&&) = delete;
            ThumbnailCache& operator=(ThumbnailCache&&) = delete;

            /**
             * @brief Get or generate a thumbnail for a generic asset.
             *
             * Detects asset type and dispatches to the appropriate thumbnail generator.
             * @param assetRef Generic reference to the asset.
             * @param size Desired thumbnail size in pixels (width, height).
             * @return Texture ID of the thumbnail, or 0 if invalid.
             */
            unsigned int getThumbnail(const assets::GenericAssetRef& assetRef,
                                const glm::vec2& size = {128, 128});

            /**
             * @brief Get or generate a thumbnail for a material asset.
             *
             * Caches thumbnails per material ID and size.
             * @param materialRef Reference to the material asset.
             * @param size Desired thumbnail size in pixels (width, height).
             * @return Texture ID of the material thumbnail, or 0 if invalid.
             */
            unsigned int getMaterialThumbnail(const assets::AssetRef<assets::Material>& materialRef,
                                            const glm::vec2& size = {128, 128});

            /**
             * @brief Force regeneration of a material thumbnail.
             *
             * Updates the cached thumbnail even if one exists for the given size.
             * @param materialRef Reference to the material asset.
             * @param size Desired thumbnail size in pixels (width, height).
             * @return OpenGL texture ID of the updated thumbnail, or 0 if invalid.
             */
            unsigned int updateMaterialThumbnail(const assets::AssetRef<assets::Material> &materialRef,
                                                const glm::vec2 &size = {128, 128});

            /**
             * @brief Get or generate a thumbnail for a texture asset.
             *
             * Uses the texture directly if already loaded; otherwise generates a preview.
             * @param textureRef Reference to the texture asset.
             * @param size Desired thumbnail size in pixels (width, height).
             * @return OpenGL texture ID of the texture thumbnail, or 0 if invalid.
             */
            unsigned int getTextureThumbnail(const assets::AssetRef<assets::Texture>& textureRef,
                                            const glm::vec2& size = {128, 128});

            /**
             * @brief Remove all cached thumbnails and free associated resources.
             */
            void clearCache();

            /**
             * @brief Remove a specific thumbnail from the cache by asset ID.
             * @param assetId UUID of the asset whose thumbnail should be removed.
             */
            void removeThumbnail(const boost::uuids::uuid& assetId);

            /**
             * @brief Check if a thumbnail exists in the cache for a given asset ID.
             * @param assetId UUID of the asset.
             * @return True if a thumbnail is cached, false otherwise.
             */
            bool hasThumbnail(const boost::uuids::uuid& assetId) const;
        private:
            ThumbnailCache() = default;
            ~ThumbnailCache();

            /**
             * @brief Internal helper to generate a material thumbnail.
             *
             * Creates a preview scene with a cube, applies the material,
             * renders to a framebuffer, and caches the result.
             * @param materialRef Reference to the material asset.
             * @param size Desired render target size.
             * @return OpenGL texture ID of the generated thumbnail.
             */
            unsigned int createMaterialThumbnail(const assets::AssetRef<assets::Material>& materialRef,
                                            const glm::vec2& size);

            /**
             * @brief Internal helper to generate a texture thumbnail.
             *
             * Caches the texture directly if loaded or creates a ScenePreview if needed.
             * @param textureRef Reference to the texture asset.
             * @param size Desired thumbnail size.
             * @return OpenGL texture ID of the generated thumbnail.
             */
            unsigned int createTextureThumbnail(const assets::AssetRef<assets::Texture>& textureRef,
                                            const glm::vec2& size);

            struct ThumbnailInfo {
                std::shared_ptr<renderer::NxFramebuffer> framebuffer;
                unsigned int textureId;
                glm::vec2 size;
                utils::ScenePreviewOut previewInfo;
                bool isScenePreview; // true if using a scene preview, false if direct texture
            };

            std::unordered_map<boost::uuids::uuid, ThumbnailInfo> m_thumbnailCache;
    };
}
