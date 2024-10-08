/*
** EPITECH PROJECT, 2024
** Nexo
** File description:
** AudioHandling
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"

namespace engine {

    /**
     * @brief Triggers an audio sound
     *
     * @param sound The sound to play
     */
    void triggerAudio(Sound sound);

    /**
     * @brief Plays a music
     *
     * @param audioPath the path to the music file
     * @return ecs::Entity
     */
    ecs::Entity playMusic(const std::string &audioPath, bool looping = false);

    /**
     * @brief Stops a music
     *
     * @param musicSource The music source entity to which the music is attached
     */
    void stopMusic(ecs::Entity musicSource);

    /**
     * @brief Pauses a music
     *
     * @param musicSource The music source entity to which the music is attached
     */
    void pauseMusic(ecs::Entity musicSource);

    /**
     * @brief Resumes a paused music
     *
     * @param musicSource The music source entity to which the paused music is attached
     */
    void resumeMusic(ecs::Entity musicSource);
}
