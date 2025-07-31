///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date: Today lol
//  Description: Test file for the Nexo functions
//
///////////////////////////////////////////////////////////////////////////////

// NOTE: Some tests in this file are limited due to Application design:
// - Cannot reset singleton instance for test isolation
// - Cannot mock non-virtual methods
// The Application class needs test support methods

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Nexo.hpp"
#include "Application.hpp"
#include "ecs/Coordinator.hpp"
#include "core/event/KeyCodes.hpp"

namespace nexo {

// Mock Application for testing
class MockApplication : public Application {
public:
    MockApplication() : Application() {}
    
    // We can't mock non-virtual methods, so just provide a way to test
    bool initCalled = false;
    bool runCalled = false;
    
    void init() {
        initCalled = true;
        Application::init();
    }
    
    void run(const SceneInfo& /*sceneInfo*/) {
        runCalled = true;
        // Don't actually run the app in tests
    }
};

// Custom test application
class TestCustomApp : public Application {
public:
    TestCustomApp(int value) : Application(), testValue(value) {}
    
    int getTestValue() const { return testValue; }
    
private:
    int testValue;
};

class NexoTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Note: We can't reset the singleton instance in tests
        // TODO: Application class needs a test reset method
    }
    
    void TearDown() override {
        // Note: We can't reset the singleton instance in tests
        // TODO: Application class needs a test reset method
    }
};

TEST_F(NexoTest, Init) {
    // Test that init creates and initializes the Application instance
    Application& app = nexo::init();
    
    // Verify we got a valid Application instance
    EXPECT_NE(&app, nullptr);
    
    // Verify it's the same as getInstance
    EXPECT_EQ(&app, &Application::getInstance());
}

TEST_F(NexoTest, GetApp) {
    // Initialize first
    nexo::init();
    
    // Test that getApp returns the same instance
    Application& app1 = nexo::getApp();
    Application& app2 = Application::getInstance();
    
    EXPECT_EQ(&app1, &app2);
}

TEST_F(NexoTest, UseApp_CustomApplication) {
    // Test using a custom application class
    nexo::useApp<TestCustomApp>(42);
    
    // Get the instance and verify it's our custom type
    auto& app = Application::getInstance();
    auto* customApp = dynamic_cast<TestCustomApp*>(&app);
    
    EXPECT_NE(customApp, nullptr);
    EXPECT_EQ(customApp->getTestValue(), 42);
}

TEST_F(NexoTest, UseApp_WithMultipleArgs) {
    // Define a test app that takes multiple arguments
    class MultiArgApp : public Application {
    public:
        MultiArgApp(int a, float b, const std::string& c) 
            : Application(), intVal(a), floatVal(b), stringVal(c) {}
        
        int intVal;
        float floatVal;
        std::string stringVal;
    };
    
    // Use the app with multiple arguments
    nexo::useApp<MultiArgApp>(10, 3.14f, "test");
    
    auto& app = Application::getInstance();
    auto* multiApp = dynamic_cast<MultiArgApp*>(&app);
    
    EXPECT_NE(multiApp, nullptr);
    EXPECT_EQ(multiApp->intVal, 10);
    EXPECT_FLOAT_EQ(multiApp->floatVal, 3.14f);
    EXPECT_EQ(multiApp->stringVal, "test");
}

TEST_F(NexoTest, RunEngine) {
    // Create a test scene info
    Application::SceneInfo sceneInfo{
        .id = 1,
        .renderingType = RenderingType::WINDOW,
        .sceneType = SceneType::GAME,
        .isChildWindow = false,
        .viewportBounds = {{0, 0}, {800, 600}}
    };
    
    // Initialize the application
    nexo::init();
    
    // Test that runEngine calls the application's run method
    // Note: This will call the actual run method which might have side effects
    // In a real test environment, you might want to use a mock here
    nexo::runEngine(sceneInfo);
}

TEST_F(NexoTest, TemplateHeaders_Available) {
    // Test that template functions from headers are available
    // This ensures all necessary headers are included in Nexo.hpp
    
    // KeyCodes
    // Test key codes are available
    int key = NEXO_KEY_A;
    EXPECT_EQ(key, NEXO_KEY_A);
    
    // Timestep
    Timestep ts(0.016f);
    EXPECT_FLOAT_EQ(ts.getSeconds(), 0.016f);
    
    // Timer (just verify we can create one)
    // Timer requires a lambda that takes the timer itself and duration
    Timer timer("TestTimer", [](auto&, long long){});
    
    // Logger macro
    LOG(NEXO_INFO, "Test log from Nexo test");
}

TEST_F(NexoTest, MultipleInitCalls) {
    // Test that multiple init calls return the same instance
    Application& app1 = nexo::init();
    Application& app2 = nexo::init();
    
    EXPECT_EQ(&app1, &app2);
}

TEST_F(NexoTest, InitThenGetApp) {
    // Test init followed by getApp
    Application& initApp = nexo::init();
    Application& getApp = nexo::getApp();
    
    EXPECT_EQ(&initApp, &getApp);
}

TEST_F(NexoTest, GetAppBeforeInit) {
    // Test that getApp works even before explicit init
    // (getInstance creates the instance if needed)
    Application& app = nexo::getApp();
    
    EXPECT_NE(&app, nullptr);
}

} // namespace nexo