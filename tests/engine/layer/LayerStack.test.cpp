#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/layer/LayerStack.hpp"
#include "core/layer/Layer.hpp"

namespace nexo::layer {
    // Mock Layer class for testing
    class MockLayer : public Layer {
    public:
        MockLayer(const std::string& name, unsigned int id)
            : Layer(id, name) {}
    };

    TEST(LayerStackTest, PushLayer) {
        LayerStack stack;
        auto layer = std::make_shared<MockLayer>("Layer1", 1);

        stack.pushLayer(layer);

        EXPECT_EQ(stack.size(), 1);
        EXPECT_EQ(stack[0], layer);
    }

    TEST(LayerStackTest, PushOverlay) {
        LayerStack stack;
        auto overlay = std::make_shared<MockLayer>("Overlay1", 2);

        stack.pushOverlay(overlay);

        EXPECT_EQ(stack.size(), 1);
        EXPECT_EQ(stack[0], overlay);
    }

    TEST(LayerStackTest, PopLayer) {
#ifdef _WIN32
        // TODO: fix test (see #100)
        GTEST_SKIP() << "Test crashes on the CI on Windows, skipping for now.";
#endif
        LayerStack stack;
        auto layer = std::make_shared<MockLayer>("Layer1", 1);
        stack.pushLayer(layer);

        EXPECT_EQ(stack.size(), 1);

        stack.popLayer(layer);
        EXPECT_EQ(stack.size(), 0);
    }

    TEST(LayerStackTest, PopOverlay) {
#ifdef _WIN32
        // TODO: fix test (see #100)
        GTEST_SKIP() << "Test crashes on the CI on Windows, skipping for now.";
#endif
        LayerStack stack;
        auto overlay = std::make_shared<MockLayer>("Overlay1", 2);
        stack.pushOverlay(overlay);

        EXPECT_EQ(stack.size(), 1);

        stack.popOverlay(overlay);
        EXPECT_EQ(stack.size(), 0);
    }

    TEST(LayerStackTest, AccessByIndex) {
        LayerStack stack;
        auto layer1 = std::make_shared<MockLayer>("Layer1", 1);
        auto layer2 = std::make_shared<MockLayer>("Layer2", 2);

        stack.pushLayer(layer1);
        stack.pushLayer(layer2);

        EXPECT_EQ(stack[0], layer2);
        EXPECT_EQ(stack[1], layer1);

        // Out of range access
        EXPECT_EQ(stack[999], nullptr);
    }

    TEST(LayerStackTest, AccessByName) {
        LayerStack stack;
        auto layer = std::make_shared<MockLayer>("Layer1", 1);
        stack.pushLayer(layer);

        EXPECT_EQ(stack["Layer1"], layer);
        EXPECT_EQ(stack["NonExistentLayer"], nullptr);
    }

    TEST(LayerStackTest, AccessById) {
        LayerStack stack;
        auto layer = std::make_shared<MockLayer>("Layer1", 1);
        stack.pushLayer(layer);

        EXPECT_EQ(stack.byId(1), layer);
        EXPECT_EQ(stack.byId(999), nullptr);
    }

    TEST(LayerStackTest, PushMultipleLayersAndOverlays) {
        LayerStack stack;
        auto layer1 = std::make_shared<MockLayer>("Layer1", 1);
        auto layer2 = std::make_shared<MockLayer>("Layer2", 2);
        auto layer3 = std::make_shared<MockLayer>("Layer3", 5);
        auto overlay1 = std::make_shared<MockLayer>("Overlay1", 3);
        auto overlay2 = std::make_shared<MockLayer>("Overlay2", 4);

        stack.pushLayer(layer1);
        stack.pushLayer(layer2);
        stack.pushOverlay(overlay1);
        stack.pushOverlay(overlay2);
        stack.pushLayer(layer3);

        // Validate the order, layers are placed on top of each other but below overlays
        EXPECT_EQ(stack.size(), 5);
        EXPECT_EQ(stack[0], overlay2);
        EXPECT_EQ(stack[1], overlay1);
        EXPECT_EQ(stack[2], layer3);
        EXPECT_EQ(stack[3], layer2);
        EXPECT_EQ(stack[4], layer1);
    }

