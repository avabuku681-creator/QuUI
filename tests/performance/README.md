# Performance Tests

This suite of performance tests helps measure and optimize various aspects of the QuUI SDK.

## Layout Performance Tests

```cpp
#include "QuUI.h"
#include "performance_test.hpp"

class LayoutPerformanceTest : public PerformanceTest {
public:
    void Setup() override {
        // Create test container
        container = std::make_shared<Container>();
        
        // Create different layouts
        flowLayout = std::make_shared<FlowLayout>();
        gridLayout = std::make_shared<GridLayout>(10, 10);
        flexLayout = std::make_shared<FlexLayout>();
    }

    void RunTests() override {
        TestFlowLayout();
        TestGridLayout();
        TestFlexLayout();
        TestNestedLayouts();
        TestDynamicUpdates();
    }

private:
    void TestFlowLayout() {
        StartTest("Flow Layout - 1000 widgets");
        
        container->SetLayout(flowLayout);
        
        // Add 1000 widgets
        for (int i = 0; i < 1000; i++) {
            auto widget = std::make_shared<Widget>();
            widget->SetSize(10, 10);
            container->Add(widget);
        }
        
        // Measure layout time
        auto start = std::chrono::high_resolution_clock::now();
        container->Layout();
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportMemoryUsage();
    }

    void TestGridLayout() {
        StartTest("Grid Layout - 100x100 grid");
        
        container->SetLayout(gridLayout);
        
        // Add 10000 widgets in a grid
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                auto widget = std::make_shared<Widget>();
                widget->SetSize(5, 5);
                container->Add(widget);
            }
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        container->Layout();
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportMemoryUsage();
    }

    void TestFlexLayout() {
        StartTest("Flex Layout - Complex arrangement");
        
        container->SetLayout(flexLayout);
        
        // Create complex flex arrangement
        for (int i = 0; i < 100; i++) {
            auto row = std::make_shared<Container>();
            auto rowLayout = std::make_shared<FlexLayout>();
            rowLayout->SetDirection(FlexLayout::Row);
            row->SetLayout(rowLayout);
            
            for (int j = 0; j < 10; j++) {
                auto widget = std::make_shared<Widget>();
                widget->SetFlexGrow(1);
                widget->SetSize(10, 10);
                row->Add(widget);
            }
            
            container->Add(row);
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        container->Layout();
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportMemoryUsage();
    }

    void TestNestedLayouts() {
        StartTest("Nested Layouts - 5 levels deep");
        
        // Create deeply nested layout structure
        auto createNestedContainer = [](int depth) -> std::shared_ptr<Container> {
            if (depth == 0) {
                auto widget = std::make_shared<Widget>();
                widget->SetSize(5, 5);
                return widget;
            }
            
            auto container = std::make_shared<Container>();
            auto layout = std::make_shared<FlowLayout>();
            container->SetLayout(layout);
            
            for (int i = 0; i < 5; i++) {
                container->Add(createNestedContainer(depth - 1));
            }
            
            return container;
        };
        
        container->Add(createNestedContainer(5));
        
        auto start = std::chrono::high_resolution_clock::now();
        container->Layout();
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportMemoryUsage();
    }

    void TestDynamicUpdates() {
        StartTest("Dynamic Updates - 1000 updates");
        
        container->SetLayout(flowLayout);
        
        std::vector<std::shared_ptr<Widget>> widgets;
        for (int i = 0; i < 100; i++) {
            auto widget = std::make_shared<Widget>();
            widget->SetSize(10, 10);
            container->Add(widget);
            widgets.push_back(widget);
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Perform 1000 random updates
        for (int i = 0; i < 1000; i++) {
            int index = rand() % widgets.size();
            widgets[index]->SetSize(
                5 + rand() % 10,
                5 + rand() % 10
            );
            container->Layout();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportMemoryUsage();
    }

    std::shared_ptr<Container> container;
    std::shared_ptr<FlowLayout> flowLayout;
    std::shared_ptr<GridLayout> gridLayout;
    std::shared_ptr<FlexLayout> flexLayout;
};

// Run the tests
int main() {
    LayoutPerformanceTest test;
    test.Run();
    return 0;
}
```

