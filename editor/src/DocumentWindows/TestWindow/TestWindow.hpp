//// TestWindow.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        05/05/2025
//  Description: Header file for the test window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"

#include <vector>
#include <filesystem>

namespace nexo::editor {

    enum class TestResult {
        NOT_TESTED,
        PASSED,
        FAILED,
        SKIPPED
    };

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

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;
        private:
            std::vector<TestSection> m_testSections;

            void parseTestFolder();
            void parseFile(const std::filesystem::directory_entry &entry);
            void resetTestCases();
            void writeTestReport();
    };
}
