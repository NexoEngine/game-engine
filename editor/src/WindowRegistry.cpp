//// WindowRegistry.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/03/2025
//  Description: Source file for the window registry
//
///////////////////////////////////////////////////////////////////////////////

#include "WindowRegistry.hpp"

namespace nexo::editor {

	void WindowRegistry::setup()
	{
		for (auto &[_, window]: m_windows)
        {
            window->setup();
        }
	}

	void WindowRegistry::shutdown()
	{
		for (const auto &[_, window]: m_windows)
        {
            window->shutdown();
        }
	}

	void WindowRegistry::update()
	{
		for (const auto &[_, window]: m_windows)
        {
            window->update();
        }
	}

	void WindowRegistry::render()
	{
		for (const auto &[_, window]: m_windows)
        {
            if (window->isOpened())
                window->show();
        }
	}
}
