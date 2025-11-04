#pragma once
#include <vector>
#include <string>
#include "../core/widget_base.hpp"

namespace gui {

class Container : public Widget {
public:
    void addChild(std::shared_ptr<Widget> child);
    void removeChild(std::shared_ptr<Widget> child);
    void clearChildren();

    const std::vector<std::shared_ptr<Widget>>& getChildren() const;
    
    void update(float deltaTime) override;
    void render() const override;
    void handleEvent(const Event& event) override;

protected:
    std::vector<std::shared_ptr<Widget>> children_;
    virtual void onChildAdded(std::shared_ptr<Widget> child);
    virtual void onChildRemoved(std::shared_ptr<Widget> child);
};

class Panel : public Container {
public:
    void setBorderVisible(bool visible);
    void setBackgroundColor(const Color& color);
    void setBorderColor(const Color& color);
    void setBorderThickness(float thickness);
    void setCornerRadius(float radius);

private:
    bool borderVisible_ = true;
    Color backgroundColor_;
    Color borderColor_;
    float borderThickness_ = 1.0f;
    float cornerRadius_ = 0.0f;
};

class Window : public Panel {
public:
    void setTitle(const std::string& title);
    void setDraggable(bool draggable);
    void setResizable(bool resizable);
    void setMinSize(const Vector2f& minSize);
    void setMaxSize(const Vector2f& maxSize);

    bool isDragging() const;
    bool isResizing() const;

protected:
    void handleEvent(const Event& event) override;
    void updateLayout() override;

private:
    std::string title_;
    bool draggable_ = true;
    bool resizable_ = true;
    bool dragging_ = false;
    bool resizing_ = false;
    Vector2f minSize_;
    Vector2f maxSize_;
    Vector2f dragOffset_;
};

class ScrollArea : public Container {
public:
    void setScrollable(bool horizontal, bool vertical);
    void setScrollSpeed(float speed);
    void setScrollPosition(const Vector2f& position);
    Vector2f getScrollPosition() const;
    
protected:
    void handleEvent(const Event& event) override;
    void updateLayout() override;

private:
    bool scrollableH_ = true;
    bool scrollableV_ = true;
    float scrollSpeed_ = 1.0f;
    Vector2f scrollPosition_;
    Vector2f contentSize_;
};

} // namespace gui