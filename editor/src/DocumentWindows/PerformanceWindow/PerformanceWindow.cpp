//// PerformanceMonitorWindow.cpp ////////////////////////////////////////////
//
//  Author:      iMeaNz
//  Date:        29/10/2025
//  Description: Implementation of Performance Monitor window
//
///////////////////////////////////////////////////////////////////////////////

#include "PerformanceWindow.hpp"
#include "IconsFontAwesome.h"
#include "Application.hpp"
#include <imgui_internal.h>

namespace nexo::editor {

void PerformanceMonitorWindow::setup()
{
    // Initialize default window state
    m_frameTimes.reserve(MAX_SAMPLES);
    m_totalSystemTimes.reserve(MAX_SAMPLES);
}

void PerformanceMonitorWindow::shutdown()
{
    // Cleanup if needed
}

void PerformanceMonitorWindow::show()
{
    beginRender(m_windowName);

    if (!m_opened) return;

    if (ImGui::Begin(m_windowName.c_str(), &m_opened, ImGuiWindowFlags_MenuBar)) {

        // Menu bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                ImGui::MenuItem("Auto Refresh", nullptr, &m_autoRefresh);
                ImGui::MenuItem("Show Only Warnings", nullptr, &m_showOnlyWarnings);
                ImGui::MenuItem("Show Detailed Stats", nullptr, &m_showDetailedStats);
                ImGui::Separator();
                if (ImGui::MenuItem("Reset Stats")) {
                    profiling::SystemProfiler::getInstance().reset();
                }
                if (ImGui::MenuItem("Log to Console")) {
                    logPerformanceStats();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        renderOverview();
        ImGui::Separator();

        if (m_showDetailedStats) {
            renderPerformanceGraphs();
            ImGui::Separator();
        }

        renderSystemsList();
        ImGui::Separator();
        renderControls();
    }
    ImGui::End();
}

void PerformanceMonitorWindow::update()
{
    if (m_autoRefresh) {
        float currentTime = ImGui::GetTime();
        if (currentTime - m_lastRefresh >= m_refreshRate) {
            updateChartData();
            m_lastRefresh = currentTime;
        }
    }
}

void PerformanceMonitorWindow::renderOverview()
{
    auto stats = profiling::SystemProfiler::getInstance().getAllStats();

    double totalTime = 0.0;
    size_t totalEntities = 0;
    size_t errorCount = 0;
    size_t warningCount = 0;
    size_t activeSystemCount = 0;

    for (const auto& stat : stats) {
        if (!stat.isActive) continue;
        activeSystemCount++;
        totalTime += stat.executionTimeMs;
        totalEntities += stat.entitiesProcessed;
        if (stat.isError()) errorCount++;
        else if (stat.isWarning()) warningCount++;
    }

    // Performance overview cards
    ImGui::BeginChild("Overview", ImVec2(0, 80), true);

    float cardWidth = (ImGui::GetContentRegionAvail().x - 30) / 4;

    // Total Systems Card
    ImGui::BeginChild("SystemsCard", ImVec2(cardWidth, 60), true);
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Assume this is a larger font
    ImGui::TextColored(ImVec4(0.7f, 0.9f, 1.0f, 1.0f), ICON_FA_COG);
    ImGui::SameLine();
    ImGui::Text("%zu", activeSystemCount);
    ImGui::PopFont();
    ImGui::Text("Active Systems");
    ImGui::EndChild();

    ImGui::SameLine();

    // Total Time Card
    ImGui::BeginChild("TimeCard", ImVec2(cardWidth, 60), true);
    ImVec4 timeColor = totalTime > 16.67 ? ImVec4(1.0f, 0.4f, 0.4f, 1.0f) : ImVec4(0.4f, 1.0f, 0.4f, 1.0f);
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::TextColored(timeColor, ICON_FA_CLOCK_O);
    ImGui::SameLine();
    ImGui::Text("%.2fms", totalTime);
    ImGui::PopFont();
    ImGui::Text("Total Time");
    ImGui::EndChild();

    ImGui::SameLine();

    // Entities Card
    ImGui::BeginChild("EntitiesCard", ImVec2(cardWidth, 60), true);
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.4f, 1.0f), ICON_FA_CUBE);
    ImGui::SameLine();
    ImGui::Text("%zu", totalEntities);
    ImGui::PopFont();
    ImGui::Text("Entities");
    ImGui::EndChild();

