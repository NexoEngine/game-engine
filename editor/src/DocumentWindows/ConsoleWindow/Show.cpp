//// Show.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the rendering of the console window
//
///////////////////////////////////////////////////////////////////////////////

#include "ConsoleWindow.hpp"
#include "IconsFontAwesome.h"
#include "ImNexo/Elements.hpp"
#include "utils/FileSystem.hpp"
#include "Path.hpp"

namespace nexo::editor {

    void ConsoleWindow::showVerbositySettingsPopup()
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
            {loguru::Verbosity_1, "USER"},
            {loguru::Verbosity_2, "DEBUG"},
            {loguru::Verbosity_3, "DEV"}
        };

        for (const auto &[level, name]: levels)
        {
            bool selected = (m_selectedVerbosityLevels.contains(level));
            if (ImGui::Checkbox(name, &selected))
            {
                if (selected)
                {
                    m_selectedVerbosityLevels.insert(level);
                    calcLogPadding();
                } else
                {
                    m_selectedVerbosityLevels.erase(level);
                    calcLogPadding();
                }
            }
        }

        ImGui::Separator();
        ImGui::Checkbox("File logging", &m_exportLog);
        if (ImNexo::Button("Open log folder"))
            utils::openFolder(Path::resolvePathRelativeToExe("../logs").string());

        ImGui::EndPopup();
    }

    void ConsoleWindow::executeCommand(const char *commandLine)
    {
        m_commands.emplace_back(commandLine);
        addLog("{}", commandLine);
    }

    void ConsoleWindow::calcLogPadding()
    {
        m_logPadding = 0.0f;

        for (const auto &selectedLevel : m_selectedVerbosityLevels) {
            const std::string tag = verbosityToString(selectedLevel);
            const ImVec2 textSize = ImGui::CalcTextSize(tag.c_str());
            if (textSize.x > m_logPadding)
                m_logPadding = textSize.x;
        }
        m_logPadding += ImGui::GetStyle().ItemSpacing.x;
    }

    void ConsoleWindow::displayLog(const loguru::Verbosity verbosity, const std::string &msg) const
    {
        const ImVec4 color = getVerbosityColor(verbosity);
        ImGui::PushStyleColor(ImGuiCol_Text, color);

        const std::string tag = verbosityToString(verbosity);
        ImGui::TextUnformatted(tag.c_str());
        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::SetCursorPosX(m_logPadding);

        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped("%s", msg.c_str());
        ImGui::PopTextWrapPos();
    }

    void ConsoleWindow::show()
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin(ICON_FA_FILE_TEXT " Console" NEXO_WND_USTRID_CONSOLE, &m_opened, ImGuiWindowFlags_NoCollapse);
        beginRender(NEXO_WND_USTRID_CONSOLE);

        const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (m_logPadding == 0.0f)
            calcLogPadding();

        auto id = 0;
        for (const auto &[verbosity, message, prefix]: m_logs)
        {
            if (!m_selectedVerbosityLevels.contains(verbosity))
                continue;

            ImGui::PushID(id++);
            displayLog(verbosity, message);
            ImGui::PopID();
        }

        if (m_scrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        m_scrollToBottom = false;

        ImGui::EndChild();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 60.0f);

        if (ImGui::InputText("Input", m_inputBuf, IM_ARRAYSIZE(m_inputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            executeCommand(m_inputBuf);
            std::memset(m_inputBuf, '\0', sizeof(m_inputBuf));
        }

        ImGui::SameLine();
        if (ImNexo::Button("..."))
            ImGui::OpenPopup("VerbositySettings");

        if (ImGui::BeginPopup("VerbositySettings"))
            showVerbositySettingsPopup();

        ImGui::End();
    }
}
