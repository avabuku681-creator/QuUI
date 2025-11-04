#pragma once
#include <queue>
#include <functional>
#include <unordered_map>
#include <string>
#include "../core/math_types.hpp"

namespace gui {

class EventDispatcher;
class Widget;

class Event {
public:
    enum class Type {
        MouseMove,
        MousePress,
        MouseRelease,
        KeyPress,
        KeyRelease,
        Focus
    };

    Event(Type type) : type_(type) {}
    virtual ~Event() = default;
    Type getType() const { return type_; }

private:
    Type type_;
};

// Базовые события
struct MouseEvent : public Event {
    Vector2f position;
    int button;
    bool pressed;
    
    MouseEvent(Type type, const Vector2f& pos, int btn = 0, bool press = false)
        : Event(type), position(pos), button(btn), pressed(press) {}
};

struct KeyEvent : public Event {
    int keyCode;
    bool pressed;
    bool alt;
    bool ctrl;
    bool shift;
    
    KeyEvent(Type type, int key, bool press, bool a = false, bool c = false, bool s = false)
        : Event(type), keyCode(key), pressed(press), alt(a), ctrl(c), shift(s) {}
};

struct FocusEvent : public Event {
    Widget* oldFocus;
    Widget* newFocus;
    
    FocusEvent(Widget* old, Widget* next)
        : Event(Type::Focus), oldFocus(old), newFocus(next) {}
};

// Система обработки событий
class EventDispatcher {
public:
    using EventCallback = std::function<void(const Event&)>;
    
    static EventDispatcher& getInstance() {
        static EventDispatcher instance;
        return instance;
    }

    void addEventListener(const std::string& eventType, EventCallback callback);
    void removeEventListener(const std::string& eventType);
    void dispatchEvent(const Event& event);
    void update();

private:
    EventDispatcher() = default;
    std::unordered_map<std::string, std::vector<EventCallback>> eventListeners_;
    std::queue<Event> eventQueue_;
};

// Вспомогательные классы для работы с событиями
class EventHandler {
public:
    virtual ~EventHandler() = default;
    virtual bool handleEvent(const Event& event) = 0;
};

class MouseHandler : public EventHandler {
public:
    bool handleEvent(const Event& event) override;
    
    void setOnMouseMove(std::function<void(const MouseEvent&)> callback);
    void setOnMousePress(std::function<void(const MouseEvent&)> callback);
    void setOnMouseRelease(std::function<void(const MouseEvent&)> callback);

private:
    std::function<void(const MouseEvent&)> onMouseMove_;
    std::function<void(const MouseEvent&)> onMousePress_;
    std::function<void(const MouseEvent&)> onMouseRelease_;
};

class KeyboardHandler : public EventHandler {
public:
    bool handleEvent(const Event& event) override;
    
    void setOnKeyPress(std::function<void(const KeyEvent&)> callback);
    void setOnKeyRelease(std::function<void(const KeyEvent&)> callback);

private:
    std::function<void(const KeyEvent&)> onKeyPress_;
    std::function<void(const KeyEvent&)> onKeyRelease_;
};

// Вспомогательные функции для работы с событиями
namespace EventUtils {
    bool isMouseEvent(const Event& event);
    bool isKeyEvent(const Event& event);
    bool isFocusEvent(const Event& event);
    
    MouseEvent* asMouseEvent(Event& event);
    KeyEvent* asKeyEvent(Event& event);
    FocusEvent* asFocusEvent(Event& event);
    
    Vector2f getMousePosition(const Event& event);
    bool isMousePressed(const Event& event);
    bool isKeyPressed(const Event& event);
    bool hasModifier(const Event& event, int modifier);
}

} // namespace gui