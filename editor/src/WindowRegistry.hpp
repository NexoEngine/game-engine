//// WindowRegistry.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/03/2025
//  Description: Header file for the window registry class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "IDocumentWindow.hpp"

#include <unordered_map>
#include <typeindex>
#include <memory>

namespace nexo::editor {
	class WindowRegistry {
		public:
			template<typename T>
			void registerWindow(std::shared_ptr<T> window)
			{
				m_windows[typeid(T)] = window;
			}

			template<typename T>
			std::shared_ptr<T> getWindow()
			{
				if (m_windows.find(typeid(T)) == m_windows.end())
				{
					throw std::runtime_error("Window not registered");
				}
				return std::static_pointer_cast<T>(m_windows[typeid(T)]);
			}

			void setup();
			void shutdown();
			void update();
			void render();

		private:
			std::unordered_map<std::type_index, std::shared_ptr<IDocumentWindow>> m_windows;
	};
}
