//// Name.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/06/2025
//  Description: Header file for the name component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "assets/Assets/Material/Material.hpp"

namespace nexo::components {

    enum class KeyframeType {
        NORMAL,
        LOOP,
        TRANSITION
    };

    struct VideoKeyframe {
        double start;
        double end;
        KeyframeType keyframeType;
    };

    struct VideoComponent {
        std::string path;
        std::vector<assets::AssetRef<assets::Material>> frames;
        double frameRate          = 30.0f; // Frames per second
        size_t currentFrameIndex = 0;
        size_t nbFrame;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;

        size_t currentKeyframeIndex = 0;
        std::vector<VideoKeyframe> keyframes = {
            {0.0f, 1.0f, KeyframeType::TRANSITION},
            {1.0f, 4.0f, KeyframeType::LOOP},
            {4.0f, 5.0f, KeyframeType::NORMAL},
            {5.0f, 6.0f, KeyframeType::TRANSITION},
            {6.0f, 9.0f, KeyframeType::NORMAL},
            {9.0f, 12.0f, KeyframeType::LOOP},
            {4.0f, 13.0f, KeyframeType::NORMAL}
        };

        bool loopVideo = false;
        bool isLoaded    = false;

        [[nodiscard]] bool loadVideoFrames(const std::string& videoPath);

        void updateFrame();

        void setCurrentKeyframe(size_t keyframeIndex);
        void setCurrentFrame(size_t frameIndex);

        void skipToNextKeyframe();
        void skipToPreviousKeyframe();
        void restartVideo();
    };
} // namespace nexo::components
