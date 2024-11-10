/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** ConsoleWindow.cpp
*/

#include <imgui.h>

#include "editor/DocumentWindows/ConsoleWindow.hpp"

#include <editor/GameEngineEditor.hpp>

namespace engine::editor {

    static constexpr const char* verbosityToString(loguru::Verbosity level) {
        switch (level) {
            case loguru::Verbosity_FATAL:   return "[FATAL]";
            case loguru::Verbosity_ERROR:   return "[ERROR]";
            case loguru::Verbosity_WARNING: return "[WARNING]";
            case loguru::Verbosity_INFO:    return "[INFO]";
            case loguru::Verbosity_INVALID:   return "[INVALID]";
            // Add more cases if there are other verbosity levels you use.
            default:                        return "[UNKNOWN]";
        }
    }

    static constexpr ImVec4 getVerbosityColor(loguru::Verbosity level)
    {
        ImVec4 color;

        switch (level) {
            case loguru::Verbosity_FATAL:    // Red
            case loguru::Verbosity_ERROR:   color = ImVec4(1, 0, 0, 1); break; // Red
            case loguru::Verbosity_WARNING: color = ImVec4(1, 1, 0, 1); break; // Yellow
            case loguru::Verbosity_INFO:    color = ImVec4(0, 0.5f, 1, 1); break; // Blue
            case loguru::Verbosity_MAX:     color = ImVec4(0, 1, 0, 1); break; // Green
            default:                        color = ImVec4(1, 1, 1, 1); // White
        }
        return color;
    }

    ConsoleWindow::~ConsoleWindow() {
        clearLog();
    }

    void ConsoleWindow::setup() {
        // setup code (if any)
        // create a multiline lorem ipsum string
        VLOG_F(loguru::Verbosity_INFO, "ConsoleWindow setup\nLorem ipsum dolor sit amet\nconsectetur adipiscing elit\nsed do eiusmod tempor incididunt\nut labore et dolore magna aliqua\nUt enim ad minim veniam\nquis nostrud exercitation ullamco\nlaboris nisi ut aliquip ex ea commodo consequat\nDuis aute irure dolor in reprehenderit\nin voluptate velit esse cillum dolore eu fugiat nulla pariatur\nExcepteur sint occaecat cupidatat non proident\nsunt in culpa qui officia deserunt mollit anim id est laborum");
    }

    void ConsoleWindow::shutdown() {
        // shutdown code (if any)
    }

    void ConsoleWindow::clearLog() {
        items.clear();
    }

    void ConsoleWindow::addLog(const char* fmt, ...) {
        // Store the new log line
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        items.push_back(std::string(buf));

        // Scroll to bottom
        scrollToBottom = true;
    }

    void ConsoleWindow::executeCommand(const char* command_line) {
        // Add to history and execute command
        commands.push_back(std::string(command_line));

        // Execute logic here (expand as needed)
        addLog("# %s\n", command_line);
    }

    void ConsoleWindow::show() {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin("Console", &_opened, ImGuiWindowFlags_NoCollapse);

        // Reserve enough space for input box
        const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

        // Display logs
        auto id = 0;
        for (auto& item : _editor.getLogs()) {
            ImGui::PushID(id++);
            {
                auto &msgtext = item.message;
                ImVec2 text_size = ImGui::CalcTextSize(msgtext.c_str(), msgtext.c_str()+msgtext.size());
                text_size.x = -FLT_MIN; // fill width (suppresses label)
                text_size.y += ImGui::GetStyle().FramePadding.y; // single pad

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0}); // make align with text height
                ImGui::PushStyleColor(ImGuiCol_FrameBg, {0.f, 0.f, 0.f, 0.f}); // remove text input box
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0)); // Set border color to transparent
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); // Set border size to 0
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 0)); // Set item spacing to 0

                // Render the verbosity tag with color
                ImGui::PushStyleColor(ImGuiCol_Text, getVerbosityColor(item.verbosity));
                ImGui::TextUnformatted(verbosityToString(item.verbosity));
                ImGui::PopStyleColor();
                //ImGui::PushStyleColor(ImGuiCol_Text, color);

                ImGui::SameLine();
                ImGui::InputTextMultiline(
                    "",
                    const_cast<char*>(msgtext.c_str()), // ugly const cast
                    msgtext.size() + 1, // needs to include '\0'
                    text_size,
                    ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoHorizontalScroll
                );
                //ImGui::PopStyleColor();

                /*
                ImGui::InputTextMultiline("##ConsoleOutput",
                    const_cast<char*>(item.c_str()),
                    item.size() + 1,
                    ImVec2(-FLT_MIN, text_size.y),
                    ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoHorizontalScroll));
                    */


                ImGui::PopStyleVar();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
            }
            ImGui::PopID();
            /*ImGui::TextColored(getVerbosityColor(item.verbosity),
                "[%s] %s", verbosityToString(item.verbosity),
                item.message.c_str());*/
        }

        // Scroll to bottom if required
        if (scrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;

        ImGui::EndChild();

        // Command line
        if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
            executeCommand(inputBuf);
            strcpy(inputBuf, "");
        }

        ImGui::End();
    }

    void ConsoleWindow::update() {
        // Update logic (if any)
    }

}
