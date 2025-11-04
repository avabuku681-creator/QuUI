#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include "../core/math_types.hpp"

namespace gui {

// Коды клавиш
enum class KeyCode {
    Unknown = -1,
    A = 0, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Escape, LControl, LShift, LAlt, LSystem,
    RControl, RShift, RAlt, RSystem, Menu,
    LBracket, RBracket, Semicolon, Comma, Period,
    Quote, Slash, Backslash, Tilde, Equal, Hyphen,
    Space, Enter, Backspace, Tab, PageUp, PageDown,
    End, Home, Insert, Delete, Add, Subtract,
    Multiply, Divide, Left, Right, Up, Down,
    Numpad0, Numpad1, Numpad2, Numpad3, Numpad4,
    Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    F13, F14, F15, Pause
};

// Коды кнопок мыши
enum class MouseButton {
    Left,
    Right,
    Middle,
    XButton1,
    XButton2
};

// Состояние клавиши
struct KeyState {
    bool pressed = false;
    bool justPressed = false;
    bool justReleased = false;
    float pressTime = 0.0f;
};

// Состояние мыши
struct MouseState {
    Vector2f position;
    Vector2f previousPosition;
    Vector2f delta;
    Vector2f wheelDelta;
    std::unordered_map<MouseButton, KeyState> buttons;
};

// Привязка ввода
class InputBinding {
public:
    enum class Type {
        Keyboard,
        Mouse,
        MouseWheel,
        MouseMotion
    };

    InputBinding(const std::string& name);
    
    void addKeyBinding(KeyCode key);
    void addMouseBinding(MouseButton button);
    void addMouseWheelBinding();
    void addMouseMotionBinding();
    
    void setCallback(std::function<void(float)> callback);
    void update(float value);

private:
    std::string name_;
    Type type_;
    std::vector<KeyCode> keys_;
    std::vector<MouseButton> buttons_;
    std::function<void(float)> callback_;
};

// Действие ввода
class InputAction {
public:
    InputAction(const std::string& name);
    
    void addBinding(std::shared_ptr<InputBinding> binding);
    void removeBinding(const std::string& bindingName);
    
    void update(float deltaTime);
    float getValue() const;
    bool isActive() const;
    
    void setDeadZone(float deadZone);
    void setMultiplier(float multiplier);

private:
    std::string name_;
    std::vector<std::shared_ptr<InputBinding>> bindings_;
    float value_ = 0.0f;
    float deadZone_ = 0.1f;
    float multiplier_ = 1.0f;
};

// Система ввода
class InputSystem {
public:
    static InputSystem& getInstance() {
        static InputSystem instance;
        return instance;
    }

    void update(float deltaTime);
    
    // Состояние клавиатуры
    bool isKeyPressed(KeyCode key) const;
    bool isKeyJustPressed(KeyCode key) const;
    bool isKeyJustReleased(KeyCode key) const;
    float getKeyPressTime(KeyCode key) const;
    
    // Состояние мыши
    const MouseState& getMouseState() const;
    bool isMouseButtonPressed(MouseButton button) const;
    bool isMouseButtonJustPressed(MouseButton button) const;
    bool isMouseButtonJustReleased(MouseButton button) const;
    
    // Управление действиями
    void addAction(std::shared_ptr<InputAction> action);
    void removeAction(const std::string& actionName);
    InputAction* getAction(const std::string& actionName);
    
    // События
    using KeyCallback = std::function<void(KeyCode, bool)>;
    using MouseButtonCallback = std::function<void(MouseButton, bool)>;
    using MouseMoveCallback = std::function<void(const Vector2f&)>;
    using MouseWheelCallback = std::function<void(const Vector2f&)>;
    
    void setKeyCallback(KeyCallback callback);
    void setMouseButtonCallback(MouseButtonCallback callback);
    void setMouseMoveCallback(MouseMoveCallback callback);
    void setMouseWheelCallback(MouseWheelCallback callback);

private:
    InputSystem() = default;
    
    std::unordered_map<KeyCode, KeyState> keyStates_;
    MouseState mouseState_;
    std::unordered_map<std::string, std::shared_ptr<InputAction>> actions_;
    
    KeyCallback keyCallback_;
    MouseButtonCallback mouseButtonCallback_;
    MouseMoveCallback mouseMoveCallback_;
    MouseWheelCallback mouseWheelCallback_;
};

// Горячие клавиши
class Shortcut {
public:
    Shortcut(const std::vector<KeyCode>& keys = {});
    
    void setKeys(const std::vector<KeyCode>& keys);
    void setCallback(std::function<void()> callback);
    
    bool matches(const std::vector<KeyCode>& pressedKeys) const;
    void execute() const;

private:
    std::vector<KeyCode> keys_;
    std::function<void()> callback_;
};

// Менеджер горячих клавиш
class ShortcutManager {
public:
    static ShortcutManager& getInstance() {
        static ShortcutManager instance;
        return instance;
    }

    void addShortcut(const std::string& name, std::shared_ptr<Shortcut> shortcut);
    void removeShortcut(const std::string& name);
    void update();

private:
    ShortcutManager() = default;
    std::unordered_map<std::string, std::shared_ptr<Shortcut>> shortcuts_;
};

} // namespace gui