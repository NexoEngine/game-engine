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
            {0.0f, 0.1f, KeyframeType::NORMAL},
            {0.1f, 1.25f, KeyframeType::TRANSITION},
            {1.25f, 16.15f, KeyframeType::LOOP},
            {16.15f, 17.0f, KeyframeType::TRANSITION},
            {17.0f, 37.23f, KeyframeType::LOOP},
            {37.23f, 38.07f, KeyframeType::TRANSITION},
            {38.07f, 82.27f, KeyframeType::LOOP},
            {82.27, 84.25f, KeyframeType::TRANSITION},
            {84.25f, 85.0f, KeyframeType::LOOP},
            {85.0f, 87.03f, KeyframeType::TRANSITION},
            {87.03f, 95.18f, KeyframeType::LOOP},
            {95.18f, 97.18f, KeyframeType::TRANSITION},
            {97.18f, 99.17f, KeyframeType::TRANSITION},
            {99.17f, 128.11f, KeyframeType::LOOP},
            {128.11f, 130.08f, KeyframeType::TRANSITION},
            {130.08f, 137.22f, KeyframeType::LOOP},
            {137.22f, 139.16f, KeyframeType::TRANSITION},
            {139.16f, 150.00f, KeyframeType::LOOP},
            {150.00f, 152.13f, KeyframeType::TRANSITION},
            {152.13f, 183.21f, KeyframeType::LOOP},
            {183.21f, 185.26f, KeyframeType::TRANSITION},
            {185.26f, 202.19f, KeyframeType::LOOP},
            {202.19f, 204.25f, KeyframeType::TRANSITION},
            {204.25f, 232.05f, KeyframeType::LOOP},
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