    TEST(LayerStackTest, RemoveLayerAndOverlayOrder) {
#ifdef _WIN32
        // TODO: fix test (see #100)
        GTEST_SKIP() << "Test crashes on the CI on Windows, skipping for now.";
#endif
        LayerStack stack;
        auto layer = std::make_shared<MockLayer>("Layer", 1);
        auto overlay = std::make_shared<MockLayer>("Overlay", 2);

        stack.pushLayer(layer);
        stack.pushOverlay(overlay);

        EXPECT_EQ(stack.size(), 2);

        stack.popOverlay(overlay);
        EXPECT_EQ(stack.size(), 1);
        EXPECT_EQ(stack[0], layer);

        stack.popLayer(layer);
        EXPECT_EQ(stack.size(), 0);
    }

    TEST(LayerStackTest, IteratorAccess) {
        LayerStack stack;
        auto layer1 = std::make_shared<MockLayer>("Layer1", 1);
        auto layer2 = std::make_shared<MockLayer>("Layer2", 2);

        stack.pushLayer(layer1);
        stack.pushLayer(layer2);

        auto it = stack.begin();
        EXPECT_EQ(*it, layer2);
        ++it;
        EXPECT_EQ(*it, layer1);
        ++it;
        EXPECT_EQ(it, stack.end());
    }

    TEST(LayerStackTest, EmptyStackAccess) {
        LayerStack stack;

        EXPECT_EQ(stack.size(), 0);
        EXPECT_EQ(stack[0], nullptr);
        EXPECT_EQ(stack["NonExistentLayer"], nullptr);
        EXPECT_EQ(stack.byId(999), nullptr);
    }

    TEST(LayerStackTest, RemoveLayersAndOverlaysInRandomOrder) {
#ifdef _WIN32
        // TODO: fix test (see #100)
        GTEST_SKIP() << "Test crashes on the CI on Windows, skipping for now.";
#endif
        LayerStack stack;

        auto layer1 = std::make_shared<MockLayer>("Layer1", 1);
        auto layer2 = std::make_shared<MockLayer>("Layer2", 2);
        auto overlay1 = std::make_shared<MockLayer>("Overlay1", 3);
        auto overlay2 = std::make_shared<MockLayer>("Overlay2", 4);

        stack.pushLayer(layer1);
        stack.pushLayer(layer2);
        stack.pushOverlay(overlay1);
        stack.pushOverlay(overlay2);

        EXPECT_EQ(stack[0], overlay2);
        EXPECT_EQ(stack[1], overlay1);
        EXPECT_EQ(stack[2], layer2);
        EXPECT_EQ(stack[3], layer1);

        stack.popLayer(layer2);
        EXPECT_EQ(stack[0], overlay2);
        EXPECT_EQ(stack[1], overlay1);
        EXPECT_EQ(stack[2], layer1);

        stack.popOverlay(overlay1);
        EXPECT_EQ(stack[0], overlay2);
        EXPECT_EQ(stack[1], layer1);

        stack.pushOverlay(overlay1);
        EXPECT_EQ(stack[0], overlay1);
        EXPECT_EQ(stack[1], overlay2);
        EXPECT_EQ(stack[2], layer1);

        stack.popOverlay(overlay2);
        EXPECT_EQ(stack[0], overlay1);
        EXPECT_EQ(stack[1], layer1);

        stack.pushLayer(layer2);
        EXPECT_EQ(stack[0], overlay1);
        EXPECT_EQ(stack[1], layer2);
        EXPECT_EQ(stack[2], layer1);

        stack.popOverlay(overlay1);
        EXPECT_EQ(stack[0], layer2);
        EXPECT_EQ(stack[1], layer1);

        stack.popLayer(layer1);
        EXPECT_EQ(stack[0], layer2);
    }

}
