//// SystemProfiler.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      iMeaNz
//  Date:        29/10/2025
//  Description: System profiler for performance monitoring
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>

namespace nexo::profiling {

struct SystemStats {
    std::string name;
    double executionTimeMs = 0.0;
    size_t entitiesProcessed = 0;
    size_t memoryUsageBytes = 0;
    double avgExecutionTimeMs = 0.0;
    double minExecutionTimeMs = 999999.0;
    double maxExecutionTimeMs = 0.0;
    size_t callCount = 0;
    bool isActive = true;

    // Performance thresholds
    double warningThresholdMs = 1.0;  // Warn if execution takes > 1ms
    double errorThresholdMs = 5.0;    // Error if execution takes > 5ms

    void update(double timeMs, size_t entities = 0) {
        executionTimeMs = timeMs;
        entitiesProcessed = entities;
        callCount++;

        // Update running averages
        avgExecutionTimeMs = (avgExecutionTimeMs * (callCount - 1) + timeMs) / callCount;
        minExecutionTimeMs = std::min(minExecutionTimeMs, timeMs);
        maxExecutionTimeMs = std::max(maxExecutionTimeMs, timeMs);
    }

    [[nodiscard]] bool isWarning() const { return executionTimeMs > warningThresholdMs; }
    [[nodiscard]] bool isError() const { return executionTimeMs > errorThresholdMs; }
};

class SystemProfiler {
public:
    static SystemProfiler& getInstance() {
        static SystemProfiler instance;
        return instance;
    }

    // Start timing a system
    void startTiming(const std::string& systemName) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_currentTimings[systemName] = std::chrono::high_resolution_clock::now();
    }

    // End timing and record stats
    void endTiming(const std::string& systemName, size_t entitiesProcessed = 0) {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_currentTimings.find(systemName);
        if (it != m_currentTimings.end()) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - it->second);
            double timeMs = duration.count() / 1000.0;

            if (m_systemStats.find(systemName) == m_systemStats.end()) {
                m_systemStats[systemName] = SystemStats{};
                m_systemStats[systemName].name = systemName;
            }

            m_systemStats[systemName].update(timeMs, entitiesProcessed);
            m_currentTimings.erase(it);
        }
    }

    // Get all system stats
    [[nodiscard]] std::vector<SystemStats> getAllStats() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<SystemStats> stats;
        stats.reserve(m_systemStats.size());

        for (const auto& [name, stat] : m_systemStats) {
            stats.push_back(stat);
        }

        return stats;
    }

    // Get specific system stats
    [[nodiscard]] SystemStats getSystemStats(const std::string& systemName) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_systemStats.find(systemName);
        return it != m_systemStats.end() ? it->second : SystemStats{};
    }

    // Reset all stats
    void reset() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_systemStats.clear();
        m_currentTimings.clear();
    }

    // Enable/disable profiling
    void setEnabled(bool enabled) { m_enabled = enabled; }
    [[nodiscard]] bool isEnabled() const { return m_enabled; }

private:
    mutable std::mutex m_mutex;
    std::unordered_map<std::string, SystemStats> m_systemStats;
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> m_currentTimings;
    bool m_enabled = true;

    SystemProfiler() = default;
};

// RAII Timer class for automatic timing
class SystemTimer {
public:
    SystemTimer(const std::string& systemName, size_t entitiesProcessed = 0)
        : m_systemName(systemName), m_entitiesProcessed(entitiesProcessed) {
        if (SystemProfiler::getInstance().isEnabled()) {
            SystemProfiler::getInstance().startTiming(m_systemName);
        }
    }

    ~SystemTimer() {
        if (SystemProfiler::getInstance().isEnabled()) {
            SystemProfiler::getInstance().endTiming(m_systemName, m_entitiesProcessed);
        }
    }

    // Update entity count during execution
    void setEntitiesProcessed(size_t count) { m_entitiesProcessed = count; }

private:
    std::string m_systemName;
    size_t m_entitiesProcessed;
};

// Scope-based timer for fine-grained profiling
class ScopeTimer {
public:
    ScopeTimer(const std::string& scopeName, size_t entities = 0)
        : m_scopeName(scopeName), m_entities(entities) {
        if (SystemProfiler::getInstance().isEnabled()) {
            SystemProfiler::getInstance().startTiming(m_scopeName);
        }
    }

    ~ScopeTimer() {
        if (SystemProfiler::getInstance().isEnabled()) {
            SystemProfiler::getInstance().endTiming(m_scopeName, m_entities);
        }
    }

private:
    std::string m_scopeName;
    size_t m_entities;
};

} // namespace nexo::profiling

// Convenience macros for automatic timing
#define PROFILE_SYSTEM(name, entities) nexo::profiling::SystemTimer _timer(name, entities)
#define PROFILE_SCOPE(name) nexo::profiling::ScopeTimer _scope_timer(name)
#define PROFILE_SCOPE_ENTITIES(name, entities) nexo::profiling::ScopeTimer _scope_timer(name, entities)
