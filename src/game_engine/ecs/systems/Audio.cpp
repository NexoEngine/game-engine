/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Audio.cpp
*/

#include "game_engine/GameEngine.hpp"
#include "game_engine/ecs/systems/Audio.hpp"
#include "game_engine/ecs/components/Audio.hpp"
#include <EntityHandling.hpp>


#include <iostream>

namespace ecs {
    namespace system {

        void AudioSystem::update() {
            for (auto &audioSource : _entities) {
                auto& audio = _coord->getComponent<ecs::components::sound::AudioSource>(audioSource);
                if (audio.isLooping && !IsSoundPlaying(audio.sound)) {
                    PlaySound(audio.sound);
                    continue;
                } else if (!IsSoundPlaying(audio.sound)) {
                    StopSound(audio.sound);
                    engine::destroyEntity(audioSource);
                    continue;
                }
            }
        }

        void MusicSystem::update() {
            for (auto &musicSource : _entities) {
                auto& musicComponent = _coord->getComponent<ecs::components::sound::MusicSource>(musicSource);
                if (musicComponent.isLooping && !IsMusicStreamPlaying(musicComponent.music)) {
                    StopMusicStream(musicComponent.music);
                    PlayMusicStream(musicComponent.music);
                } else if (!IsMusicStreamPlaying(musicComponent.music) || musicComponent.stop) {
                    StopMusicStream(musicComponent.music);
                    UnloadMusicStream(musicComponent.music);
                    engine::destroyEntity(musicSource);
                    continue;
                } else if (musicComponent.isPaused) {
                    PauseMusicStream(musicComponent.music);
                    musicComponent.isPaused = false;
                } else if (musicComponent.resume) {
                    ResumeMusicStream(musicComponent.music);
                    musicComponent.resume = false;
                }
                UpdateMusicStream(musicComponent.music);
            }
        }
    }
}
