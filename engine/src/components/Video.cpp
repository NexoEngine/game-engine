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

#include "Video.hpp"
#include <format>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include "assets/AssetCatalog.hpp"

namespace nexo::components {

    static std::unique_ptr<assets::Texture> createTextureFromMat(const cv::Mat &frame)
    {
        cv::Mat tmp;

        if (frame.channels() == 3) {
            cv::cvtColor(frame, tmp, cv::COLOR_BGR2RGB);
        } else if (frame.channels() == 4) {
            cv::cvtColor(frame, tmp, cv::COLOR_BGRA2RGBA);
        } else {
            tmp = frame.clone();
        }

        cv::flip(tmp, tmp, 0);

        if (!tmp.isContinuous()) tmp = tmp.clone();

        const unsigned int width  = static_cast<unsigned int>(tmp.cols);
        const unsigned int height = static_cast<unsigned int>(tmp.rows);
        const size_t byteCount    = tmp.total() * tmp.elemSize();

        std::vector<uint8_t> buffer(byteCount);
        std::memcpy(buffer.data(), tmp.data, byteCount);

        renderer::NxTextureFormat format;
        if (tmp.channels() == 1)
            format = renderer::NxTextureFormat::R8;
        else if (tmp.channels() == 3)
            format = renderer::NxTextureFormat::RGB8;
        else
            format = renderer::NxTextureFormat::RGBA8;

        return std::make_unique<assets::Texture>(buffer.data(), width, height, format);
    }

    bool VideoComponent::loadVideoFrames(const std::string &videoPath)
    {
        cv::VideoCapture cap(videoPath);

        if (!cap.isOpened()) {
            LOG_ONCE(NEXO_ERROR, "Failed to open the video {}", videoPath);
            return false;
        }
        int frameNumber = static_cast<int>(cap.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_COUNT)); // Get total
        frameRate       = cap.get(cv::VideoCaptureProperties::CAP_PROP_FPS);                           // Get FPS

        cv::Mat frame;
        frames.reserve(frameNumber);
        size_t frameCount = 0;
        auto &catalog     = assets::AssetCatalog::getInstance();

        while (true) {
            cap >> frame;
            if (frame.empty()) break;

            auto texturePtr = createTextureFromMat(frame);

            auto texture = catalog.registerAsset(
                assets::AssetLocation(std::format("_internal::frame_{}@_internal", frameCount)), std::move(texturePtr));

            components::Material material;
            material.albedoTexture = texture.as<assets::Texture>();
            material.albedoColor   = glm::vec4(1.0f); // White to show texture colors
            material.isOpaque      = false;
            material.shader        = "Albedo unshaded transparent";

            frames.push_back(catalog.createAsset<assets::Material>(
                assets::AssetLocation(std::format("_internal::frame_mat_{}@_internal", frameCount)),
                std::make_unique<components::Material>(material)));

            ++frameCount;
        }

        cap.release();

        path    = videoPath;
        nbFrame = frameCount;
        return true;
    }
} // namespace nexo::components
