#ifndef QUUI_H
#define QUUI_H

#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <stdexcept>

namespace QuUI {

// Forward declarations
class UIContext;
class Theme;
class Event;
class ResourceManager;

// Main QuUI class - the primary interface for the SDK
class QuUISystem {
public:
    // Initialize the UI system with default settings
    static bool Initialize();
    
    // Initialize with custom settings
    static bool Initialize(const struct UIConfig& config);
    
    // Clean up and shutdown
    static void Shutdown();
    
    // Get the global instance
    static QuUISystem& Instance();
    
    // Create a new UI context (for multiple windows/viewports)
    std::shared_ptr<UIContext> CreateContext();
    
    // Set the active theme
    void SetTheme(std::shared_ptr<Theme> theme);
    
    // Get the resource manager
    ResourceManager& GetResourceManager();
    
    // Update and render
    void Update(float deltaTime);
    void Render();
    
    // Event handling
    void ProcessEvent(const Event& event);
    
    // Add global event handlers
    template<typename EventType>
    void AddEventHandler(std::function<void(const EventType&)> handler);

private:
    QuUISystem();
    
    QuUISystem(const QuUISystem&) = delete;
    QuUISystem& operator=(const QuUISystem&) = delete;
    
    struct Implementation;
    std::unique_ptr<Implementation> impl;
    
    static std::unique_ptr<QuUISystem> s_instance;

public:
    ~QuUISystem();
};

// Configuration structure for initialization
struct UIConfig {
    struct {
        int width = 1920;
        int height = 1080;
        bool vsync = true;
        int msaa = 4;
    } renderer;
    
    struct {
        std::string defaultFont = "Arial";
        float defaultFontSize = 14.0f;
        bool enableAnimations = true;
        float animationSpeed = 1.0f;
    } appearance;
    
    struct {
        bool enableGestures = true;
        float touchSensitivity = 1.0f;
        int maxTouchPoints = 10;
    } input;
    
    struct {
        bool enablePhysics = false;
        float physicsFPS = 60.0f;
    } physics;
};

// Helper macros for cleaner syntax
#define QUUI_INIT() QuUI::QuUISystem::Initialize()
#define QUUI_INIT_WITH_CONFIG(config) QuUI::QuUISystem::Initialize(config)
#define QUUI_SHUTDOWN() QuUI::QuUISystem::Shutdown()
#define QUUI_INSTANCE() QuUI::QuUISystem::Instance()

} // namespace QuUI

#endif // QUUI_H
