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
        //const size_t byteCount    = tmp.total() * tmp.elemSize();

        //std::vector<uint8_t> buffer(byteCount);
        //std::memcpy(buffer.data(), tmp.data, byteCount);

        renderer::NxTextureFormat format;
        if (tmp.channels() == 1)
            format = renderer::NxTextureFormat::R8;
        else if (tmp.channels() == 3)
            format = renderer::NxTextureFormat::RGB8;
        else
            format = renderer::NxTextureFormat::RGBA8;

        return std::make_unique<assets::Texture>(tmp.data, width, height, format);
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
        isLoaded = true;
        return true;
    }

    void VideoComponent::updateFrame()
    {
        auto secondsPassed = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - lastFrameTime);

        auto &currentKeyframe = keyframes[currentKeyframeIndex];
        if (secondsPassed.count() >= (currentKeyframe.end - currentKeyframe.start)) {
            switch (currentKeyframe.keyframeType) {
                case KeyframeType::LOOP:
                    lastFrameTime = std::chrono::high_resolution_clock::now();
                    setCurrentFrame(static_cast<size_t>(currentKeyframe.start * frameRate));
                    break;
                case KeyframeType::TRANSITION:
                    skipToNextKeyframe();
                    break;
                default:
                    //lastFrameTime = std::chrono::high_resolution_clock::now();
                    setCurrentFrame(static_cast<size_t>(currentKeyframe.end * frameRate));
                    break;
            }
            return;
        }

        setCurrentFrame(static_cast<size_t>(secondsPassed.count() * frameRate + currentKeyframe.start * frameRate));
    }

    void VideoComponent::setCurrentKeyframe(size_t keyframeIndex)
    {
        if (keyframeIndex >= keyframes.size()) {
            if (loopVideo) {
                currentKeyframeIndex = 0;
            } else {
                currentKeyframeIndex = keyframes.size() - 1;
            }
            return;
        }
        currentKeyframeIndex = keyframeIndex;
        auto &keyframe       = keyframes[currentKeyframeIndex];
        currentFrameIndex   = static_cast<size_t>(keyframe.start * frameRate);
        lastFrameTime       = std::chrono::high_resolution_clock::now();
    }

    void VideoComponent::setCurrentFrame(size_t frameIndex)
    {
        if (frameIndex >= nbFrame) {
            if (loopVideo) {
                currentFrameIndex = 0;
            } else {
                currentFrameIndex = nbFrame - 1;
            }
            return;
        }
        currentFrameIndex = frameIndex;
    }

    void VideoComponent::skipToNextKeyframe()
    {
        setCurrentKeyframe(currentKeyframeIndex + 1);
    }

    void VideoComponent::skipToPreviousKeyframe()
    {
        if (currentKeyframeIndex == 0) {
            if (loopVideo)
                setCurrentKeyframe(keyframes.size() - 1);
            return;
        }
        auto &previousKeyframe = keyframes[currentKeyframeIndex - 1];
        if (previousKeyframe.keyframeType == KeyframeType::TRANSITION) {
            if (currentKeyframeIndex < 2) {
                if (loopVideo)
                    setCurrentKeyframe(keyframes.size() - 1);
                return;
            }
            setCurrentKeyframe(currentKeyframeIndex - 2);
            return;
        }
        setCurrentKeyframe(currentKeyframeIndex - 1);
    }

    void VideoComponent::restartVideo()
    {
        setCurrentKeyframe(0);
    }
} // namespace nexo::components
