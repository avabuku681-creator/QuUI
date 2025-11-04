#pragma once
#include <memory>
#include <string>
#include <functional>
#include "math_types.hpp"
#include "event_types.hpp"

namespace gui {

class Event;
class Widget;
class Theme;

using EventCallback = std::function<void(const Event&)>;
using RenderCallback = std::function<void(const Widget&)>;

// Базовый класс для всех событий
class Event {
public:
    enum class Type {
        None,
        MouseMove,
        MousePress,
        MouseRelease,
        MouseEnter,
        MouseLeave,
        KeyPress,
        KeyRelease,
        Focus,
        Blur,
        Resize,
        Custom
    };

    explicit Event(Type type) : type_(type) {}
    virtual ~Event() = default;

    Type getType() const { return type_; }

private:
    Type type_;
};

// Базовый класс для всех виджетов
class Widget {
public:
    Widget();
    virtual ~Widget() = default;

    // Основные функции жизненного цикла
    virtual void update(float deltaTime);
    virtual void render() const;
    virtual void handleEvent(const Event& event);

    // Геометрия и позиционирование
    void setPosition(const Vector2f& pos);
    void setSize(const Vector2f& size);
    void setRotation(float rotation);
    void setScale(const Vector2f& scale);

    Vector2f getPosition() const;
    Vector2f getSize() const;
    float getRotation() const;
    Vector2f getScale() const;

    // Состояние и видимость
    void setVisible(bool visible);
    void setEnabled(bool enabled);
    void setFocused(bool focused);

    bool isVisible() const;
    bool isEnabled() const;
    bool isFocused() const;

    // Обработчики событий
    void setOnMouseEnter(EventCallback callback);
    void setOnMouseLeave(EventCallback callback);
    void setOnMousePress(EventCallback callback);
    void setOnMouseRelease(EventCallback callback);
    void setOnKeyPress(EventCallback callback);
    void setOnKeyRelease(EventCallback callback);

    // Стили и темы
    void setTheme(std::shared_ptr<Theme> theme);
    void setCustomStyle(const std::string& property, const std::string& value);

protected:
    Vector2f position_;
    Vector2f size_;
    float rotation_;
    Vector2f scale_;

    bool visible_;
    bool enabled_;
    bool focused_;
    bool hovered_;

    std::shared_ptr<Theme> theme_;
    std::map<std::string, std::string> customStyles_;
    std::map<Event::Type, EventCallback> eventHandlers_;

    virtual void onThemeChanged();
    virtual void updateLayout();
    virtual void updateState();
};

} // namespace gui