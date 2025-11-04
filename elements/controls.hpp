#pragma once
#include <string>
#include <functional>
#include "../core/widget_base.hpp"

namespace gui {

class Button : public Widget {
public:
    enum class Style {
        Normal,
        Primary,
        Secondary,
        Danger,
        Success,
        Custom
    };

    explicit Button(const std::string& text = "");

    void setText(const std::string& text);
    void setIcon(const std::string& iconPath);
    void setStyle(Style style);
    void setClickCallback(std::function<void()> callback);
    
    bool isPressed() const;
    bool isHovered() const;

protected:
    void render() const override;
    void handleEvent(const Event& event) override;
    void updateState() override;

private:
    std::string text_;
    std::string iconPath_;
    Style style_ = Style::Normal;
    bool pressed_ = false;
    std::function<void()> clickCallback_;
};

class Checkbox : public Widget {
public:
    explicit Checkbox(const std::string& label = "");

    void setChecked(bool checked);
    bool isChecked() const;
    void setLabel(const std::string& label);
    void setChangeCallback(std::function<void(bool)> callback);

protected:
    void render() const override;
    void handleEvent(const Event& event) override;

private:
    std::string label_;
    bool checked_ = false;
    std::function<void(bool)> changeCallback_;
};

class RadioButton : public Widget {
public:
    explicit RadioButton(const std::string& label = "");

    void setGroup(const std::string& group);
    void setSelected(bool selected);
    bool isSelected() const;
    void setLabel(const std::string& label);
    void setChangeCallback(std::function<void()> callback);

protected:
    void render() const override;
    void handleEvent(const Event& event) override;

private:
    std::string label_;
    std::string group_;
    bool selected_ = false;
    std::function<void()> changeCallback_;
};

class Slider : public Widget {
public:
    Slider(float min = 0.0f, float max = 1.0f);

    void setRange(float min, float max);
    void setValue(float value);
    float getValue() const;
    void setStep(float step);
    void setOrientation(bool horizontal);
    void setChangeCallback(std::function<void(float)> callback);

protected:
    void render() const override;
    void handleEvent(const Event& event) override;
    void updateLayout() override;

private:
    float minValue_;
    float maxValue_;
    float currentValue_;
    float step_ = 0.1f;
    bool horizontal_ = true;
    bool dragging_ = false;
    std::function<void(float)> changeCallback_;
};

class TextInput : public Widget {
public:
    explicit TextInput(const std::string& text = "");

    void setText(const std::string& text);
    std::string getText() const;
    void setPlaceholder(const std::string& placeholder);
    void setPasswordMode(bool enabled);
    void setMaxLength(size_t maxLength);
    void setChangeCallback(std::function<void(const std::string&)> callback);

protected:
    void render() const override;
    void handleEvent(const Event& event) override;

private:
    std::string text_;
    std::string placeholder_;
    bool passwordMode_ = false;
    size_t maxLength_ = 0;
    size_t cursorPosition_ = 0;
    std::function<void(const std::string&)> changeCallback_;
};

} // namespace gui