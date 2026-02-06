//// ImGuiTestFixture.hpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        03/12/2025
//  Description: Reusable test fixture for ImGui-dependent tests
//               Provides headless ImGui context with input injection helpers
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gtest/gtest.h>
#include <imgui.h>

namespace nexo::editor::testing {

    /**
     * @brief Test fixture that provides a headless ImGui context for testing
     *        ImGui-dependent code without requiring a display or rendering backend.
     *
     * Usage:
     *   class MyTest : public ImGuiTestFixture { ... };
     *
     *   TEST_F(MyTest, TestKeyPress) {
     *       beginFrame();
     *       pressKey(ImGuiKey_A);
     *       nextFrame();
     *       // Now ImGui::IsKeyDown(ImGuiKey_A) returns true
     *   }
     */
    class ImGuiTestFixture : public ::testing::Test {
    protected:
        void SetUp() override {
            m_ctx = ImGui::CreateContext();
            ImGui::SetCurrentContext(m_ctx);

            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(1280.0f, 720.0f);
            io.DeltaTime = 1.0f / 60.0f;
            io.IniFilename = nullptr;  // Disable .ini file

            // Initialize fonts (required before NewFrame)
            io.Fonts->AddFontDefault();
            unsigned char* pixels;
            int width, height;
            io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
            io.Fonts->SetTexID(static_cast<ImTextureID>(1));
        }

        void TearDown() override {
            if (m_ctx) {
                ImGui::DestroyContext(m_ctx);
                m_ctx = nullptr;
            }
        }

        /**
         * @brief Simulate pressing a key
         * @param key The ImGuiKey to press
         */
        void pressKey(ImGuiKey key) {
            ImGui::GetIO().AddKeyEvent(key, true);
        }

        /**
         * @brief Simulate releasing a key
         * @param key The ImGuiKey to release
         */
        void releaseKey(ImGuiKey key) {
            ImGui::GetIO().AddKeyEvent(key, false);
        }

        /**
         * @brief Release all keys (helps with test isolation)
         *        Call this at the start of tests to clear any lingering state
         */
        void releaseAllKeys() {
            // Only release keys that are not alias/mod keys
            // ImGuiKey_NamedKey_END includes alias keys (ImGuiMod_*) which cannot be used with AddKeyEvent
            // Use ImGuiKey_GamepadLStickLeft as upper bound - it's the last non-alias named key
            for (int k = ImGuiKey_NamedKey_BEGIN; k <= ImGuiKey_GamepadRStickDown; ++k) {
                ImGui::GetIO().AddKeyEvent(static_cast<ImGuiKey>(k), false);
            }
        }

        /**
         * @brief Advance to the next frame
         *        Ends current frame and starts a new one, processing input events
         * @param deltaTime Time delta for this frame (default 1/60s)
         */
        void nextFrame(float deltaTime = 1.0f / 60.0f) {
            ImGui::EndFrame();
            ImGui::GetIO().DeltaTime = deltaTime;
            ImGui::NewFrame();
        }

        /**
         * @brief Start the first frame
         *        Call this before any ImGui operations in a test
         */
        void beginFrame() {
            ImGui::NewFrame();
        }

        /**
         * @brief Run multiple frames with keys released to help clear static state
         * @param frameCount Number of frames to run
         */
        void runEmptyFrames(int frameCount = 2) {
            for (int i = 0; i < frameCount; ++i) {
                releaseAllKeys();
                nextFrame(0.5f);  // Use larger delta to exceed multi-press threshold
            }
        }

    private:
        ImGuiContext* m_ctx = nullptr;
    };

}  // namespace nexo::editor::testing
