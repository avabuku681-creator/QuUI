#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "../core/math_types.hpp"

namespace gui {

// Типы анимации для плавности переходов
enum class EaseType {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut
};

// Базовый класс для стилей
class Style {
public:
    virtual ~Style() = default;

    // Геометрия
    float margin = 0.0f;
    float padding = 5.0f;
    float borderWidth = 1.0f;
    float borderRadius = 0.0f;
    float opacity = 1.0f;

    // Цвета
    Color backgroundColor = Color(0.2f, 0.2f, 0.2f, 1.0f);
    Color foregroundColor = Color::white();
    Color borderColor = Color(0.5f, 0.5f, 0.5f, 1.0f);
    Color accentColor = Color(0.4f, 0.6f, 1.0f, 1.0f);
    Color hoverColor = Color(0.3f, 0.3f, 0.3f, 1.0f);
    Color activeColor = Color(0.25f, 0.25f, 0.25f, 1.0f);
    Color disabledColor = Color(0.5f, 0.5f, 0.5f, 0.5f);

    // Шрифт
    std::string fontFamily = "Arial";
    float fontSize = 14.0f;
    bool fontBold = false;
    bool fontItalic = false;
    Color textColor = Color::white();

    // Тени
    struct Shadow {
        Vector2f offset = Vector2f(2.0f, 2.0f);
        float blur = 4.0f;
        Color color = Color(0, 0, 0, 0.25f);
        bool enabled = false;
    } shadow;

    // Градиент
    struct Gradient {
        Color startColor;
        Color endColor;
        float angle = 0.0f;
        bool enabled = false;
    } gradient;

    // Анимация
    struct Animation {
        float duration = 0.2f;
        EaseType easing = EaseType::EaseInOut;
        bool enabled = true;
    } animation;

    // Клонирование стиля
    virtual std::unique_ptr<Style> clone() const {
        return std::make_unique<Style>(*this);
    }
};

// Система управления стилями
class StyleManager {
public:
    static StyleManager& getInstance() {
        static StyleManager instance;
        return instance;
    }

    // Регистрация и получение стилей
    void registerStyle(const std::string& name, std::unique_ptr<Style> style);
    const Style* getStyle(const std::string& name) const;
    void removeStyle(const std::string& name);

    // Наследование стилей
    void setParentStyle(const std::string& child, const std::string& parent);
    std::unique_ptr<Style> computeStyle(const std::string& name) const;

    // Состояния стилей
    void setStateStyle(const std::string& name, const std::string& state, std::unique_ptr<Style> style);
    const Style* getStateStyle(const std::string& name, const std::string& state) const;

private:
    StyleManager() = default;
    std::unordered_map<std::string, std::unique_ptr<Style>> styles_;
    std::unordered_map<std::string, std::string> styleHierarchy_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<Style>>> stateStyles_;
};

// Предопределенные стили для разных виджетов
namespace Styles {
    std::unique_ptr<Style> createButtonStyle();
    std::unique_ptr<Style> createCheckboxStyle();
    std::unique_ptr<Style> createSliderStyle();
    std::unique_ptr<Style> createTextInputStyle();
    std::unique_ptr<Style> createPanelStyle();
    std::unique_ptr<Style> createWindowStyle();
    std::unique_ptr<Style> createMenuStyle();
    std::unique_ptr<Style> createToolBarStyle();
}

// Классы для конкретных стилей виджетов
class ButtonStyle : public Style {
public:
    float pressedOffset = 1.0f;
    bool useGradient = true;
    
    std::unique_ptr<Style> clone() const override {
        return std::make_unique<ButtonStyle>(*this);
    }
};

class SliderStyle : public Style {
public:
    float handleSize = 16.0f;
    float trackHeight = 4.0f;
    Color trackColor = Color(0.3f, 0.3f, 0.3f, 1.0f);
    Color handleColor = Color::white();
    
    std::unique_ptr<Style> clone() const override {
        return std::make_unique<SliderStyle>(*this);
    }
};

class WindowStyle : public Style {
public:
    float titleBarHeight = 30.0f;
    Color titleBarColor = Color(0.15f, 0.15f, 0.15f, 1.0f);
    Color titleTextColor = Color::white();
    bool showCloseButton = true;
    bool showMinimizeButton = true;
    bool showMaximizeButton = true;
    
    std::unique_ptr<Style> clone() const override {
        return std::make_unique<WindowStyle>(*this);
    }
};

} // namespace gui