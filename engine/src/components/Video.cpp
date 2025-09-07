//// Video.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        05/09/2025
//  Description: Class Video
//
///////////////////////////////////////////////////////////////////////////////

#include <format>
#include <opencv2/opencv.hpp>
// #include <opencv2/videoio.hpp>
#include "Video.hpp"
#include "assets/AssetCatalog.hpp"

namespace nexo::components {
    bool VideoComponent::loadVideoFrames(const std::string &videoPath)
    {
        cv::VideoCapture cap(videoPath);

        if (!cap.isOpened()) {
            LOG_ONCE(NEXO_ERROR, "Erreur : impossible d'ouvrir la vidéo {}", videoPath);
            return false;
        }
        // int frameNumber = static_cast<int>(cap.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_COUNT)); // Get total
        // number of frames
        cv::Mat frame;
        int frameCount = 0;

        while (true) {
            cap >> frame;             // lire une frame
            if (frame.empty()) break; // fin de la vidéo

            // Sauvegarder une frame en image
            std::string filename = "frame_" + std::to_string(frameCount) + ".jpg";
            cv::imwrite(filename, frame);

            auto &catalog                                          = assets::AssetCatalog::getInstance();
            static const assets::AssetRef<assets::Texture> texture = catalog.createAsset<assets::Texture>(
                assets::AssetLocation(std::format("_internal::frame_{}@_internal", frameCount)), filename);

            components::Material material;
            material.albedoTexture = texture;
            material.albedoColor   = glm::vec4(1.0f); // White to show texture colors
            material.isOpaque      = false;
            material.shader        = "Albedo unshaded transparent";

            frameCount++;

            // TODO: set memory management for frames before pushing back
            frames.push_back(catalog.createAsset<assets::Material>(
                assets::AssetLocation(std::format("_internal::frame_mat_{}@_internal", frameCount)),
                std::make_unique<components::Material>(material)));
        }

        cap.release();

        path = videoPath;
        return true;
    }
} // namespace nexo::components
