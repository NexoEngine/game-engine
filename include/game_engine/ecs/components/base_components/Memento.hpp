/*
** EPITECH PROJECT, 2023
** Nexo
** File description:
** Memento.hpp
*/

#pragma once

#include "./Physics.hpp"
#include "../Coordinator.hpp"
#include "../Entity.hpp"
#include "../../core/event/Event.hpp"
#include <iostream>
#include <functional>
#include <memory>
#include "game_engine/ecs/save/IMemento.hpp"

namespace ecs::components {
        namespace memento {
            /**
             * @class Memento
             * @brief Abstract class representing a memento component.
             *
			 * This class provides a base for creating memento components in an ECS (Entity Component System).
			 * Memento components are used to store the state of an entity at a given point in time.
			 * This is useful for saving and loading game states, as well as for undo/redo functionality.
             * 
			 * Mementos must implement the IMemento interface, serializing and deserializing the state.
             */
			class Memento : public engine::save::IMemento {
			public:
				virtual ~Memento() = default;
            };
        }
    }
} // namespace ecs::components::memento
