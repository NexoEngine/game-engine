//// PerformanceMonitorWindow.hpp ////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      iMeaNz
//  Date:        29/10/2025
//  Description: Performance monitoring window for the editor
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ADocumentWindow.hpp"
#include "SystemProfiler.hpp"
#include <imgui.h>
#include <vector>
#include <algorithm>


namespace nexo::editor {

class PerformanceMonitorWindow final : public ADocumentWindow {
public:
    using ADocumentWindow::ADocumentWindow;
    ~PerformanceMonitorWindow() override = default;

    /**
     * @brief Sets up the Performance Monitor window.
     */
    void setup() override;

    /**
     * @brief Shuts down the Performance Monitor window.
     */
    void shutdown() override;

    /**
     * @brief Renders the Performance Monitor window.
     */
    void show() override;

    /**
     * @brief Updates the Performance Monitor window.
     */
    void update() override;

    /**
     * @brief Logs performance statistics to console.
     */
    void logPerformanceStats();

private:
    bool m_sortByTime = true;
    float m_refreshRate = 1.0f; // seconds
    float m_lastRefresh = 0.0f;
    bool m_autoRefresh = true;
    bool m_showOnlyWarnings = false;
    bool m_showDetailedStats = true;

    // Chart data for performance graphs
    std::vector<float> m_frameTimes;
    std::vector<float> m_totalSystemTimes;
    static constexpr int MAX_SAMPLES = 120; // 2 seconds at 60fps

    void renderOverview();
    void renderSystemsList();
    void renderControls();
    void renderPerformanceGraphs();
    void renderSystemDetails();

    // Helper methods
    void updateChartData();
    static ImU32 getPerformanceColor(const profiling::SystemStats& stat);
    static const char* getPerformanceIcon(const profiling::SystemStats& stat);
};

} // namespace nexo::editor
