/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** AudioHandling
*/

#include "game_engine/GameEngine.hpp"

namespace engine {

    void Engine::triggerAudio(Sound sound)
    {
        auto audioEntity = this->_coordinator->createEntity();
        ecs::components::sound::AudioSource audioSrc{sound, false};
        this->_coordinator->addComponent(audioEntity, audioSrc);
        PlaySound(sound);
    }

    ecs::Entity Engine::playMusic(const std::string &musicPath, bool looping)
    {
        ecs::Entity entity = _coordinator->createEntity();
        ecs::components::sound::MusicSource musicSource;
        musicSource.music = LoadMusicStream(musicPath.c_str());
        musicSource.isLooping = looping;
        musicSource.isPaused = false;
        musicSource.resume = false;
        musicSource.stop = false;
        _coordinator->addComponent<ecs::components::sound::MusicSource>(entity, musicSource);
        PlayMusicStream(musicSource.music);
        return entity;
    }

    void stopMusic(ecs::Entity musicSource)
    {
        Engine::getInstance()->stopMusic(musicSource);
    }

    void Engine::pauseMusic(ecs::Entity musicSource)
    {
        auto &music = _coordinator->getComponent<ecs::components::sound::MusicSource>(musicSource);
        music.isPaused = true;
    }

    void Engine::resumeMusic(ecs::Entity musicSource)
    {
        auto &music = _coordinator->getComponent<ecs::components::sound::MusicSource>(musicSource);
        music.resume = true;
        music.isPaused = false;
    }
}