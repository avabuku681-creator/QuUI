#pragma once
#include <memory>
#include <string>
#include <functional>
#include "../core/widget_base.hpp"

namespace gui {

class Menu : public Widget {
public:
    class MenuItem {
    public:
        MenuItem(const std::string& text = "", std::function<void()> callback = nullptr);
        
        void setText(const std::string& text);
        void setIcon(const std::string& iconPath);
        void setEnabled(bool enabled);
        void setChecked(bool checked);
        void setCallback(std::function<void()> callback);
        void addSubmenu(std::shared_ptr<Menu> submenu);
        
        const std::string& getText() const;
        bool isEnabled() const;
        bool isChecked() const;
        bool hasSubmenu() const;
        
    private:
        std::string text_;
        std::string iconPath_;
        bool enabled_ = true;
        bool checked_ = false;
        std::function<void()> callback_;
        std::shared_ptr<Menu> submenu_;
    };
    
    void addItem(std::shared_ptr<MenuItem> item);
    void removeItem(std::shared_ptr<MenuItem> item);
    void clear();
    
    void setOrientation(bool horizontal);
    void setItemSpacing(float spacing);
    
protected:
    void render() const override;
    void handleEvent(const Event& event) override;
    void updateLayout() override;
    
private:
    std::vector<std::shared_ptr<MenuItem>> items_;
    bool horizontal_ = false;
    float itemSpacing_ = 5.0f;
    int hoveredIndex_ = -1;
};

class MenuBar : public Widget {
public:
    void addMenu(const std::string& text, std::shared_ptr<Menu> menu);
    void removeMenu(const std::string& text);
    void clear();
    
protected:
    void render() const override;
    void handleEvent(const Event& event) override;
    void updateLayout() override;
    
private:
    std::unordered_map<std::string, std::shared_ptr<Menu>> menus_;
    std::string activeMenu_;
};

class ContextMenu : public Menu {
public:
    static void show(const Vector2f& position);
    static void hide();
    static bool isVisible();
    
protected:
    void render() const override;
    void handleEvent(const Event& event) override;
    
private:
    static ContextMenu* instance_;
    Vector2f showPosition_;
};

class ToolBar : public Widget {
public:
    void addButton(const std::string& text, const std::string& iconPath, std::function<void()> callback);
    void addSeparator();
    void addWidget(std::shared_ptr<Widget> widget);
    void clear();
    
    void setOrientation(bool horizontal);
    void setItemSpacing(float spacing);
    
protected:
    void render() const override;
    void updateLayout() override;
    
private:
    struct ToolBarItem {
        enum class Type { Button, Separator, Widget };
        Type type;
        std::string text;
        std::string iconPath;
        std::function<void()> callback;
        std::shared_ptr<Widget> widget;
    };
    
    std::vector<ToolBarItem> items_;
    bool horizontal_ = true;
    float itemSpacing_ = 5.0f;
};

} // namespace gui