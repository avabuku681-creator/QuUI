#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../core/widget_base.hpp"

namespace gui {

class Label : public Widget {
public:
    explicit Label(const std::string& text = "");
    
    void setText(const std::string& text);
    void setFont(const std::string& fontPath);
    void setFontSize(float size);
    void setTextColor(const Color& color);
    void setAlignment(Alignment alignment);
    void setWordWrap(bool wrap);
    
protected:
    void render() const override;
    void updateLayout() override;
    
private:
    std::string text_;
    std::string fontPath_;
    float fontSize_ = 12.0f;
    Color textColor_;
    Alignment alignment_ = Alignment::Left;
    bool wordWrap_ = false;
};

class Image : public Widget {
public:
    explicit Image(const std::string& imagePath = "");
    
    void setImage(const std::string& imagePath);
    void setTint(const Color& tint);
    void setStretch(bool stretch);
    void setKeepAspectRatio(bool keep);
    
protected:
    void render() const override;
    void updateLayout() override;
    
private:
    std::string imagePath_;
    Color tint_;
    bool stretch_ = false;
    bool keepAspectRatio_ = true;
};

class ProgressBar : public Widget {
public:
    ProgressBar(float min = 0.0f, float max = 100.0f);
    
    void setValue(float value);
    float getValue() const;
    void setRange(float min, float max);
    void setShowText(bool show);
    void setOrientation(bool horizontal);
    
protected:
    void render() const override;
    void updateLayout() override;
    
private:
    float minValue_;
    float maxValue_;
    float currentValue_;
    bool showText_ = true;
    bool horizontal_ = true;
};

class ScrollBar : public Widget {
public:
    ScrollBar(bool horizontal = true);
    
    void setRange(float min, float max);
    void setValue(float value);
    float getValue() const;
    void setPageSize(float size);
    void setOrientation(bool horizontal);
    
protected:
    void render() const override;
    void handleEvent(const Event& event) override;
    void updateLayout() override;
    
private:
    float minValue_;
    float maxValue_;
    float currentValue_;
    float pageSize_;
    bool horizontal_;
    bool dragging_ = false;
    Vector2f dragOffset_;
};

class TabView : public Widget {
public:
    void addTab(const std::string& title, std::shared_ptr<Widget> content);
    void removeTab(const std::string& title);
    void setActiveTab(const std::string& title);
    std::string getActiveTab() const;
    
protected:
    void render() const override;
    void handleEvent(const Event& event) override;
    void updateLayout() override;
    
private:
    struct Tab {
        std::string title;
        std::shared_ptr<Widget> content;
    };
    
    std::vector<Tab> tabs_;
    size_t activeTab_ = 0;
};

class Separator : public Widget {
public:
    explicit Separator(bool horizontal = true);
    
    void setOrientation(bool horizontal);
    void setColor(const Color& color);
    void setThickness(float thickness);
    
protected:
    void render() const override;
    
private:
    bool horizontal_;
    Color color_;
    float thickness_ = 1.0f;
};

} // namespace gui