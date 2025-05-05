//// Show.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        05/05/2025
//  Description: Source file for the test window rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "TestWindow.hpp"
#include "ImNexo/Components.hpp"

namespace nexo::editor {

    static void renderRadioButtons(TestResult &result)
    {
        if (ImGui::RadioButton("Passed",   result == TestResult::PASSED))
            result = TestResult::PASSED;
        ImGui::SameLine();
        if (ImGui::RadioButton("Failed",   result == TestResult::FAILED))
            result = TestResult::FAILED;
        ImGui::SameLine();
        if (ImGui::RadioButton("Skipped",  result == TestResult::SKIPPED))
            result = TestResult::SKIPPED;
    }

    static void renderTestCases(TestSection &section)
    {
        for (unsigned int i = 0; i < section.testCases.size(); ++i) {
            auto &tc = section.testCases[i];
            ImGui::PushID(static_cast<int>(i));
            ImGui::Text("%s", tc.name.c_str());
            ImGui::SameLine();

            renderRadioButtons(tc.result);

            if (tc.result == TestResult::SKIPPED) {
                ImGui::Indent(20.0f);
                ImGui::InputTextWithHint(
                    "##skip_reason",
                    "Reason for skip...",
                    tc.skippedMessage,
                    sizeof(tc.skippedMessage)
                );
                ImGui::Unindent(20.0f);
            }

            ImGui::PopID();
        }
    }

    static void renderSubSections(std::vector<TestSection> &subSections)
    {
        for (unsigned int i = 0; i < subSections.size(); ++i) {
            auto &sub = subSections[i];
            ImGui::PushID(static_cast<int>(i));
            ImNexo::ToggleButtonWithSeparator(sub.name, &sub.sectionOpen);
            if (sub.sectionOpen)
                renderTestCases(sub);
            ImGui::PopID();
        }
    }

    void TestWindow::show()
    {
        if (!ImGui::Begin(NEXO_WND_USTRID_TEST, &m_opened, ImGuiWindowFlags_None)) {
            ImGui::End();
            return;
        }
        beginRender(NEXO_WND_USTRID_TEST);

        for (unsigned int i = 0; i < m_testSections.size(); ++i) {
            auto &section = m_testSections[i];
            ImGui::PushID(static_cast<int>(i));
            if (ImNexo::Header(std::format("##MainSection{}", i), section.name)) {
                // Any test cases directly in this section
                renderTestCases(section);

                // Now sub-sections
                renderSubSections(section.subSections);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        // Action buttons
        ImGui::Separator();
        if (ImGui::Button("Cancel")) {
            resetTestCases();
            m_opened = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Confirm")) {
            writeTestReport();
            resetTestCases();
            m_opened = false;
        }

        ImGui::End();
    }
}
