//// Shutdown.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        05/05/2025
//  Description: Source file for the shutdown logic of the test window
//
///////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"
#include "TestWindow.hpp"
#include "Path.hpp"
#include "exceptions/Exceptions.hpp"

#include <fstream>

#ifdef __linux__
  #include <unistd.h>
  #include <sys/utsname.h>
#endif

#ifdef NX_GRAPHICS_API_OPENGL
  #include <glad/glad.h>
#endif

namespace nexo::editor {

    void TestWindow::resetTestCases()
    {
        for (auto &section : m_testSections) {
            for (auto &tc : section.testCases) {
                tc.result = TestResult::NOT_TESTED;
                memset(&tc.skippedMessage, 0, sizeof(tc.skippedMessage));
            }
            for (auto &sub : section.subSections) {
                for (auto &tc : sub.testCases) {
                    tc.result = TestResult::NOT_TESTED;
                    memset(&tc.skippedMessage, 0, sizeof(tc.skippedMessage));
                }
            }
        }
    }

    // Helper to get OS name
    static std::string getOSName()
    {
#if defined(_WIN32)
        return "Windows";
#elif defined(__APPLE__)
        return "macOS";
#elif defined(__linux__)
        struct utsname info;
        if (uname(&info) == 0)
            return std::string(info.sysname) + " " + info.release;
        else
            return "Linux";
#else
        return "Unknown OS";
#endif
    }

    // Helper to get CPU info (model name)
    static std::string getCPUInfo()
    {
#if defined(__linux__)
        std::ifstream cpuinfoFile("/proc/cpuinfo");
        std::string line;
        while (std::getline(cpuinfoFile, line)) {
            if (line.rfind("model name", 0) == 0) {
                auto pos = line.find(':');
                if (pos != std::string::npos) {
                    std::string model = line.substr(pos + 1);
                    // trim leading spaces
                    model.erase(model.begin(), std::find_if(model.begin(), model.end(), [](unsigned char ch) { return !std::isspace(ch); }));
                    return model;
                }
            }
        }
        return "Unknown CPU";
#elif defined(_WIN32)
        // Using __cpuid to get CPU brand string
        int cpuInfo[4] = {0};
        char brand[0x40] = { 0 };
        __cpuid(cpuInfo, 0x80000000);
        unsigned int maxId = cpuInfo[0];
        if (maxId >= 0x80000004) {
            __cpuid((int*)cpuInfo, 0x80000002);
            memcpy(brand, cpuInfo, sizeof(cpuInfo));
            __cpuid((int*)cpuInfo, 0x80000003);
            memcpy(brand + 16, cpuInfo, sizeof(cpuInfo));
            __cpuid((int*)cpuInfo, 0x80000004);
            memcpy(brand + 32, cpuInfo, sizeof(cpuInfo));
            return std::string(brand);
        }
        return "Unknown CPU";
#else
        return std::to_string(std::thread::hardware_concurrency()) + " cores";
#endif
    }

    // Helper to get GPU / graphics backend info (OpenGL example)
    static std::string getGraphicsInfo()
    {
#ifdef NX_GRAPHICS_API_OPENGL
        const char* vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const char* version  = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        return std::string("OpenGL: ") + vendor + " - " + renderer + " (" + version + ")";
#else
        return "Graphics info not available";
#endif
    }

    // Write environment section at top of report
    static void writeEnvironmentReport(std::ofstream &out)
    {
        out << std::format("# Environment\n");
        out << std::format("OS: {}\n", getOSName());
        out << std::format("CPU: {}\n", getCPUInfo());
        out << std::format("Graphics: {}\n", getGraphicsInfo());
        out << std::format("Timestamp: {:%Y-%m-%d %H:%M:%S}\n\n", std::chrono::system_clock::now());
    }

    static constexpr std::string testResultToString(const TestResult r)
    {
        switch (r)
        {
            case TestResult::PASSED:    return "PASSED";
            case TestResult::FAILED:    return "FAILED";
            case TestResult::SKIPPED:   return "SKIPPED";
            default:                    return "NOT_TESTED";
        }
    }

    static void writeTestCaseReport(std::ofstream& out, const TestCase& tc)
    {
        out << std::format("- {} : {}\n", tc.name, testResultToString(tc.result));
        if (tc.result == TestResult::SKIPPED)
            out << std::format("  Reason: {}\n", tc.skippedMessage);
    }

    static std::filesystem::path getTestReportFilePath()
    {
        auto now_tp = floor<std::chrono::seconds>(std::chrono::system_clock::now());
        std::chrono::zoned_time local_zoned{std::chrono::current_zone(), now_tp};
        std::string ts = std::format("{:%Y%m%d}", local_zoned);
        std::string filename = std::format("EditorTestResults_{}.report", ts);

        auto testDir = std::filesystem::path(Path::resolvePathRelativeToExe("../tests/editor"));
        std::filesystem::create_directories(testDir);
        auto filePath = testDir / filename;
        return filePath;
    }

    void TestWindow::writeTestReport()
    {
        const auto filePath = getTestReportFilePath();

        std::ofstream out(filePath.string());
        if (!out)
            THROW_EXCEPTION(FileWriteException, filePath.string(), std::strerror(errno));

        writeEnvironmentReport(out);

        for (auto &section : m_testSections) {
            out << std::format("# {}\n", section.name);
            for (auto &tc : section.testCases)
                writeTestCaseReport(out, tc);
            for (auto &sub : section.subSections) {
                out << std::format("## {}\n", sub.name);
                for (auto &tc : sub.testCases)
                    writeTestCaseReport(out, tc);
            }
        }
    }

    void TestWindow::shutdown()
    {
        writeTestReport();
    }

}