## Rendering Performance Tests

```cpp
class RenderingPerformanceTest : public PerformanceTest {
public:
    void Setup() override {
        renderer = std::make_shared<Renderer>(800, 600);
    }

    void RunTests() override {
        TestBasicShapes();
        TestTextures();
        TestAlphaBlending();
        TestBatchRendering();
        TestComplexScene();
    }

private:
    void TestBasicShapes() {
        StartTest("Basic Shapes - 10000 rectangles");
        
        std::vector<Rect> rectangles;
        for (int i = 0; i < 10000; i++) {
            rectangles.push_back(Rect(
                rand() % 800,
                rand() % 600,
                10,
                10
            ));
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        renderer->BeginFrame();
        for (const auto& rect : rectangles) {
            renderer->DrawRect(rect, Color(1, 1, 1));
        }
        renderer->EndFrame();
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportGPUStats();
    }

    void TestTextures() {
        StartTest("Textures - 1000 textured quads");
        
        // Load test texture
        auto texture = resourceManager.LoadTexture("test.png");
        
        auto start = std::chrono::high_resolution_clock::now();
        
        renderer->BeginFrame();
        for (int i = 0; i < 1000; i++) {
            renderer->DrawTexture(
                texture,
                Rect(rand() % 800, rand() % 600, 32, 32)
            );
        }
        renderer->EndFrame();
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportGPUStats();
    }

    void TestAlphaBlending() {
        StartTest("Alpha Blending - 1000 transparent shapes");
        
        auto start = std::chrono::high_resolution_clock::now();
        
        renderer->BeginFrame();
        for (int i = 0; i < 1000; i++) {
            renderer->DrawRect(
                Rect(rand() % 800, rand() % 600, 50, 50),
                Color(1, 1, 1, 0.5f)
            );
        }
        renderer->EndFrame();
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportGPUStats();
    }

    void TestBatchRendering() {
        StartTest("Batch Rendering - 10000 shapes");
        
        std::vector<RenderCommand> commands;
        for (int i = 0; i < 10000; i++) {
            commands.push_back(RenderCommand::Rectangle(
                Rect(rand() % 800, rand() % 600, 10, 10),
                Color(1, 1, 1)
            ));
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        renderer->BeginBatch();
        for (const auto& cmd : commands) {
            renderer->AddToBatch(cmd);
        }
        renderer->EndBatch();
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportGPUStats();
    }

    void TestComplexScene() {
        StartTest("Complex Scene - Mixed rendering");
        
        auto start = std::chrono::high_resolution_clock::now();
        
        renderer->BeginFrame();
        
        // Background
        renderer->DrawRect(Rect(0, 0, 800, 600), Color(0.1f, 0.1f, 0.1f));
        
        // Shapes
        for (int i = 0; i < 1000; i++) {
            renderer->DrawRect(
                Rect(rand() % 800, rand() % 600, 20, 20),
                Color(
                    rand() / float(RAND_MAX),
                    rand() / float(RAND_MAX),
                    rand() / float(RAND_MAX),
                    0.5f
                )
            );
        }
        
        // Text
        for (int i = 0; i < 100; i++) {
            renderer->DrawText(
                "Test",
                Point(rand() % 800, rand() % 600),
                Font("Arial", 12)
            );
        }
        
        renderer->EndFrame();
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportGPUStats();
    }

    std::shared_ptr<Renderer> renderer;
    ResourceManager resourceManager;
};
```

## Event System Performance Tests

