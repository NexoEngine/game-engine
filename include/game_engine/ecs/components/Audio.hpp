/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Audio.hpp
*/

#pragma once

#include "my_raylib.h"

namespace ecs {
    namespace components {
        namespace sound {
            struct AudioSource {
                Sound sound;
                bool isLooping;

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(AudioSource, sound, isLooping)
            };

            struct MusicSource {
                Music music;
                bool isLooping;
                bool isPaused;
                bool resume;
                bool stop;

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(MusicSource, music, isLooping, isPaused, resume, stop)
            };
        }
    }
}
