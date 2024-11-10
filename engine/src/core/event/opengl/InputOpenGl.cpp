//// InputOpenGl.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the input polling opengl class
//
///////////////////////////////////////////////////////////////////////////////

#include "InputOpenGl.hpp"

namespace nexo::event {
    bool InputOpenGl::isKeyPressed(const int keycode) const
    {
        return glfwGetKey(static_cast<GLFWwindow *>(m_window->window()), keycode) == GLFW_PRESS;
    }

    bool InputOpenGl::isKeyReleased(const int keycode) const
    {
        return glfwGetKey(static_cast<GLFWwindow *>(m_window->window()), keycode) == GLFW_RELEASE;
    }

    bool InputOpenGl::isKeyRepeat(const int keycode) const
    {
        return glfwGetKey(static_cast<GLFWwindow *>(m_window->window()), keycode) == GLFW_REPEAT;
    }

    bool InputOpenGl::isMouseDown(const int button) const
    {
        return glfwGetMouseButton(static_cast<GLFWwindow *>(m_window->window()), button) == GLFW_PRESS;
    }
    bool InputOpenGl::isMouseReleased(const int button) const
    {
        return glfwGetMouseButton(static_cast<GLFWwindow *>(m_window->window()), button) == GLFW_RELEASE;
    }
}