```cpp
class EventSystemPerformanceTest : public PerformanceTest {
public:
    void Setup() override {
        eventSystem = std::make_shared<EventSystem>();
    }

    void RunTests() override {
        TestEventDispatch();
        TestEventFiltering();
        TestMultipleHandlers();
        TestEventPropagation();
    }

private:
    void TestEventDispatch() {
        StartTest("Event Dispatch - 100000 events");
        
        std::vector<Event> events;
        for (int i = 0; i < 100000; i++) {
            events.push_back(MouseEvent(
                MouseEvent::Move,
                Point(rand() % 800, rand() % 600)
            ));
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (const auto& event : events) {
            eventSystem->DispatchEvent(event);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportMemoryUsage();
    }

    void TestEventFiltering() {
        StartTest("Event Filtering - 100000 events with filters");
        
        // Add event filters
        eventSystem->AddFilter([](const Event& e) {
            return e.GetType() == Event::Type::Mouse;
        });
        
        eventSystem->AddFilter([](const Event& e) {
            if (auto mouseEvent = dynamic_cast<const MouseEvent*>(&e)) {
                return mouseEvent->type == MouseEvent::Move;
            }
            return false;
        });
        
        std::vector<Event> events;
        for (int i = 0; i < 100000; i++) {
            if (rand() % 2) {
                events.push_back(MouseEvent(
                    MouseEvent::Move,
                    Point(rand() % 800, rand() % 600)
                ));
            } else {
                events.push_back(KeyEvent(
                    KeyEvent::Press,
                    KeyCode::A
                ));
            }
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (const auto& event : events) {
            eventSystem->DispatchEvent(event);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportMemoryUsage();
    }

    void TestMultipleHandlers() {
        StartTest("Multiple Handlers - 1000 handlers, 1000 events");
        
        // Add handlers
        for (int i = 0; i < 1000; i++) {
            eventSystem->AddHandler<MouseEvent>([](const MouseEvent&) {
                // Handler logic
            });
        }
        
        std::vector<Event> events;
        for (int i = 0; i < 1000; i++) {
            events.push_back(MouseEvent(
                MouseEvent::Move,
                Point(rand() % 800, rand() % 600)
            ));
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (const auto& event : events) {
            eventSystem->DispatchEvent(event);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportMemoryUsage();
    }

    void TestEventPropagation() {
        StartTest("Event Propagation - 5 levels deep, 1000 events");
        
        // Create widget hierarchy
        auto root = std::make_shared<Widget>();
        auto current = root;
        for (int i = 0; i < 5; i++) {
            auto child = std::make_shared<Widget>();
            current->AddChild(child);
            current = child;
        }
        
        std::vector<Event> events;
        for (int i = 0; i < 1000; i++) {
            events.push_back(MouseEvent(
                MouseEvent::Move,
                Point(rand() % 800, rand() % 600)
            ));
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (const auto& event : events) {
            root->ProcessEvent(event);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        ReportTime(start, end);
        ReportMemoryUsage();
    }

    std::shared_ptr<EventSystem> eventSystem;
};
```

## Running the Tests

```cpp
int main() {
    // Run layout tests
    {
        LayoutPerformanceTest layoutTest;
        layoutTest.Run();
    }
    
    // Run rendering tests
    {
        RenderingPerformanceTest renderTest;
        renderTest.Run();
    }
    
    // Run event system tests
    {
        EventSystemPerformanceTest eventTest;
        eventTest.Run();
    }
    
    return 0;
}
```

## Performance Metrics

The tests measure:
1. Execution time
2. Memory usage
3. GPU statistics
4. Event processing time
5. Layout calculation time

## Expected Results

- Layout tests should complete within:
  - Flow layout: < 50ms for 1000 widgets
  - Grid layout: < 100ms for 10000 widgets
  - Flex layout: < 150ms for complex arrangements
  - Nested layouts: < 200ms for 5 levels
  - Dynamic updates: < 500ms for 1000 updates

- Rendering tests should maintain:
  - 60 FPS for basic shapes
  - 60 FPS for textured quads
  - 30 FPS for alpha blending
  - 60 FPS for batched rendering
  - 30 FPS for complex scenes

- Event system tests should process:
  - 100000 events in < 100ms
  - Filtered events in < 150ms
  - Multiple handlers in < 200ms
  - Event propagation in < 300ms