# API Reference

## Core Classes

### QuUISystem

Main entry point for the SDK.

```cpp
class QuUISystem {
public:
    // Initialization
    static bool Initialize();
    static bool Initialize(const UIConfig& config);
    static void Shutdown();
    static QuUISystem& Instance();
    
    // Context Management
    std::shared_ptr<UIContext> CreateContext();
    
    // Theme Management
    void SetTheme(std::shared_ptr<Theme> theme);
    
    // Resource Management
    ResourceManager& GetResourceManager();
    
    // Update and Render
    void Update(float deltaTime);
    void Render();
    
    // Event Handling
    void ProcessEvent(const Event& event);
    template<typename EventType>
    void AddEventHandler(std::function<void(const EventType&)> handler);
};
```

### Widget

Base class for all UI elements.

```cpp
class Widget : public std::enable_shared_from_this<Widget> {
public:
    // Hierarchy
    void AddChild(std::shared_ptr<Widget> child);
    void RemoveChild(std::shared_ptr<Widget> child);
    Widget* GetParent() const;
    
    // Geometry
    void SetPosition(const Point& position);
    void SetSize(const Size& size);
    void SetBounds(const Rect& bounds);
    
    // Appearance
    void SetVisible(bool visible);
    void SetOpacity(float opacity);
    void SetClipEnabled(bool enabled);
    
    // Input
    void SetEnabled(bool enabled);
    void SetFocusable(bool focusable);
    
    // Event Handling
    virtual void ProcessEvent(const Event& event);
    
    // Layout
    virtual Size GetPreferredSize() const;
    virtual void Layout();
    
    // Rendering
    virtual void Render(Renderer* renderer);
    
    // Animation
    AnimationBuilder Animate();
};
```

### Container

Widget container with layout capabilities.

```cpp
class Container : public Widget {
public:
    // Layout
    void SetLayout(std::shared_ptr<Layout> layout);
    Layout* GetLayout() const;
    
    // Children
    void Add(std::shared_ptr<Widget> widget);
    void Remove(std::shared_ptr<Widget> widget);
    void Clear();
    
    // Updates
    void BeginUpdate();
    void EndUpdate();
};
```

### Theme

Theme management class.

```cpp
class Theme {
public:
    // Colors
    void SetColor(const std::string& name, const Color& color);
    Color GetColor(const std::string& name) const;
    
    // Fonts
    void SetFont(const std::string& name, const Font& font);
    Font GetFont(const std::string& name) const;
    
    // Sizes
    void SetSize(const std::string& name, float size);
    float GetSize(const std::string& name) const;
    
    // Styles
    void SetStyle(const std::string& selector, const Style& style);
    Style GetStyle(const std::string& selector) const;
};
```

### Event

Base class for all events.

```cpp
class Event {
public:
    enum class Type {
        None,
        Mouse,
        Keyboard,
        Touch,
        Gesture,
        Custom
    };
    
    virtual Type GetType() const = 0;
    virtual bool IsHandled() const;
    virtual void SetHandled(bool handled);
};
```

### Layout

Base class for layout managers.

```cpp
class Layout {
public:
    virtual void Layout(Container* container) = 0;
    virtual Size GetPreferredSize(Container* container) const = 0;
    
    void SetSpacing(float spacing);
    void SetPadding(float padding);
    void SetAlignment(Alignment alignment);
};
```

### ResourceManager

Resource management class.

```cpp
class ResourceManager {
public:
    // Textures
    TextureHandle LoadTexture(const std::string& path);
    void UnloadTexture(TextureHandle handle);
    
    // Fonts
    FontHandle LoadFont(const std::string& path, float size);
    void UnloadFont(FontHandle handle);
    
    // Shaders
    ShaderHandle LoadShader(const std::string& vertPath, const std::string& fragPath);
    void UnloadShader(ShaderHandle handle);
    
    // Async Loading
    void LoadAsync(const std::string& path, std::function<void(Resource*)> callback);
};
```

### Renderer

Rendering system interface.

```cpp
class Renderer {
public:
    // State
    void BeginFrame();
    void EndFrame();
    void SetViewport(const Rect& viewport);
    
    // Drawing
    void DrawRect(const Rect& rect, const Color& color);
    void DrawText(const std::string& text, const Point& position, const Font& font);
    void DrawTexture(TextureHandle texture, const Rect& destRect);
    
    // Batching
    void BeginBatch();
    void EndBatch();
    
    // Clipping
    void PushClip(const Rect& rect);
    void PopClip();
};
```

### AnimationSystem

Animation management class.

```cpp
class AnimationSystem {
public:
    // Creation
    AnimationHandle CreateAnimation();
    void DestroyAnimation(AnimationHandle handle);
    
    // Control
    void Play(AnimationHandle handle);
    void Pause(AnimationHandle handle);
    void Stop(AnimationHandle handle);
    void Seek(AnimationHandle handle, float time);
    
    // Properties
    void SetDuration(AnimationHandle handle, float duration);
    void SetEasing(AnimationHandle handle, EasingFunction easing);
    void SetLooping(AnimationHandle handle, bool looping);
};
```

### InputSystem

Input handling system.

```cpp
class InputSystem {
public:
    // State
    bool IsKeyPressed(KeyCode key) const;
    bool IsMouseButtonPressed(MouseButton button) const;
    Point GetMousePosition() const;
    
    // Touch
    const TouchState& GetTouchState() const;
    
    // Gestures
    void EnableGestures(bool enable);
    bool IsGestureDetected(GestureType type) const;
};
```

## Utility Classes

### Point
```cpp
struct Point {
    float x;
    float y;
};
```

### Size
```cpp
struct Size {
    float width;
    float height;
};
```

### Rect
```cpp
struct Rect {
    float x;
    float y;
    float width;
    float height;
};
```

### Color
```cpp
struct Color {
    float r;
    float g;
    float b;
    float a;
};
```

### Font
```cpp
struct Font {
    std::string family;
    float size;
    FontWeight weight;
    FontStyle style;
};
```

### Style
```cpp
struct Style {
    std::map<std::string, Value> properties;
};
```

## Enums

### Alignment
```cpp
enum class Alignment {
    Left,
    Center,
    Right,
    Top,
    Middle,
    Bottom
};
```

### MouseButton
```cpp
enum class MouseButton {
    Left,
    Middle,
    Right
};
```

### KeyCode
```cpp
enum class KeyCode {
    None,
    A, B, C, // ... other keys
};
```

### GestureType
```cpp
enum class GestureType {
    Tap,
    DoubleTap,
    LongPress,
    Swipe,
    Pinch,
    Rotate
};
```

## Constants

### System Constants
```cpp
namespace Constants {
    const float DefaultFontSize = 14.0f;
    const float DefaultPadding = 8.0f;
    const float DefaultMargin = 8.0f;
    const float DefaultAnimationDuration = 0.3f;
    const int DefaultFrameRate = 60;
}
```

### Theme Constants
```cpp
namespace ThemeConstants {
    const char* DefaultFontFamily = "Arial";
    const char* DefaultColorScheme = "Light";
    const float DefaultBorderWidth = 1.0f;
    const float DefaultBorderRadius = 4.0f;
}
```