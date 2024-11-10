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
            case loguru::Verbosity_MAX: color = ImVec4(0, 1, 0, 1);
                break; // Green
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
        VLOG_F(loguru::Verbosity_INFO,
               "ConsoleWindow setup\nLorem ipsum dolor sit amet\nconsectetur adipiscing elit\nsed do eiusmod tempor incididunt\nut labore et dolore magna aliqua\nUt enim ad minim veniam\nquis nostrud exercitation ullamco\nlaboris nisi ut aliquip ex ea commodo consequat\nDuis aute irure dolor in reprehenderit\nin voluptate velit esse cillum dolore eu fugiat nulla pariatur\nExcepteur sint occaecat cupidatat non proident\nsunt in culpa qui officia deserunt mollit anim id est laborum");
    }

    void ConsoleWindow::shutdown()
    {
    }

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

        // Reserve enough space for input box
        const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

        auto id = 0;
        for (const auto &[verbosity, message, prefix]: _editor.getLogs())
        {
            ImGui::PushID(id++); {
                auto &msgtext = message;
                ImVec2 text_size = ImGui::CalcTextSize(msgtext.c_str(), msgtext.c_str() + msgtext.size());
                text_size.x = -FLT_MIN; // fill width (suppresses label)
                text_size.y += ImGui::GetStyle().FramePadding.y; // single pad

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0}); // make align with text height
                ImGui::PushStyleColor(ImGuiCol_FrameBg, {0.f, 0.f, 0.f, 0.f}); // remove text input box
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0)); // Set border color to transparent
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); // Set border size to 0
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 0)); // Set item spacing to 0

                ImVec4 color = getVerbosityColor(verbosity);

                // Split the message into verbosity tag and actual message
                std::string tag = "[" + std::string(verbosityToString(verbosity)) + "]";
                //std::string message = item.message.substr(tag.length());  // assuming item.message starts with the tag

                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(tag.c_str());
                ImGui::PopStyleColor();

                ImGui::SameLine();
                ImGui::InputTextMultiline(
                    "",
                    const_cast<char *>(msgtext.c_str()), // ugly const cast
                    msgtext.size() + 1, // needs to include '\0'
                    text_size,
                    ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoHorizontalScroll
                );

                ImGui::PopStyleVar();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
            }
            ImGui::PopID();
        }

        if (scrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;

        ImGui::EndChild();

        if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            executeCommand(inputBuf);
            strcpy(inputBuf, "");
        }

        ImGui::End();
    }

    void ConsoleWindow::update()
    {
    }
}
