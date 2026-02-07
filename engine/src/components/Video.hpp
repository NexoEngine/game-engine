//// Name.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        24/06/2025
//  Description: Header file for the video component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "assets/Assets/Material/Material.hpp"

namespace nexo::components {

    /**
     * @brief Enum representing different types of keyframes in a video.
     */
    enum class KeyframeType { NORMAL, LOOP, TRANSITION };

    /**
     * @brief Structure representing a keyframe in a video, including its start and end times and type.
     */
    struct VideoKeyframe {
        double start;
        double end;
        KeyframeType keyframeType;
    };

    /**
     * @brief Component for handling video playback within an entity.
     */
    struct VideoComponent {
        std::string path;
        std::vector<assets::AssetRef<assets::Material>> frames = {};
        double frameRate                                       = 30.0f; // Frames per second
        size_t currentFrameIndex                               = 0;
        size_t nbFrame                                         = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;

        size_t currentKeyframeIndex          = 0;
        std::vector<VideoKeyframe> keyframes = {};

        bool loopVideo = false;
        bool isLoaded  = false;

        [[nodiscard]] bool loadVideoFrames(const std::string& videoPath);

        void updateFrame();

        void setCurrentKeyframe(size_t keyframeIndex);
        void setCurrentFrame(size_t frameIndex);

        void skipToNextKeyframe();
        void skipToPreviousKeyframe();
        void restartVideo();
    };
} // namespace nexo::components
