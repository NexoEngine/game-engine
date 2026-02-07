//// Parser.cpp ///////////////////////////////////////////////////////////////
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
//  Description: Source file for the parsing logic for the test window
//
///////////////////////////////////////////////////////////////////////////////

#include "Error.hpp"
#include "Exception.hpp"
#include "Logger.hpp"
#include "Path.hpp"
#include "TestWindow.hpp"
#include "exceptions/Exceptions.hpp"
#include "utils/String.hpp"

#include <cctype>
#include <cerrno>
#include <fstream>

namespace nexo::editor {

    /**
     * @brief Parses a bullet point line to extract the test case name.
     *
     * This function checks if the provided line starts with a bullet point
     * indicator (a hyphen followed by a space) and extracts the text following
     * it as the test case name. If the line does not conform to this format,
     * an empty string is returned.
     *
     * @param line The line of text to parse.
     * @return The extracted test case name, or an empty string if the format is invalid.
     */
    static std::string parseBullet(const std::string &line)
    {
        if (line.size() >= 2 && line[0] == '-' && std::isspace(line[1])) return line.substr(2);
        return {};
    }

    void TestWindow::parseFile(const std::filesystem::directory_entry &entry)
    {
        std::ifstream in(entry.path());
        if (!in) THROW_EXCEPTION(FileReadException, entry.path().string(), nexo::strerror(errno));

        TestSection *currentSection    = nullptr;
        TestSection *currentSubSection = nullptr;
        std::string line;

        while (std::getline(in, line)) {
            utils::trim(line);
            // Top-level section
            if (line.rfind("# ", 0) == 0) {
                m_testSections.emplace_back();
                currentSection       = &m_testSections.back();
                currentSection->name = line.substr(2);
                currentSubSection    = nullptr;
                // Subsection
            } else if (line.rfind("## ", 0) == 0) {
                if (!currentSection)
                    THROW_EXCEPTION(InvalidTestFileFormat, entry.path().string(),
                                    "Subsection found without main section");
                currentSection->subSections.emplace_back();
                currentSubSection       = &currentSection->subSections.back();
                currentSubSection->name = line.substr(3);
                // Test case
            } else if (line.rfind('-', 0) == 0) {
                std::string testName = parseBullet(line);
                if (testName.empty())
                    THROW_EXCEPTION(InvalidTestFileFormat, entry.path().string(),
                                    "Test case format is invalid : \"- Test case name \"");

                TestCase testCase;
                testCase.name = std::move(testName);

                if (currentSubSection)
                    currentSubSection->testCases.push_back(std::move(testCase));
                else if (currentSection)
                    currentSection->testCases.push_back(std::move(testCase));
            }
        }
    }

    void TestWindow::parseTestFolder()
    {
        const std::filesystem::path testDir = Path::resolvePathRelativeToExe("../tests/editor");

        for (const auto &entry : std::filesystem::directory_iterator(testDir)) {
            if (!entry.is_regular_file()) {
                LOG(NEXO_WARN, "{} is a directory, skipping...", entry.path().string());
                continue;
            }
            if (entry.path().extension() != ".test") {
                LOG(NEXO_WARN, "{} is not a test file, skipping...", entry.path().string());
                continue;
            }
            try {
                parseFile(entry);
            } catch (const nexo::Exception &e) {
                LOG_EXCEPTION(e);
            }
        }
    }

} // namespace nexo::editor
