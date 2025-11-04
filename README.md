# QuUI SDK

QuUI is a lightweight, modern C++ UI framework designed for creating beautiful and responsive user interfaces without external dependencies. It provides a comprehensive set of tools for building sophisticated applications while maintaining high performance and minimal overhead.

## Features

- 🚀 Zero external dependencies
- 💡 Modern C++ design (C++17)
- 🎨 Flexible theming system
- 📱 Touch and gesture support
- 🖼️ Hardware-accelerated rendering
- 🎭 Advanced animation system
- 📐 Flexible layout system
- 🔄 Event-driven architecture
- 🎮 Optional physics integration
- 🎯 Multi-window/viewport support

## Quick Start

### Installation

QuUI is header-only and requires no external dependencies. Simply include the SDK in your project:

```cpp
#include "QuUI.h"
```

### Basic Usage

1. Initialize the SDK:
```cpp
// Initialize with default settings
if (!QUUI_INIT()) {
    // Handle initialization error
    return;
}

// Or initialize with custom configuration
QuUI::UIConfig config;
config.renderer.width = 1280;
config.renderer.height = 720;
config.appearance.enableAnimations = true;

if (!QUUI_INIT_WITH_CONFIG(config)) {
    // Handle initialization error
    return;
}
```

2. Create a UI context:
```cpp
auto& ui = QUUI_INSTANCE();
auto context = ui.CreateContext();
```

3. Main loop:
```cpp
while (running) {
    float deltaTime = calculateDeltaTime(); // Your time calculation
    
    // Update UI
    ui.Update(deltaTime);
    
    // Render UI
    ui.Render();
}
```

4. Cleanup:
```cpp
QUUI_SHUTDOWN();
```

## Example Application

```cpp
#include "QuUI.h"

int main() {
    // Initialize SDK with custom configuration
    QuUI::UIConfig config;
    config.renderer.width = 1280;
    config.renderer.height = 720;
    config.appearance.enableAnimations = true;
    config.input.enableGestures = true;
    
    if (!QUUI_INIT_WITH_CONFIG(config)) {
        return -1;
    }
    
    // Get SDK instance
    auto& ui = QUUI_INSTANCE();
    
    // Create UI context
    auto context = ui.CreateContext();
    
    // Main loop
    bool running = true;
    while (running) {
        // Your frame time calculation
        float deltaTime = 1.0f / 60.0f;
        
        // Process events
        Event event;
        while (pollEvents(&event)) { // Your event polling
            ui.ProcessEvent(event);
        }
        
        // Update and render
        ui.Update(deltaTime);
        ui.Render();
    }
    
    // Cleanup
    QUUI_SHUTDOWN();
    return 0;
}
```

## Documentation

For detailed documentation, please refer to the following sections:

- [Getting Started](docs/getting_started.md)
- [Core Concepts](docs/core_concepts.md)
- [Widget Reference](docs/widget_reference.md)
- [Theme System](docs/theme_system.md)
- [Layout Guide](docs/layout_guide.md)
- [Event System](docs/event_system.md)
- [Animation Guide](docs/animation_guide.md)
- [Best Practices](docs/best_practices.md)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.