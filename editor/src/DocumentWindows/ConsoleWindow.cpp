//// ConsoleWindow.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the console window class
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>
#include <format>
#include "ConsoleWindow.hpp"

namespace nexo::editor {
    static constexpr std::string verbosityToString(const loguru::Verbosity level)
    {
        switch (level)
        {
            case loguru::Verbosity_FATAL: return "[FATAL]";
            case loguru::Verbosity_ERROR: return "[ERROR]";
            case loguru::Verbosity_WARNING: return "[WARNING]";
            case loguru::Verbosity_INFO: return "[INFO]";
            case loguru::Verbosity_INVALID: return "[INVALID]";
            case loguru::Verbosity_1: return "[DEBUG]";
            case loguru::Verbosity_2: return "[DEV]";
            default: return "[UNKNOWN]";
        }
    }

    static constexpr ImVec4 getVerbosityColor(loguru::Verbosity level)
    {
        ImVec4 color;

        switch (level)
        {
            case loguru::Verbosity_FATAL: // Red
            case loguru::Verbosity_ERROR: color = ImVec4(1, 0, 0, 1);
                break; // Red
            case loguru::Verbosity_WARNING: color = ImVec4(1, 1, 0, 1);
                break; // Yellow
            case loguru::Verbosity_INFO: color = ImVec4(0, 0.5f, 1, 1);
                break; // Blue
            case loguru::Verbosity_1: color = ImVec4(0.898, 0, 1, 1); // Debug
                break; // Pink
            case loguru::Verbosity_2: color = ImVec4(0.388, 0.055, 0.851, 1); // Debug
                break; // Purple
            default: color = ImVec4(1, 1, 1, 1); // White
        }
        return color;
    }

    ConsoleWindow::~ConsoleWindow()
    {
        clearLog();
    }

    void ConsoleWindow::setup()
    {}

    void ConsoleWindow::shutdown()
    {}

    void ConsoleWindow::clearLog()
    {
        items.clear();
    }

    template <typename... Args>
    void ConsoleWindow::addLog(const char* fmt, Args&&... args)
    {
        try
        {
            std::string formattedMessage = std::format(fmt, std::forward<Args>(args)...);
            items.emplace_back(formattedMessage);
        }
        catch (const std::format_error& e)
        {
            items.emplace_back("[Error formatting log message]");
        }

        scrollToBottom = true;
    }

    void ConsoleWindow::executeCommand(const char* command_line)
    {
        commands.emplace_back(command_line);
        addLog("# {}\n", command_line);
    }

    void ConsoleWindow::show()
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin("Console", &m_opened, ImGuiWindowFlags_NoCollapse);

        const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

        static float messageStartX = 0.0f;

        if (messageStartX == 0.0f)
        {
            for (const auto &[verbosity, message, prefix]: _editor.getLogs())
            {
                if (!selectedVerbosityLevels.contains(verbosity))
                    continue;

                const std::string tag = verbosityToString(verbosity);
                const ImVec2 textSize = ImGui::CalcTextSize(tag.c_str());
                if (textSize.x > messageStartX)
                {
                    messageStartX = textSize.x;
                }
            }
            messageStartX += ImGui::GetStyle().ItemSpacing.x * 2;
        }

        auto id = 0;
        for (const auto &[verbosity, message, prefix]: _editor.getLogs())
        {
            if (!selectedVerbosityLevels.contains(verbosity))
                continue;

            ImGui::PushID(id++); {
                ImVec4 color = getVerbosityColor(verbosity);
                ImGui::PushStyleColor(ImGuiCol_Text, color);

                const std::string tag = verbosityToString(verbosity);
                ImGui::TextUnformatted(tag.c_str());
                ImGui::PopStyleColor();

                ImGui::SameLine();
                ImGui::SetCursorPosX(messageStartX);

                ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
                ImGui::TextWrapped("%s", message.c_str());
                ImGui::PopTextWrapPos();
            }
            ImGui::PopID();
        }

        if (scrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;

        ImGui::EndChild();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 60.0f);

        if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            executeCommand(inputBuf);
            std::memset(inputBuf, '\0', sizeof(inputBuf));
        }

        ImGui::SameLine();
        if (ImGui::Button("..."))
            ImGui::OpenPopup("VerbositySettings");

        if (ImGui::BeginPopup("VerbositySettings"))
        {
            ImGui::Text("Select Verbosity Levels");
            ImGui::Separator();

            const struct {
                loguru::Verbosity level;
                const char *name;
            } levels[] = {
                        {loguru::Verbosity_FATAL, "FATAL"},
                        {loguru::Verbosity_ERROR, "ERROR"},
                        {loguru::Verbosity_WARNING, "WARNING"},
                        {loguru::Verbosity_INFO, "INFO"},
                        {loguru::Verbosity_1, "DEBUG"},
                        {loguru::Verbosity_2, "DEV"},
                    };

            for (const auto &[level, name]: levels)
            {
                bool selected = (selectedVerbosityLevels.contains(level));
                if (ImGui::Checkbox(name, &selected))
                {
                    if (selected)
                    {
                        selectedVerbosityLevels.insert(level);
                    } else
                    {
                        selectedVerbosityLevels.erase(level);
                    }
                }
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }


    void ConsoleWindow::update()
    {}
}
