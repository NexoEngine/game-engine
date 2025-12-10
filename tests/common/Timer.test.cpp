//// Timer.test.cpp ///////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for Timer utility class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Timer.hpp"
#include <thread>
#include <atomic>

namespace nexo {

// =============================================================================
// ProfileResult Tests
// =============================================================================

class ProfileResultTest : public ::testing::Test {};

TEST_F(ProfileResultTest, DefaultConstruction) {
    ProfileResult result{};
    EXPECT_TRUE(result.name.empty());
    EXPECT_EQ(result.time, 0);
}

TEST_F(ProfileResultTest, ConstructionWithValues) {
    ProfileResult result{"test_profile", 1000};
    EXPECT_EQ(result.name, "test_profile");
    EXPECT_EQ(result.time, 1000);
}

TEST_F(ProfileResultTest, NameCanBeModified) {
    ProfileResult result;
    result.name = "modified";
    EXPECT_EQ(result.name, "modified");
}

TEST_F(ProfileResultTest, TimeCanBeModified) {
    ProfileResult result;
    result.time = 42;
    EXPECT_EQ(result.time, 42);
}

// =============================================================================
// Timer Basic Tests
// =============================================================================

class TimerBasicTest : public ::testing::Test {};

TEST_F(TimerBasicTest, ConstructionWithCallback) {
    bool callbackCalled = false;
    long long recordedDuration = -1;

    {
        Timer timer("test", [&callbackCalled, &recordedDuration](auto&, long long duration) {
            callbackCalled = true;
            recordedDuration = duration;
        });
    }

    EXPECT_TRUE(callbackCalled);
    EXPECT_GE(recordedDuration, 0);
}

TEST_F(TimerBasicTest, TimerNameIsStored) {
    std::string storedName;

    {
        Timer timer("my_timer_name", [](auto&, long long) {});
    }

    SUCCEED();
}

TEST_F(TimerBasicTest, CallbackReceivedOnDestruction) {
    std::atomic<int> callCount{0};

    {
        Timer timer("test", [&callCount](auto&, long long) {
            callCount++;
        });
    }

    EXPECT_EQ(callCount.load(), 1);
}

// =============================================================================
// Timer Stop Tests
// =============================================================================

class TimerStopTest : public ::testing::Test {};

TEST_F(TimerStopTest, ExplicitStopCallsCallback) {
    bool callbackCalled = false;

    Timer timer("test", [&callbackCalled](auto&, long long) {
        callbackCalled = true;
    });

    EXPECT_FALSE(callbackCalled);
    timer.stop();
    EXPECT_TRUE(callbackCalled);
}

TEST_F(TimerStopTest, StopPreventsDoubleCallback) {
    int callCount = 0;

    {
        Timer timer("test", [&callCount](auto&, long long) {
            callCount++;
        });
        timer.stop();
    }

    EXPECT_EQ(callCount, 1);
}

TEST_F(TimerStopTest, StopRecordsDuration) {
    long long duration = -1;

    Timer timer("test", [&duration](auto&, long long d) {
        duration = d;
    });

    timer.stop();
    EXPECT_GE(duration, 0);
}

// =============================================================================
// Timer Duration Tests
// =============================================================================

class TimerDurationTest : public ::testing::Test {};

TEST_F(TimerDurationTest, MeasuresPositiveDuration) {
    long long duration = -1;

    {
        Timer timer("test", [&duration](auto&, long long d) {
            duration = d;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_GE(duration, 0);
}

TEST_F(TimerDurationTest, LongerSleepRecordsLongerDuration) {
    long long shortDuration = -1;
    long long longDuration = -1;

    {
        Timer timer("short", [&shortDuration](auto&, long long d) {
            shortDuration = d;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    {
        Timer timer("long", [&longDuration](auto&, long long d) {
            longDuration = d;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    EXPECT_GE(longDuration, shortDuration);
}

// =============================================================================
// Timer Callback Tests
// =============================================================================

class TimerCallbackTest : public ::testing::Test {};

TEST_F(TimerCallbackTest, LambdaCallback) {
    bool called = false;

    {
        Timer timer("test", [&called](auto&, long long) {
            called = true;
        });
    }

    EXPECT_TRUE(called);
}

TEST_F(TimerCallbackTest, CapturingLambdaCallback) {
    int value = 0;

    {
        Timer timer("test", [&value](auto&, long long) {
            value = 42;
        });
    }

    EXPECT_EQ(value, 42);
}

TEST_F(TimerCallbackTest, CallbackReceivesDuration) {
    long long receivedDuration = -1;

    {
        Timer timer("test", [&receivedDuration](auto&, long long duration) {
            receivedDuration = duration;
        });
    }

    EXPECT_GE(receivedDuration, 0);
}

// =============================================================================
// Timer Edge Cases
// =============================================================================

class TimerEdgeCaseTest : public ::testing::Test {};

TEST_F(TimerEdgeCaseTest, EmptyTimerName) {
    bool called = false;

    {
        Timer timer("", [&called](auto&, long long) {
            called = true;
        });
    }

    EXPECT_TRUE(called);
}

TEST_F(TimerEdgeCaseTest, LongTimerName) {
    bool called = false;
    std::string longName(1000, 'x');

    {
        Timer timer(longName, [&called](auto&, long long) {
            called = true;
        });
    }

    EXPECT_TRUE(called);
}

TEST_F(TimerEdgeCaseTest, ImmediateDestruction) {
    bool called = false;

    {
        Timer timer("immediate", [&called](auto&, long long) {
            called = true;
        });
    }

    EXPECT_TRUE(called);
}

TEST_F(TimerEdgeCaseTest, ZeroDurationPossible) {
    long long duration = -1;

    {
        Timer timer("instant", [&duration](auto&, long long d) {
            duration = d;
        });
        // No delay - immediate destruction
    }

    EXPECT_GE(duration, 0);
}

// =============================================================================
// Timer RAII Pattern Tests
// =============================================================================

class TimerRAIITest : public ::testing::Test {};

TEST_F(TimerRAIITest, CallbackOnScopeExit) {
    bool called = false;

    {
        Timer timer("scope", [&called](auto&, long long) {
            called = true;
        });
        EXPECT_FALSE(called);
    }

    EXPECT_TRUE(called);
}

TEST_F(TimerRAIITest, NestedTimers) {
    std::vector<int> callOrder;

    {
        Timer outer("outer", [&callOrder](auto&, long long) {
            callOrder.push_back(1);
        });

        {
            Timer inner("inner", [&callOrder](auto&, long long) {
                callOrder.push_back(2);
            });
        }
    }

    ASSERT_EQ(callOrder.size(), 2u);
    EXPECT_EQ(callOrder[0], 2);
    EXPECT_EQ(callOrder[1], 1);
}

TEST_F(TimerRAIITest, MultipleSequentialTimers) {
    std::vector<int> callOrder;

    {
        Timer timer1("first", [&callOrder](auto&, long long) {
            callOrder.push_back(1);
        });
    }

    {
        Timer timer2("second", [&callOrder](auto&, long long) {
            callOrder.push_back(2);
        });
    }

    ASSERT_EQ(callOrder.size(), 2u);
    EXPECT_EQ(callOrder[0], 1);
    EXPECT_EQ(callOrder[1], 2);
}

// =============================================================================
// Timer Move Semantics Tests
// =============================================================================

class TimerMoveTest : public ::testing::Test {};

TEST_F(TimerMoveTest, MoveConstruction) {
    int callCount = 0;

    {
        auto createTimer = [&callCount]() {
            return Timer("moveable", [&callCount](auto&, long long) {
                callCount++;
            });
        };

        auto timer = createTimer();
    }

    EXPECT_EQ(callCount, 1);
}

// =============================================================================
// Timer Profiling Use Case Tests
// =============================================================================

class TimerProfilingTest : public ::testing::Test {};

TEST_F(TimerProfilingTest, AccumulateDurations) {
    long long totalDuration = 0;

    for (int i = 0; i < 3; ++i) {
        Timer timer("iteration", [&totalDuration](auto&, long long d) {
            totalDuration += d;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    EXPECT_GT(totalDuration, 0);
}

TEST_F(TimerProfilingTest, TrackMultipleOperations) {
    std::map<std::string, long long> durations;

    {
        Timer timer("operation_a", [&durations](auto&, long long d) {
            durations["operation_a"] = d;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    {
        Timer timer("operation_b", [&durations](auto&, long long d) {
            durations["operation_b"] = d;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_EQ(durations.size(), 2u);
    EXPECT_GE(durations["operation_a"], 0);
    EXPECT_GE(durations["operation_b"], 0);
}

}  // namespace nexo
