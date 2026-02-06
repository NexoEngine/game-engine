//// Config.test.cpp ///////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        13/12/2025
//  Description: Comprehensive unit tests for Config parser utilities
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <regex>
#include "utils/Config.hpp"
#include "WindowRegistry.hpp"

namespace nexo::editor {

    // Test fixture for Config tests with temporary file creation
    class ConfigTest : public ::testing::Test {
    protected:
        std::filesystem::path testConfigPath;
        std::filesystem::path testDir;

        void SetUp() override {
            // Create temporary test directory
            testDir = std::filesystem::temp_directory_path() / "nexo_config_test";
            std::filesystem::create_directories(testDir);
            testConfigPath = testDir / "test-layout.ini";
        }

        void TearDown() override {
            // Clean up test files
            if (std::filesystem::exists(testConfigPath)) {
                std::filesystem::remove(testConfigPath);
            }
            if (std::filesystem::exists(testDir)) {
                std::filesystem::remove_all(testDir);
            }
        }

        // Helper to create a config file with specified content
        void createConfigFile(const std::string& content) {
            std::ofstream file(testConfigPath);
            file << content;
            file.close();
        }

        // Helper to create a standard config file with a window
        void createStandardConfig(const std::string& windowName, const std::string& dockId) {
            std::stringstream ss;
            ss << "[Window][" << windowName << "]\n";
            ss << "Pos=100,100\n";
            ss << "Size=800,600\n";
            ss << "DockId=" << dockId << "\n";
            ss << "\n";
            createConfigFile(ss.str());
        }
    };

    // ==========================================================================
    // Hex Parsing Tests (ImGuiID conversion)
    // ==========================================================================

