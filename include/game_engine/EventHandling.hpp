/*
** EPITECH PROJECT, 2024
** Nexo
** File description:
** EventHandling
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/GameEngine.hpp"

namespace engine {

    /**
     * @brief Checks if the window of the game engine is open.
     * @return True if the window is open, false otherwise.
     */
    bool isWindowOpen(void);

    /**
     * @brief Returns true if a key has been pressed
     *
     * @param key
     * @return true
     * @return false
     */
    bool isKeyPressed(ecs::components::input::Keys key);

    /**
     * @brief Returns true if a key has been released
     *
     * @param key
     * @return true
     * @return false
     */
    bool isKeyReleased(ecs::components::input::Keys key);

    /**
     * @brief Returns true if a key is down
     *
     * @param key
     * @return true
     * @return false
     */
    bool isKeyDown(ecs::components::input::Keys key);

    /**
     * @brief Returns true if a key is up
     *
     * @param key
     * @return true
     * @return false
     */
    bool isKeyUp(ecs::components::input::Keys key);

    Vector2 getMouseDelta(void);

    bool isMouseButtonDown(ecs::components::input::MouseButtons button);

    /**
     * @brief Registers an event listener.
     * @tparam T Type of the event.
     * @param listener Function to handle the event.
     */
    template<typename T>
    void registerListener(std::function<void(T&)> listener)
    {
        engine::Engine::getInstance()->registerListener<T>(listener);
    }

    template<typename T>
    void emitEvent(T &event)
    {
        engine::Engine::getInstance()->emitEvent(event);
    }
}