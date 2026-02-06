//// GitHubIntegration.cpp ////////////////////////////////////////////////////
//
//  Author:      Jean CARDONNE
//  Date:        05/11/2025
//  Description: GitHub Issues integration implementation (Stub)
//
///////////////////////////////////////////////////////////////////////////////

#include "GitHubIntegration.hpp"
#include <functional>

namespace nexo::crash {

    std::string GitHubIntegration::computeCrashSignature(const std::string& stackTrace, const std::string& exceptionType) {
        std::hash<std::string> hasher;
        std::string combined = exceptionType + ":" + stackTrace;
        return std::to_string(hasher(combined));
    }

    bool GitHubIntegration::shouldCreateIssue(const std::string& signature, uint32_t occurrenceCount) {
        return occurrenceCount >= ISSUE_THRESHOLD;
    }

    void GitHubIntegration::createIssue(const std::string& signature, const std::string& stackTrace,
                                        uint32_t userCount, const std::string& sentryUrl) {
    }

    void GitHubIntegration::notifyUser(const std::string& issueUrl) {
    }

}