    ImGui::SameLine();

    // Issues Card
    ImGui::BeginChild("IssuesCard", ImVec2(cardWidth, 60), true);
    ImVec4 issueColor = (errorCount + warningCount) > 0 ? ImVec4(1.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.4f, 1.0f, 0.4f, 1.0f);
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::TextColored(issueColor, ICON_FA_EXCLAMATION_TRIANGLE);
    ImGui::SameLine();
    ImGui::Text("%zu", errorCount + warningCount);
    ImGui::PopFont();
    ImGui::Text("Issues");
    ImGui::EndChild();

    ImGui::EndChild();
}

void PerformanceMonitorWindow::renderPerformanceGraphs()
{
    if (m_frameTimes.empty()) return;

    ImGui::Text("Performance Graphs");

    // Frame time graph
    ImGui::PlotLines("Frame Times (ms)", m_frameTimes.data(), static_cast<int>(m_frameTimes.size()),
                     0, nullptr, 0.0f, 20.0f, ImVec2(0, 80));

    // Total system time graph
    ImGui::PlotLines("Total System Time (ms)", m_totalSystemTimes.data(), static_cast<int>(m_totalSystemTimes.size()),
                     0, nullptr, 0.0f, 20.0f, ImVec2(0, 80));
}

void PerformanceMonitorWindow::renderSystemsList()
{
    auto stats = profiling::SystemProfiler::getInstance().getAllStats();

    // Filter stats if needed
    if (m_showOnlyWarnings) {
        stats.erase(std::remove_if(stats.begin(), stats.end(),
                                  [](const profiling::SystemStats& stat) {
                                      return !stat.isWarning() && !stat.isError();
                                  }), stats.end());
    }

    // Sort stats
    if (m_sortByTime) {
        std::sort(stats.begin(), stats.end(),
                 [](const profiling::SystemStats& a, const profiling::SystemStats& b) {
                     return a.executionTimeMs > b.executionTimeMs;
                 });
    } else {
        std::sort(stats.begin(), stats.end(),
                 [](const profiling::SystemStats& a, const profiling::SystemStats& b) {
                     return a.name < b.name;
                 });
    }

    if (ImGui::BeginTable("SystemStats", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                         ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY)) {

        ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 30.0f);
        ImGui::TableSetupColumn("System Name", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("Time (ms)", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Entities", ImGuiTableColumnFlags_WidthFixed, 70.0f);
        ImGui::TableSetupColumn("Avg (ms)", ImGuiTableColumnFlags_WidthFixed, 70.0f);
        ImGui::TableSetupColumn("Min/Max (ms)", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Calls", ImGuiTableColumnFlags_WidthFixed, 60.0f);
        ImGui::TableHeadersRow();

        for (const auto& stat : stats) {
            if (!stat.isActive) continue;

            ImGui::TableNextRow();

            // Status icon
            ImGui::TableNextColumn();
            ImGui::TextColored(ImColor(getPerformanceColor(stat)), "%s", getPerformanceIcon(stat));

            // System name
            ImGui::TableNextColumn();
            ImGui::Text("%s", stat.name.c_str());

            // Current execution time
            ImGui::TableNextColumn();
            ImGui::TextColored(ImColor(getPerformanceColor(stat)), "%.3f", stat.executionTimeMs);

            // Entities processed
            ImGui::TableNextColumn();
            ImGui::Text("%zu", stat.entitiesProcessed);

            // Average time
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", stat.avgExecutionTimeMs);

            // Min/Max time
            ImGui::TableNextColumn();
            ImGui::Text("%.3f/%.3f", stat.minExecutionTimeMs, stat.maxExecutionTimeMs);

            // Call count
            ImGui::TableNextColumn();
            ImGui::Text("%zu", stat.callCount);
        }

        ImGui::EndTable();
    }
}

void PerformanceMonitorWindow::renderControls()
{
    ImGui::Text("Controls");

    bool enabled = profiling::SystemProfiler::getInstance().isEnabled();
    if (ImGui::Checkbox("Enable Profiling", &enabled)) {
        profiling::SystemProfiler::getInstance().setEnabled(enabled);
    }
    ImGui::SameLine();

    ImGui::Checkbox("Sort by Time", &m_sortByTime);
    ImGui::SameLine();

    if (ImGui::Button(ICON_FA_REFRESH " Reset Stats")) {
        profiling::SystemProfiler::getInstance().reset();
    }
    ImGui::SameLine();

    if (ImGui::Button(ICON_FA_TERMINAL " Log to Console")) {
        logPerformanceStats();
    }

    ImGui::SliderFloat("Refresh Rate", &m_refreshRate, 0.1f, 5.0f, "%.1fs");
}

void PerformanceMonitorWindow::updateChartData()
{
    auto stats = profiling::SystemProfiler::getInstance().getAllStats();

    double totalTime = 0.0;
    for (const auto& stat : stats) {
        if (stat.isActive) {
            totalTime += stat.executionTimeMs;
        }
    }

    // Add new data points
    m_totalSystemTimes.push_back(static_cast<float>(totalTime));
    m_frameTimes.push_back(ImGui::GetIO().DeltaTime * 1000.0f); // Convert to ms

    // Keep only the last MAX_SAMPLES
    if (m_totalSystemTimes.size() > MAX_SAMPLES) {
        m_totalSystemTimes.erase(m_totalSystemTimes.begin());
    }
    if (m_frameTimes.size() > MAX_SAMPLES) {
        m_frameTimes.erase(m_frameTimes.begin());
    }
}

ImU32 PerformanceMonitorWindow::getPerformanceColor(const profiling::SystemStats& stat)
{
    if (stat.isError()) return IM_COL32(255, 100, 100, 255);
    if (stat.isWarning()) return IM_COL32(255, 200, 100, 255);
    return IM_COL32(100, 255, 100, 255);
}

const char* PerformanceMonitorWindow::getPerformanceIcon(const profiling::SystemStats& stat)
{
    if (stat.isError()) return ICON_FA_TIMES_CIRCLE;
    if (stat.isWarning()) return ICON_FA_EXCLAMATION_TRIANGLE;
    return ICON_FA_CHECK_CIRCLE;
}

void PerformanceMonitorWindow::logPerformanceStats()
{
    auto stats = profiling::SystemProfiler::getInstance().getAllStats();
    if (stats.empty()) return;

    LOG(NEXO_INFO, "=== System Performance Report ===");

    // Sort by execution time (descending)
    std::sort(stats.begin(), stats.end(),
             [](const profiling::SystemStats& a, const profiling::SystemStats& b) {
                 return a.executionTimeMs > b.executionTimeMs;
             });

    for (const auto& stat : stats) {
        if (!stat.isActive) continue;

        std::string status = "";
        if (stat.isError()) status = "[ERROR] ";
        else if (stat.isWarning()) status = "[WARN] ";

        LOG(NEXO_INFO, "{}{}: {:.3f}ms ({} entities) - Avg: {:.3f}ms Min: {:.3f}ms Max: {:.3f}ms",
            status, stat.name, stat.executionTimeMs, stat.entitiesProcessed,
            stat.avgExecutionTimeMs, stat.minExecutionTimeMs, stat.maxExecutionTimeMs);
    }

    LOG(NEXO_INFO, "=== End Performance Report ===");
}

} // namespace nexo::editor
