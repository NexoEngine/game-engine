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
#include "ConsoleWindow.hpp"

namespace nexo::editor {
    static constexpr const char *verbosityToString(const loguru::Verbosity level)
    {
        switch (level)
        {
            case loguru::Verbosity_FATAL: return "FATAL";
            case loguru::Verbosity_ERROR: return "ERROR";
            case loguru::Verbosity_WARNING: return "WARNING";
            case loguru::Verbosity_INFO: return "INFO";
            case loguru::Verbosity_INVALID: return "INVALID";
            case loguru::Verbosity_1: return "DEBUG";
            default: return "UNKNOWN";
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
            default: color = ImVec4(1, 1, 1, 1); // White
        }
        return color;
    }

    ConsoleWindow::~ConsoleWindow()
    {
        clearLog();
    }

    void ConsoleWindow::setup()
    {
    }

    void ConsoleWindow::shutdown()
    {}

    void ConsoleWindow::clearLog()
    {
        items.clear();
    }

    void ConsoleWindow::addLog(const char *fmt, ...)
    {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        items.emplace_back(buf);

        scrollToBottom = true;
    }

    void ConsoleWindow::executeCommand(const char *command_line)
    {
        commands.emplace_back(command_line);

        addLog("# %s\n", command_line);
    }

    void ConsoleWindow::show()
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin("Console", &m_opened, ImGuiWindowFlags_NoCollapse);

        const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

        auto id = 0;
        for (const auto &[verbosity, message, prefix]: _editor.getLogs())
        {
            if (verbosity > loguru::g_stderr_verbosity)
                continue;
            ImGui::PushID(id++);
            {
                const float availableWidth = ImGui::GetContentRegionAvail().x;

                ImVec4 color = getVerbosityColor(verbosity);
                ImGui::PushStyleColor(ImGuiCol_Text, color);

                std::string tag = "[" + std::string(verbosityToString(verbosity)) + "]";
                ImGui::TextUnformatted(tag.c_str());
                ImGui::PopStyleColor();

                ImGui::SameLine();

                ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + availableWidth);

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
            strcpy(inputBuf, "");
        }

        ImGui::SameLine();
        if (ImGui::Button("..."))
            ImGui::OpenPopup("VerbositySettings");

        if (ImGui::BeginPopup("VerbositySettings"))
        {
            ImGui::Text("Set Verbosity Level");
            ImGui::Separator();

            static loguru::Verbosity currentVerbosity = loguru::Verbosity_INFO;

            const struct {
                loguru::Verbosity level;
                const char *name;
            } levels[] = {
                        {loguru::Verbosity_FATAL, "FATAL"},
                        {loguru::Verbosity_ERROR, "ERROR"},
                        {loguru::Verbosity_WARNING, "WARNING"},
                        {loguru::Verbosity_INFO, "INFO"},
                        {loguru::Verbosity_1, "DEBUG"},
                    };

            for (const auto &[level, name]: levels)
            {
                const bool selected = (currentVerbosity == level);
                if (ImGui::Selectable(name, selected))
                {
                    currentVerbosity = level;
                    loguru::g_stderr_verbosity = currentVerbosity;
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }

    void ConsoleWindow::update()
    {}
}