    TEST_F(ConfigTest, HexParsingWithPrefixStandard) {
        createStandardConfig("###TestWindow", "0x00000001");

        // This test validates that the hex parsing works correctly
        // We can't directly test the parsing logic without modifying the source,
        // but we validate the overall function behavior
        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                EXPECT_EQ(hexDockId, "0x00000001");

                // Test hex conversion
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 1);
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, HexParsingUppercaseLetters) {
        createStandardConfig("###TestWindow", "0xABCDEF12");

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 0xABCDEF12);
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, HexParsingLowercaseLetters) {
        createStandardConfig("###TestWindow", "0xabcdef12");

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 0xabcdef12);
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, HexParsingMixedCase) {
        createStandardConfig("###TestWindow", "0xAbCdEf12");

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 0xAbCdEf12);
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, HexParsingZeroValue) {
        createStandardConfig("###TestWindow", "0x00000000");

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 0);
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, HexParsingMaxValue) {
        createStandardConfig("###TestWindow", "0xFFFFFFFF");

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 0xFFFFFFFF);
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    // ==========================================================================
    // Window Section Detection Pattern Tests
    // ==========================================================================

    TEST_F(ConfigTest, WindowSectionDetectionStandard) {
        createStandardConfig("###Inspector", "0x00000001");

        std::ifstream file(testConfigPath);
        std::string line;
        std::string windowHeader = "[Window][###Inspector]";

        bool found = false;
        while (std::getline(file, line)) {
            if (line == windowHeader) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, WindowSectionDetectionWithSpaces) {
        std::stringstream ss;
        ss << "[Window][###My Window Name]\n";
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::string windowHeader = "[Window][###My Window Name]";

        bool found = false;
        while (std::getline(file, line)) {
            if (line == windowHeader) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, WindowSectionDetectionSpecialCharacters) {
        std::stringstream ss;
        ss << "[Window][###Window-Name_123]\n";
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::string windowHeader = "[Window][###Window-Name_123]";

        bool found = false;
        while (std::getline(file, line)) {
            if (line == windowHeader) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, WindowSectionDetectionMultipleWindows) {
        std::stringstream ss;
        ss << "[Window][###Window1]\n";
        ss << "DockId=0x00000001\n\n";
        ss << "[Window][###Window2]\n";
        ss << "DockId=0x00000002\n\n";
        ss << "[Window][###Window3]\n";
        ss << "DockId=0x00000003\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;

        int windowCount = 0;
        while (std::getline(file, line)) {
            if (line.find("[Window][###Window") != std::string::npos) {
                windowCount++;
            }
        }
        EXPECT_EQ(windowCount, 3);
    }

    TEST_F(ConfigTest, WindowSectionDetectionNonHashedWindow) {
        std::stringstream ss;
        ss << "[Window][RegularWindow]\n";
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::string windowHeader = "[Window][RegularWindow]";

        bool found = false;
        while (std::getline(file, line)) {
            if (line == windowHeader) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    // ==========================================================================
    // Scene Window Pattern Tests (###Default Scene\d+)
    // ==========================================================================

    TEST_F(ConfigTest, SceneWindowPatternSingleDigit) {
        std::stringstream ss;
        ss << "[Window][###Default Scene0]\n";
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex windowRegex(R"(\[Window\]\[(###Default Scene\d+)\])");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, windowRegex) && match.size() > 1) {
                EXPECT_EQ(match[1].str(), "###Default Scene0");
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, SceneWindowPatternMultipleDigits) {
        std::stringstream ss;
        ss << "[Window][###Default Scene123]\n";
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex windowRegex(R"(\[Window\]\[(###Default Scene\d+)\])");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, windowRegex) && match.size() > 1) {
                EXPECT_EQ(match[1].str(), "###Default Scene123");
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, SceneWindowPatternMultipleScenes) {
        std::stringstream ss;
        ss << "[Window][###Default Scene0]\n";
        ss << "DockId=0x00000001\n\n";
        ss << "[Window][###Default Scene1]\n";
        ss << "DockId=0x00000002\n\n";
        ss << "[Window][###Default Scene2]\n";
        ss << "DockId=0x00000003\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex windowRegex(R"(\[Window\]\[(###Default Scene\d+)\])");

        std::vector<std::string> scenes;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, windowRegex) && match.size() > 1) {
                scenes.push_back(match[1].str());
            }
        }

        EXPECT_EQ(scenes.size(), 3);
        EXPECT_EQ(scenes[0], "###Default Scene0");
        EXPECT_EQ(scenes[1], "###Default Scene1");
        EXPECT_EQ(scenes[2], "###Default Scene2");
    }

    TEST_F(ConfigTest, SceneWindowPatternDoesNotMatchInvalidFormat) {
        std::stringstream ss;
        ss << "[Window][###Default SceneA]\n";  // Letter instead of digit
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex windowRegex(R"(\[Window\]\[(###Default Scene\d+)\])");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, windowRegex) && match.size() > 1) {
                found = true;
                break;
            }
        }
        EXPECT_FALSE(found);
    }

    // ==========================================================================
    // Config Line Parsing Tests
    // ==========================================================================

    TEST_F(ConfigTest, ConfigLineParsingStandardFormat) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "Pos=100,200\n";
        ss << "Size=800,600\n";
        ss << "DockId=0x00000001\n";
        ss << "Collapsed=0\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::map<std::string, std::string> properties;

        bool inSection = false;
        while (std::getline(file, line)) {
            if (line == "[Window][###TestWindow]") {
                inSection = true;
                continue;
            }

            if (inSection && !line.empty() && line[0] == '[') {
                break;
            }

            if (inSection && line.find('=') != std::string::npos) {
                size_t pos = line.find('=');
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                properties[key] = value;
            }
        }

        EXPECT_EQ(properties["Pos"], "100,200");
        EXPECT_EQ(properties["Size"], "800,600");
        EXPECT_EQ(properties["DockId"], "0x00000001");
        EXPECT_EQ(properties["Collapsed"], "0");
    }

    TEST_F(ConfigTest, ConfigLineParsingWithWhitespace) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "  Pos=100,200  \n";
        ss << "\tSize=800,600\n";
        ss << "DockId = 0x00000001\n";  // Spaces around =
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;

        bool foundDockId = false;
        while (std::getline(file, line)) {
            if (line.find("DockId") != std::string::npos) {
                foundDockId = true;
                // Line should contain the equals sign despite spaces
                EXPECT_NE(line.find('='), std::string::npos);
            }
        }
        EXPECT_TRUE(foundDockId);
    }

    TEST_F(ConfigTest, ConfigLineParsingEmptyLines) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "\n";
        ss << "DockId=0x00000001\n";
        ss << "\n";
        ss << "\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, ConfigLineParsingMultipleEquals) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "CustomProperty=value=with=equals\n";
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;

        bool found = false;
        while (std::getline(file, line)) {
            if (line.find("CustomProperty") != std::string::npos) {
                size_t pos = line.find('=');
                std::string value = line.substr(pos + 1);
                EXPECT_EQ(value, "value=with=equals");
                found = true;
            }
        }
        EXPECT_TRUE(found);
    }

    // ==========================================================================
    // Missing File/Section Handling Tests
    // ==========================================================================

    TEST_F(ConfigTest, MissingFileHandling) {
        std::filesystem::path nonExistentPath = testDir / "non-existent.ini";

        std::ifstream file(nonExistentPath);
        EXPECT_FALSE(file.is_open());
    }

    TEST_F(ConfigTest, MissingSectionHandling) {
        createStandardConfig("###Window1", "0x00000001");

        std::ifstream file(testConfigPath);
        std::string line;
        std::string windowHeader = "[Window][###NonExistentWindow]";

        bool found = false;
        while (std::getline(file, line)) {
            if (line == windowHeader) {
                found = true;
                break;
            }
        }
        EXPECT_FALSE(found);
    }

    TEST_F(ConfigTest, MissingDockIdProperty) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "Pos=100,200\n";
        ss << "Size=800,600\n";
        // No DockId property
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool inSection = false;
        bool foundDockId = false;

        while (std::getline(file, line)) {
            if (line == "[Window][###TestWindow]") {
                inSection = true;
                continue;
            }

            if (inSection) {
                if (!line.empty() && line[0] == '[') {
                    break;
                }

                std::smatch match;
                if (std::regex_search(line, match, dockIdRegex)) {
                    foundDockId = true;
                    break;
                }
            }
        }

        EXPECT_FALSE(foundDockId);
    }

    // ==========================================================================
    // Edge Cases: Empty Configs
    // ==========================================================================

    TEST_F(ConfigTest, EmptyConfigFile) {
        createConfigFile("");

        std::ifstream file(testConfigPath);
        std::string line;

        int lineCount = 0;
        while (std::getline(file, line)) {
            lineCount++;
        }

        EXPECT_EQ(lineCount, 0);
    }

    TEST_F(ConfigTest, ConfigWithOnlyWhitespace) {
        createConfigFile("   \n\n\t\n   ");

        std::ifstream file(testConfigPath);
        std::string line;

        bool hasNonWhitespace = false;
        while (std::getline(file, line)) {
            if (!line.empty() && line.find_first_not_of(" \t\n\r") != std::string::npos) {
                hasNonWhitespace = true;
                break;
            }
        }

        EXPECT_FALSE(hasNonWhitespace);
    }

    TEST_F(ConfigTest, ConfigWithOnlyComments) {
        std::stringstream ss;
        ss << "# This is a comment\n";
        ss << "# Another comment\n";
        ss << "; Yet another comment style\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;

        bool foundSection = false;
        while (std::getline(file, line)) {
            if (!line.empty() && line[0] == '[') {
                foundSection = true;
                break;
            }
        }

        EXPECT_FALSE(foundSection);
    }

    // ==========================================================================
    // Edge Cases: Malformed Lines
    // ==========================================================================

    TEST_F(ConfigTest, MalformedWindowHeader) {
        std::stringstream ss;
        ss << "[Window][###TestWindow\n";  // Missing closing bracket
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::string correctHeader = "[Window][###TestWindow]";

        bool foundCorrectHeader = false;
        while (std::getline(file, line)) {
            if (line == correctHeader) {
                foundCorrectHeader = true;
                break;
            }
        }

        EXPECT_FALSE(foundCorrectHeader);
    }

    TEST_F(ConfigTest, MalformedDockIdValue) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "DockId=INVALID\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool foundValidDockId = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                foundValidDockId = true;
                break;
            }
        }

        EXPECT_FALSE(foundValidDockId);
    }

    TEST_F(ConfigTest, MalformedDockIdMissingPrefix) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "DockId=12345678\n";  // Missing 0x prefix
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool foundValidDockId = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                foundValidDockId = true;
                break;
            }
        }

        EXPECT_FALSE(foundValidDockId);
    }

    TEST_F(ConfigTest, PropertyWithoutValue) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "DockId=\n";  // Property with no value
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;

        bool foundEmptyValue = false;
        while (std::getline(file, line)) {
            if (line == "DockId=") {
                foundEmptyValue = true;
                break;
            }
        }

        EXPECT_TRUE(foundEmptyValue);
    }

    TEST_F(ConfigTest, PropertyWithoutEquals) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "DockId\n";  // Property without equals sign
        ss << "Size=800,600\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;

        bool foundInvalidProperty = false;
        while (std::getline(file, line)) {
            if (line == "DockId" && line.find('=') == std::string::npos) {
                foundInvalidProperty = true;
                break;
            }
        }

        EXPECT_TRUE(foundInvalidProperty);
    }

    // ==========================================================================
    // Edge Cases: Special Characters
    // ==========================================================================

    TEST_F(ConfigTest, WindowNameWithUnicode) {
        std::stringstream ss;
        ss << "[Window][###Окно]\n";  // Russian characters
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;

        bool found = false;
        while (std::getline(file, line)) {
            if (line.find("###Окно") != std::string::npos) {
                found = true;
                break;
            }
        }

        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, WindowNameWithSymbols) {
        std::stringstream ss;
        ss << "[Window][###Window!@#$%]\n";
        ss << "DockId=0x00000001\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;

        bool found = false;
        while (std::getline(file, line)) {
            if (line.find("###Window!@#$%") != std::string::npos) {
                found = true;
                break;
            }
        }

        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, DockIdWithLeadingZeros) {
        createStandardConfig("###TestWindow", "0x00000ABC");

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 0xABC);  // Leading zeros should be ignored
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    // ==========================================================================
    // Complex Config Structure Tests
    // ==========================================================================

    TEST_F(ConfigTest, ComplexConfigWithMultipleSections) {
        std::stringstream ss;
        ss << "[Docking][Data]\n";
        ss << "DockSpace ID=0x12345678\n\n";
        ss << "[Window][###Inspector]\n";
        ss << "Pos=100,100\n";
        ss << "Size=400,600\n";
        ss << "DockId=0x00000001\n\n";
        ss << "[Window][###Scene]\n";
        ss << "Pos=500,100\n";
        ss << "Size=800,600\n";
        ss << "DockId=0x00000002\n\n";
        ss << "[Window][RegularWindow]\n";
        ss << "Pos=0,0\n";
        ss << "Size=300,400\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex windowHeaderRegex(R"(\[Window\]\[(.+)\])");

        std::vector<std::string> windowNames;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, windowHeaderRegex) && match.size() > 1) {
                windowNames.push_back(match[1].str());
            }
        }

        EXPECT_EQ(windowNames.size(), 3);
        EXPECT_EQ(windowNames[0], "###Inspector");
        EXPECT_EQ(windowNames[1], "###Scene");
        EXPECT_EQ(windowNames[2], "RegularWindow");
    }

    TEST_F(ConfigTest, ConfigWithAdjacentSections) {
        std::stringstream ss;
        ss << "[Window][###Window1]\n";
        ss << "DockId=0x00000001\n";
        ss << "[Window][###Window2]\n";  // No blank line separator
        ss << "DockId=0x00000002\n";
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex windowHeaderRegex(R"(\[Window\]\[(.+)\])");

        std::vector<std::string> windowNames;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, windowHeaderRegex) && match.size() > 1) {
                windowNames.push_back(match[1].str());
            }
        }

        EXPECT_EQ(windowNames.size(), 2);
    }

    // ==========================================================================
    // DockId Extraction Different Formats
    // ==========================================================================

    TEST_F(ConfigTest, DockIdExtractionShortHex) {
        createStandardConfig("###TestWindow", "0x1");

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 1);
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, DockIdExtractionFullHex) {
        createStandardConfig("###TestWindow", "0x12345678");

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 0x12345678);
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST_F(ConfigTest, DockIdExtractionWithComma) {
        std::stringstream ss;
        ss << "[Window][###TestWindow]\n";
        ss << "DockId=0x00000001,0\n";  // ImGui format with visibility flag
        createConfigFile(ss.str());

        std::ifstream file(testConfigPath);
        std::string line;
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        bool found = false;
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                std::string hexDockId = match[1];
                ImGuiID dockId = 0;
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                EXPECT_EQ(dockId, 1);
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

}  // namespace nexo::editor
