# Debugging Guide

This guide covers debugging techniques and tools available in QuUI SDK.

## Debug Features

### Debug Overlay

```cpp
// Enable debug overlay
DebugOverlay::Enable();

// Show different debug information
DebugOverlay::ShowMetrics(true);      // Performance metrics
DebugOverlay::ShowLayoutBounds(true);  // Layout boundaries
DebugOverlay::ShowEvents(true);        // Event flow
DebugOverlay::ShowMemory(true);        // Memory usage
```

### Performance Metrics

```cpp
// Enable performance monitoring
PerformanceMonitor::Enable();

// Get performance metrics
auto metrics = PerformanceMonitor::GetMetrics();
std::cout << "FPS: " << metrics.fps << std::endl;
std::cout << "Frame Time: " << metrics.frameTime << " ms" << std::endl;
std::cout << "Update Time: " << metrics.updateTime << " ms" << std::endl;
std::cout << "Render Time: " << metrics.renderTime << " ms" << std::endl;
```

### Memory Tracking

```cpp
// Enable memory tracking
MemoryTracker::Enable();

// Get memory statistics
auto stats = MemoryTracker::GetStats();
std::cout << "Total Allocated: " << stats.totalAllocated << " bytes" << std::endl;
std::cout << "Total Freed: " << stats.totalFreed << " bytes" << std::endl;
std::cout << "Current Usage: " << stats.currentUsage << " bytes" << std::endl;
std::cout << "Peak Usage: " << stats.peakUsage << " bytes" << std::endl;
```

### Event Logging

```cpp
// Enable event logging
EventLogger::Enable();

// Set log level
EventLogger::SetLevel(LogLevel::Debug);

// Filter specific events
EventLogger::AddFilter([](const Event& e) {
    return e.GetType() == Event::Type::Mouse;
});

// Log callback
EventLogger::SetCallback([](const std::string& message) {
    std::cout << "Event: " << message << std::endl;
});
```

## Debugging Techniques

### Widget Inspection

```cpp
// Enable widget inspector
WidgetInspector::Enable();

// Inspect specific widget
WidgetInspector::Inspect(widget);

// Get widget hierarchy
auto hierarchy = WidgetInspector::GetHierarchy(widget);
for (const auto& entry : hierarchy) {
    std::cout << entry.level << ": " << entry.type << std::endl;
}
```

### Layout Debugging

```cpp
// Enable layout debugging
LayoutDebugger::Enable();

// Track layout passes
LayoutDebugger::TrackLayoutPass([](const LayoutInfo& info) {
    std::cout << "Layout pass for: " << info.widgetName << std::endl;
    std::cout << "Duration: " << info.duration << " ms" << std::endl;
});

// Highlight layout issues
LayoutDebugger::HighlightIssues(true);
```

### Animation Debugging

```cpp
// Enable animation debugging
AnimationDebugger::Enable();

// Track animation performance
AnimationDebugger::TrackPerformance([](const AnimationMetrics& metrics) {
    std::cout << "Active animations: " << metrics.activeCount << std::endl;
    std::cout << "CPU usage: " << metrics.cpuUsage << "%" << std::endl;
});

// Slow down animations
AnimationDebugger::SetTimeScale(0.5f); // Half speed
```

### Resource Tracking

```cpp
// Enable resource tracking
ResourceTracker::Enable();

// Track resource loading
ResourceTracker::TrackLoading([](const ResourceInfo& info) {
    std::cout << "Loading: " << info.path << std::endl;
    std::cout << "Size: " << info.size << " bytes" << std::endl;
});

// Get resource statistics
auto stats = ResourceTracker::GetStats();
std::cout << "Total resources: " << stats.totalCount << std::endl;
std::cout << "Memory usage: " << stats.memoryUsage << " bytes" << std::endl;
```

## Common Issues and Solutions

### Layout Issues

1. **Widget Not Visible**
```cpp
// Check widget bounds
auto bounds = widget->GetBounds();
std::cout << "Position: " << bounds.x << ", " << bounds.y << std::endl;
std::cout << "Size: " << bounds.width << ", " << bounds.height << std::endl;

// Check visibility chain
auto parent = widget->GetParent();
while (parent) {
    if (!parent->IsVisible()) {
        std::cout << "Hidden parent found!" << std::endl;
    }
    parent = parent->GetParent();
}
```

2. **Layout Not Updating**
```cpp
// Force layout update
widget->InvalidateLayout();
widget->GetParent()->InvalidateLayout();

// Check layout constraints
auto constraints = widget->GetLayoutConstraints();
std::cout << "Min size: " << constraints.minWidth << ", " 
          << constraints.minHeight << std::endl;
```

### Event Issues

1. **Events Not Reaching Widget**
```cpp
// Check event propagation
widget->SetEventDebugEnabled(true);

// Track event chain
EventTracker::Enable();
EventTracker::SetCallback([](const EventChain& chain) {
    for (const auto& step : chain) {
        std::cout << step.widget << ": " << step.eventType << std::endl;
    }
});
```

2. **Input Not Working**
```cpp
// Check input state
auto inputSystem = ui.GetInputSystem();
std::cout << "Mouse pos: " << inputSystem->GetMousePosition().x 
          << ", " << inputSystem->GetMousePosition().y << std::endl;
std::cout << "Focus widget: " << inputSystem->GetFocusedWidget() << std::endl;
```

### Performance Issues

1. **Low FPS**
```cpp
// Enable performance profiling
Profiler::Enable();

// Profile specific section
Profiler::Begin("CriticalSection");
// ... code ...
Profiler::End();

// Get profile results
auto results = Profiler::GetResults();
for (const auto& entry : results) {
    std::cout << entry.name << ": " << entry.duration << "ms" << std::endl;
}
```

2. **Memory Leaks**
```cpp
// Track allocations
MemoryTracker::TrackAllocations(true);

// Get allocation report
auto report = MemoryTracker::GenerateReport();
std::cout << report.toString() << std::endl;

// Find leaked widgets
auto leaks = WidgetTracker::FindLeaks();
for (const auto& leak : leaks) {
    std::cout << "Leaked widget: " << leak.type << std::endl;
    std::cout << "Location: " << leak.file << ":" << leak.line << std::endl;
}
```

## Debug Build Configuration

```cmake
# CMakeLists.txt
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DQUUI_DEBUG)
    add_definitions(-DQUUI_ENABLE_PROFILING)
    add_definitions(-DQUUI_ENABLE_LOGGING)
    add_definitions(-DQUUI_ENABLE_MEMORY_TRACKING)
endif()
```

## Logging System

```cpp
// Configure logging
Logger::SetLevel(LogLevel::Debug);
Logger::SetOutput("debug.log");

// Add custom logger
Logger::AddHandler([](LogLevel level, const std::string& message) {
    // Custom logging logic
});

// Log messages
QUUI_LOG_DEBUG("Debug message");
QUUI_LOG_INFO("Info message");
QUUI_LOG_WARNING("Warning message");
QUUI_LOG_ERROR("Error message");
```

## Best Practices

1. **Development Setup**
   - Enable debug builds
   - Use debug overlays
   - Enable logging
   - Set up memory tracking

2. **Issue Investigation**
   - Check logs first
   - Use appropriate debug tools
   - Profile performance
   - Track resource usage

3. **Testing**
   - Test edge cases
   - Verify event handling
   - Check memory usage
   - Profile performance

4. **Maintenance**
   - Regular profiling
   - Memory leak checks
   - Performance monitoring
   - Log analysis