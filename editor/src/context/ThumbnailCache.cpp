//// ThumbnailCache.cpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/06/2025
//  Description: Source file for the thumbnail cache
//
///////////////////////////////////////////////////////////////////////////////

#include "ThumbnailCache.hpp"
#include "Application.hpp"
#include "EntityFactory3D.hpp"
#include "Nexo.hpp"
#include "assets/Assets/Material/Material.hpp"
#include "components/Camera.hpp"
#include "Framebuffer.hpp"

namespace nexo::editor {

    ThumbnailCache& ThumbnailCache::getInstance()
    {
        static ThumbnailCache instance;
        return instance;
    }

    ThumbnailCache::~ThumbnailCache()
    {
        clearCache();
    }


    unsigned int ThumbnailCache::getThumbnail(const assets::GenericAssetRef& assetRef,
                                           const glm::vec2& size)
    {
        if (!assetRef.isValid())
            return 0;


        auto asset = assetRef.lock();
        if (!asset)
            return 0;

        switch (asset->getType()) {
            case assets::AssetType::MATERIAL: {
                auto materialRef = assetRef.as<assets::Material>();
                return getMaterialThumbnail(materialRef, size);
            }
            case assets::AssetType::TEXTURE: {
                auto textureRef = assetRef.as<assets::Texture>();
                return getTextureThumbnail(textureRef, size);
            }
            case assets::AssetType::MODEL:
                // Model thumbnails not currently implemented
                return 0;
            default:
                // Unsupported asset type
                return 0;
        }
    }

    unsigned int ThumbnailCache::getMaterialThumbnail(const assets::AssetRef<assets::Material>& materialRef,
                                                    const glm::vec2& size)
    {
        if (!materialRef.isValid())
            return 0;

        auto material = materialRef.lock();
        if (!material || !material->getData())
            return 0;

        const boost::uuids::uuid& assetId = material->getID();

        // Check if thumbnail exists at requested size
        auto it = m_thumbnailCache.find(assetId);
        if (it != m_thumbnailCache.end() && it->second.size == size)
            return it->second.framebuffer->getColorAttachmentId(0);

        if (it != m_thumbnailCache.end()) {
            // Clean up existing thumbnail
            removeThumbnail(assetId);
        }

        return createMaterialThumbnail(materialRef, size);
    }

    unsigned int ThumbnailCache::updateMaterialThumbnail(const assets::AssetRef<assets::Material> &materialRef,
                                        const glm::vec2 &size)
    {
        if (!materialRef.isValid())
            return 0;

        auto material = materialRef.lock();
        if (!material || !material->getData())
            return 0;

        return createMaterialThumbnail(materialRef, size);
    }

    unsigned int ThumbnailCache::getTextureThumbnail(const assets::AssetRef<assets::Texture>& textureRef,
                                                   const glm::vec2& size) {
        if (!textureRef.isValid())
            return 0;

        auto texture = textureRef.lock();
        if (!texture)
            return 0;

        const boost::uuids::uuid& assetId = texture->getID();

        auto it = m_thumbnailCache.find(assetId);
        if (it != m_thumbnailCache.end() && it->second.size == size)
            return it->second.textureId;

        if (it != m_thumbnailCache.end())
            removeThumbnail(assetId);

        return createTextureThumbnail(textureRef, size);
    }

    unsigned int ThumbnailCache::createMaterialThumbnail(const assets::AssetRef<assets::Material>& materialRef,
                                                       const glm::vec2& size)
    {
        auto material = materialRef.lock();
        if (!material || !material->getData())
            return 0;

        utils::ScenePreviewOut previewInfo;

        const components::Material& materialData = *material->getData();
        ecs::Entity previewEntity = nexo::EntityFactory3D::createCube(
            glm::vec3(0.0f),         // position
            glm::vec3(1.0f),         // size
            glm::vec3(30.0f, 45.0f, 0.0f), // rotation - angled for better lighting
            materialData             // material
        );

        utils::genScenePreview("Material_Thumbnail", size, previewEntity, previewInfo);

        auto& app = getApp();

        const Application::SceneInfo sceneInfo{previewInfo.sceneId, nexo::RenderingType::FRAMEBUFFER};
        app.run(sceneInfo);
        auto& cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(previewInfo.cameraId);

        auto framebuffer = cameraComponent.m_renderTarget;
        app.getSceneManager().deleteScene(previewInfo.sceneId);
        ThumbnailInfo info;
        info.framebuffer = framebuffer;
        info.size = size;
        info.previewInfo = previewInfo;
        info.isScenePreview = false;

        m_thumbnailCache[material->getID()] = info;

        previewInfo.sceneGenerated = false;

        return framebuffer->getColorAttachmentId(0);
    }

    unsigned int ThumbnailCache::createTextureThumbnail(const assets::AssetRef<assets::Texture>& textureRef,
                                                      const glm::vec2& size)
    {
        auto texture = textureRef.lock();
        if (!texture)
            return 0;

        // For textures, we can often just use the texture directly
        if (texture->isLoaded() && texture->getData()) {
            ThumbnailInfo info;
            info.textureId = texture->getData().get()->texture->getId();
            info.size = size;
            info.isScenePreview = false;

            m_thumbnailCache[texture->getID()] = info;
            return info.textureId;
        }

        return 0;
    }

    void ThumbnailCache::clearCache()
    {
        auto& app = getApp();

        // Clean up all scene previews
        for (auto& [assetId, info] : m_thumbnailCache) {
            if (info.isScenePreview && info.previewInfo.sceneGenerated) {
                app.getSceneManager().deleteScene(info.previewInfo.sceneId);
                info.previewInfo.sceneGenerated = false;
            }
        }

        m_thumbnailCache.clear();
    }

    void ThumbnailCache::removeThumbnail(const boost::uuids::uuid& assetId)
    {
        auto it = m_thumbnailCache.find(assetId);
        if (it == m_thumbnailCache.end())
            return;

        // If it's a scene preview, clean it up
        if (it->second.isScenePreview && it->second.previewInfo.sceneGenerated) {
            auto& app = getApp();
            app.getSceneManager().deleteScene(it->second.previewInfo.sceneId);
            it->second.previewInfo.sceneGenerated = false;
        }

        m_thumbnailCache.erase(it);
    }

    bool ThumbnailCache::hasThumbnail(const boost::uuids::uuid& assetId) const
    {
        return m_thumbnailCache.find(assetId) != m_thumbnailCache.end();
    }
}
