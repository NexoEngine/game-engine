//// MockDocumentWindow.hpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        03/12/2025
//  Description: GMock-based mock for IDocumentWindow interface
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gmock/gmock.h>
#include <string>
#include <imgui.h>
#include "IDocumentWindow.hpp"
#include "inputs/WindowState.hpp"

namespace nexo::editor::testing {

/**
 * @brief Mock implementation of IDocumentWindow for unit testing
 *
 * Provides a GMock-based mock for testing code that depends on IDocumentWindow.
 * Stores state internally to allow returning references as required by the interface.
 */
class MockDocumentWindow : public IDocumentWindow {
public:
    explicit MockDocumentWindow(const std::string& name)
        : m_name(name), m_windowState(nextWindowId++) {}

    // Lifecycle methods - mockable
    MOCK_METHOD(void, setup, (), (override));
    MOCK_METHOD(void, shutdown, (), (override));
    MOCK_METHOD(void, show, (), (override));
    MOCK_METHOD(void, update, (), (override));

    // State query methods - mockable
    MOCK_METHOD(bool, isFocused, (), (const, override));
    MOCK_METHOD(bool, isOpened, (), (const, override));
    MOCK_METHOD(bool, isHovered, (), (const, override));

    // State modification
    void setOpened(bool opened) override { m_opened = opened; }

    // Reference-returning methods - return stored state
    [[nodiscard]] const ImVec2& getContentSize() const override { return m_contentSize; }
    [[nodiscard]] bool& getOpened() override { return m_opened; }
    [[nodiscard]] const std::string& getWindowName() const override { return m_name; }
    [[nodiscard]] const WindowState& getWindowState() const override { return m_windowState; }

    // Test helpers to set internal state
    void setContentSize(const ImVec2& size) { m_contentSize = size; }
    void setFocusedState(bool focused) { m_focused = focused; }

private:
    std::string m_name;
    bool m_opened = true;
    bool m_focused = false;
    ImVec2 m_contentSize{800.0f, 600.0f};
    WindowState m_windowState;
};

/**
 * @brief Second mock type for testing multi-type window registration
 *
 * Identical to MockDocumentWindow but as a separate type, allowing tests
 * to verify that WindowRegistry correctly handles multiple window types.
 */
class MockDocumentWindow2 : public IDocumentWindow {
public:
    explicit MockDocumentWindow2(const std::string& name)
        : m_name(name), m_windowState(nextWindowId++) {}

    MOCK_METHOD(void, setup, (), (override));
    MOCK_METHOD(void, shutdown, (), (override));
    MOCK_METHOD(void, show, (), (override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(bool, isFocused, (), (const, override));
    MOCK_METHOD(bool, isOpened, (), (const, override));
    MOCK_METHOD(bool, isHovered, (), (const, override));

    void setOpened(bool opened) override { m_opened = opened; }

    [[nodiscard]] const ImVec2& getContentSize() const override { return m_contentSize; }
    [[nodiscard]] bool& getOpened() override { return m_opened; }
    [[nodiscard]] const std::string& getWindowName() const override { return m_name; }
    [[nodiscard]] const WindowState& getWindowState() const override { return m_windowState; }

private:
    std::string m_name;
    bool m_opened = true;
    ImVec2 m_contentSize{800.0f, 600.0f};
    WindowState m_windowState;
};

}  // namespace nexo::editor::testing
