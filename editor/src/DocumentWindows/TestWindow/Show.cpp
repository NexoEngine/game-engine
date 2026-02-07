//// Show.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        05/05/2025
//  Description: Source file for the test window rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "TestWindow.hpp"
#include "ImNexo/Components.hpp"

namespace nexo::editor {

    /**
     * @brief Renders radio buttons for selecting a test result.
     *
     * This function displays three radio buttons labeled "Passed", "Failed",
     * and "Skipped". The currently selected test result is highlighted, and
     * clicking on a button updates the provided TestResult reference to the
     * corresponding value.
     *
     * @param result A reference to the TestResult variable to be updated based on user selection.
     */
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

    /**
     * @brief Renders the test cases within a given test section.
     *
     * This function creates a table layout to display each test case's name
     * and its associated result. For test cases marked as "Skipped", an input
     * field is provided to enter the reason for skipping the test.
     *
     * @param section The TestSection containing the test cases to be rendered.
     */
    static void renderTestCases(TestSection &section)
    {
        if (ImGui::BeginTable("TestCasesTable", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Result", ImGuiTableColumnFlags_WidthStretch);

            for (unsigned int i = 0; i < section.testCases.size(); ++i) {
                auto &tc = section.testCases[i];
                ImGui::PushID(static_cast<int>(i));

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGui::TextWrapped("%s", tc.name.c_str());

                ImGui::TableSetColumnIndex(1);
                renderRadioButtons(tc.result);

                // Skip reason (below the table row)
                if (tc.result == TestResult::SKIPPED) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Indent(20.0f);
                    ImGui::InputTextWithHint(
                        "##skip_reason",
                        "Reason for skip...",
                        tc.skippedMessage,
                        sizeof(tc.skippedMessage)
                    );
                    ImGui::Unindent(20.0f);

                    ImGui::TableSetColumnIndex(1);
                }

                ImGui::PopID();
            }
            ImGui::EndTable();
        }
    }

    /**
     * @brief Renders the subsections within a test section.
     *
     * This function iterates through each subsection, creating a toggleable
     * header for each. If a subsection is expanded, its test cases are rendered
     * using the renderTestCases function.
     *
     * @param subSections A vector of TestSection objects representing the subsections to be rendered.
     */
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

                // Now subsections
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
