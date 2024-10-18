/*
** NEXO ENGINE, 2024
** Memento
** File description:
** Memento to save states of the editor
*/

#pragma once

namespace engine::editor {
	class Memento {
	public:
		virtual ~Memento() = default;
	};
} // namespace engine::editor