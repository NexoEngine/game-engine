//// InputOpenGl.cpp //////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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

    glm::vec2 InputOpenGl::getMousePosition() const
    {
        double xpos = 0.0f;
        double ypos = 0.0f;
        glfwGetCursorPos(static_cast<GLFWwindow *>(m_window->window()), &xpos, &ypos);
        return {static_cast<float>(xpos), static_cast<float>(ypos)};
    }

}
