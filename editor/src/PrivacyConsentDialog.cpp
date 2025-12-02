//// PrivacyConsentDialog.cpp ////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Jean CARDONNE
//  Date:        05/11/2025
//  Description: Implementation of privacy consent dialog
//
///////////////////////////////////////////////////////////////////////////////

#include "PrivacyConsentDialog.hpp"
#include "Path.hpp"
#include "Logger.hpp"

#ifdef NEXO_SENTRY_ENABLED
#include "core/crash/CrashTracker.hpp"
#endif

#include <fstream>
#include <filesystem>

namespace nexo::editor {

    constexpr const char* PRIVACY_CONFIG_FILE = "../config/privacy.ini";
    constexpr const char* CONSENT_DIALOG_ID = "Privacy & Crash Reporting";

    bool PrivacyConsentDialog::hasExistingConsent() const
    {
        const std::filesystem::path configPath = Path::resolvePathRelativeToExe(PRIVACY_CONFIG_FILE);
        return std::filesystem::exists(configPath);
    }

    void PrivacyConsentDialog::saveConsent()
    {
        const std::filesystem::path configPath = Path::resolvePathRelativeToExe(PRIVACY_CONFIG_FILE);

        std::filesystem::create_directories(configPath.parent_path());

        std::ofstream configFile(configPath);
        if (!configFile.is_open()) {
            LOG(NEXO_ERROR, "Failed to save privacy consent preferences to: {}", configPath.string());
            return;
        }

        configFile << "[Privacy]\n";
        configFile << "crash_reporting=" << (m_crashReportingConsent ? "true" : "false") << "\n";
        configFile << "performance_monitoring=" << (m_performanceMonitoringConsent ? "true" : "false") << "\n";
        configFile << "consent_version=1\n";

        configFile.close();

#ifdef NEXO_SENTRY_ENABLED
        auto tracker = nexo::crash::CrashTracker::getInstance();
        if (tracker) {
            tracker->setUserConsent(m_crashReportingConsent, m_performanceMonitoringConsent);
            if (m_crashReportingConsent) {
                tracker->initialize();
            }
        }
#endif

        LOG(NEXO_INFO, "Privacy consent saved: crash_reporting={}, performance_monitoring={}",
            m_crashReportingConsent, m_performanceMonitoringConsent);
    }

    void PrivacyConsentDialog::checkAndShow()
    {
        if (!m_dialogInitialized) {
            m_shouldShow = !hasExistingConsent();
            m_dialogInitialized = true;
        }
    }

    void PrivacyConsentDialog::show()
    {
        if (!m_shouldShow)
            return;

        ImGui::OpenPopup(CONSENT_DIALOG_ID);

        ImGui::SetNextWindowSize(ImVec2(600, 0), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(CONSENT_DIALOG_ID, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {

            ImGui::TextWrapped("Welcome to NexoEngine!");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::TextWrapped(
                "To improve the engine and fix bugs more effectively, we collect anonymous "
                "crash reports when the engine encounters errors."
            );
            ImGui::Spacing();

            ImGui::TextWrapped("Data collected includes:");
            ImGui::BulletText("Stack traces and error messages");
            ImGui::BulletText("System information (OS, CPU, GPU, memory)");
            ImGui::BulletText("Engine state at the time of crash");
            ImGui::Spacing();

            ImGui::TextWrapped(
                "All data is anonymized before transmission. No personally identifiable "
                "information (PII) such as file paths, usernames, or project content is sent."
            );
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Checkbox("Enable crash reporting (recommended)", &m_crashReportingConsent);
            ImGui::Spacing();

            ImGui::Checkbox("Enable performance monitoring (optional)", &m_performanceMonitoringConsent);
            ImGui::Spacing();

            ImGui::TextWrapped(
                "You can change these preferences at any time in the editor settings."
            );
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            const float buttonWidth = 120.0f;
            const float spacing = 10.0f;
            const float totalWidth = buttonWidth * 2 + spacing;
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);

            if (ImGui::Button("Accept", ImVec2(buttonWidth, 0))) {
                saveConsent();
                m_shouldShow = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine(0, spacing);

            if (ImGui::Button("Decline", ImVec2(buttonWidth, 0))) {
                m_crashReportingConsent = false;
                m_performanceMonitoringConsent = false;
                saveConsent();
                m_shouldShow = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

}
