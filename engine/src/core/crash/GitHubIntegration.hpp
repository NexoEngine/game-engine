//// GitHubIntegration.hpp ////////////////////////////////////////////////////
//
//  Author:      Jean CARDONNE
//  Date:        05/11/2025
//  Description: GitHub Issues integration for crash deduplication (Stub)
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <cstdint>

namespace nexo::crash {

    class GitHubIntegration {
    public:
        static std::string computeCrashSignature(const std::string& stackTrace, const std::string& exceptionType);

        static bool shouldCreateIssue(const std::string& signature, uint32_t occurrenceCount);

        static void createIssue(const std::string& signature, const std::string& stackTrace,
                               uint32_t userCount, const std::string& sentryUrl);

        static void notifyUser(const std::string& issueUrl);

    private:
        static constexpr uint32_t ISSUE_THRESHOLD = 3;
        static constexpr uint64_t RATE_LIMIT_MS = 3600000;
    };

}
