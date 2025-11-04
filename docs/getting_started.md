# Getting Started with QuUI

This guide will walk you through the basics of using QuUI SDK in your application.

## Prerequisites

- C++17 compatible compiler
- Basic understanding of C++
- Basic understanding of UI concepts

## Integration

QuUI is designed to be easy to integrate into your project. Since it has no external dependencies, you can simply include the headers in your project.

### Project Structure

```
your_project/
├── QuUI/
│   ├── QuUI.h
│   ├── QuUI.cpp
│   └── ...
├── CMakeLists.txt
└── your_source.cpp
```

### CMake Integration

```cmake
# Add QuUI directory
add_subdirectory(QuUI)

# Link with your target
target_link_libraries(your_target PRIVATE QuUI)
```

## Basic Concepts

### UI Context

A UI context represents a complete UI hierarchy. You can have multiple contexts for different windows or viewports.

```cpp
auto context = ui.CreateContext();
```

### Event System

QuUI uses an event-driven architecture. Events can be:
- Mouse events
- Keyboard events
- Touch events
- Gesture events
- Custom events

```cpp
// Add event handler
ui.AddEventHandler<MouseEvent>([](const MouseEvent& e) {
    // Handle mouse event
});
```

### Resource Management

Resources (images, fonts, etc.) are managed through the ResourceManager:

```cpp
auto& resources = ui.GetResourceManager();
resources.LoadFont("main", "path/to/font.ttf");
resources.LoadImage("logo", "path/to/logo.png");
```

### Theming

QuUI supports comprehensive theming:

```cpp
auto theme = std::make_shared<Theme>();
theme->SetColor("background", Color(0.2f, 0.2f, 0.2f));
theme->SetColor("text", Color(1.0f, 1.0f, 1.0f));
ui.SetTheme(theme);
```

## Configuration Options

### Renderer Configuration

```cpp
UIConfig config;
config.renderer.width = 1920;        // Window width
config.renderer.height = 1080;       // Window height
config.renderer.vsync = true;        // Enable VSync
config.renderer.msaa = 4;           // MSAA samples
```

### Appearance Configuration

```cpp
config.appearance.defaultFont = "Arial";
config.appearance.defaultFontSize = 14.0f;
config.appearance.enableAnimations = true;
config.appearance.animationSpeed = 1.0f;
```

### Input Configuration

```cpp
config.input.enableGestures = true;
config.input.touchSensitivity = 1.0f;
config.input.maxTouchPoints = 10;
```

### Physics Configuration

```cpp
config.physics.enablePhysics = true;
config.physics.physicsFPS = 60.0f;
```

## Best Practices

1. **Initialization**
   - Always check initialization return value
   - Initialize early in your application
   - Use custom configuration for specific needs

2. **Resource Management**
   - Load resources during initialization
   - Use resource manager for all assets
   - Clean up resources when shutting down

3. **Event Handling**
   - Keep event handlers lightweight
   - Avoid blocking operations in handlers
   - Use proper event types

4. **Performance**
   - Update only when needed
   - Use proper layout containers
   - Minimize dynamic allocations

## Common Pitfalls

1. **Initialization**
   - Not checking initialization return value
   - Trying to use SDK before initialization
   - Forgetting to call shutdown

2. **Memory Management**
   - Not cleaning up resources
   - Keeping references after shutdown
   - Manual memory management of UI elements

3. **Thread Safety**
   - Accessing UI from multiple threads
   - Not synchronizing updates
   - Blocking the main thread

## Next Steps

- Explore the [Widget Reference](widget_reference.md)
- Learn about [Layouts](layout_guide.md)
- Understand the [Theme System](theme_system.md)
- Check out [Advanced Features](advanced_features.md)