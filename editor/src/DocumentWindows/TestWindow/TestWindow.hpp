//// TestWindow.hpp ///////////////////////////////////////////////////////////////
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
//  Description: Header file for the test window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"

#include <filesystem>
#include <vector>

namespace nexo::editor {

    enum class TestResult { NOT_TESTED, PASSED, FAILED, SKIPPED };

    struct TestCase {
        std::string name;
        TestResult result = TestResult::NOT_TESTED;
        char skippedMessage[512];
    };

    struct TestSection {
        std::string name;
        std::vector<TestCase> testCases;
        bool sectionOpen = false;
        std::vector<TestSection> subSections;
    };

    class TestWindow final : public ADocumentWindow {
       public:
        using ADocumentWindow::ADocumentWindow;

        /**
         * @brief Initializes the TestWindow.
         *
         * This method is called when the TestWindow is first created or opened.
         * It is responsible for setting up any necessary resources, loading test
         * files, and preparing the UI elements required for displaying and managing
         * the test cases.
         */
        void setup() override;

        /**
         * @brief Shuts down the TestWindow.
         *
         * This method is called when the TestWindow is being closed or destroyed.
         * It is responsible for cleaning up any resources, saving state, or performing
         * any necessary finalization tasks related to the TestWindow.
         */
        void shutdown() override;

        /**
         * @brief Renders the TestWindow UI.
         *
         * This method is responsible for rendering the entire TestWindow interface,
         * including sections, subsections, test cases, and action buttons. It handles
         * user interactions such as marking test cases as passed, failed, or skipped,
         * and provides options to confirm or cancel the test results.
         */
        void show() override;

        /**
         * @brief Updates the state of the TestWindow.
         *
         * This method is called periodically to refresh or update the internal
         * state of the TestWindow. It can be used to handle dynamic content,
         * respond to user interactions, or perform any necessary background
         * processing related to the test cases and sections.
         */
        void update() override;

       private:
        std::vector<TestSection> m_testSections;

        /**
         * @brief Parses all test files in the designated test folder.
         *
         * This function iterates over all files in the predefined test folder,
         * invoking parseFile for each file to extract test sections and cases.
         * It populates the m_testSections member variable with the aggregated data.
         *
         * @throws FileReadException if any file cannot be opened for reading.
         * @throws InvalidTestFileFormat if any file format is incorrect.
         */
        void parseTestFolder();

        /**
         * @brief Parses a single test file to extract sections and test cases.
         *
         * This function reads the specified file line by line, identifying
         * sections, subsections, and test cases based on markdown-like syntax.
         * It populates the m_testSections member variable with the parsed data.
         *
         * @param entry The directory entry representing the test file to parse.
         * @throws FileReadException if the file cannot be opened for reading.
         * @throws InvalidTestFileFormat if the file format is incorrect.
         */
        void parseFile(const std::filesystem::directory_entry &entry);

        /**
         * @brief Resets all test cases to their initial state.
         *
         * This function iterates through all test sections and their respective
         * test cases, resetting each test case's result to NOT_TESTED and
         * clearing any skipped messages. It prepares the TestWindow for a fresh
         * testing session.
         */
        void resetTestCases();

        /**
         * @brief Generates and writes a test report to a user-specified file.
         *
         * This function compiles the results of all test cases, including their
         * statuses (passed, failed, skipped) and any relevant messages. It prompts
         * the user to select a file location and writes the formatted report to
         * that file.
         *
         * @throws FileWriteException if the report file cannot be created or written to.
         */
        void writeTestReport() const;
    };
} // namespace nexo::editor
