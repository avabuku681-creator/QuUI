#pragma once
#include <vector>
#include <memory>
#include "../core/widget_base.hpp"

namespace gui {

class Layout {
public:
    virtual ~Layout() = default;
    virtual void arrange(Container* container) = 0;
};

class GridLayout : public Layout {
public:
    GridLayout(int columns, int rows);
    void setSpacing(float horizontal, float vertical);
    void setCellSize(float width, float height);
    void setAutoSize(bool autoSize);
    void arrange(Container* container) override;

private:
    int columns_;
    int rows_;
    float horizontalSpacing_;
    float verticalSpacing_;
    float cellWidth_;
    float cellHeight_;
    bool autoSize_;
};

class FlexLayout : public Layout {
public:
    enum class Direction {
        Row,
        Column
    };

    enum class Alignment {
        Start,
        Center,
        End,
        SpaceBetween,
        SpaceAround
    };

    FlexLayout(Direction direction = Direction::Row);
    void setDirection(Direction direction);
    void setAlignment(Alignment alignment);
    void setSpacing(float spacing);
    void setWrap(bool wrap);
    void arrange(Container* container) override;

private:
    Direction direction_;
    Alignment alignment_;
    float spacing_;
    bool wrap_;
};

class DockLayout : public Layout {
public:
    enum class DockEdge {
        Left,
        Top,
        Right,
        Bottom,
        Fill
    };

    void setMargin(float margin);
    void setDock(Widget* widget, DockEdge edge);
    void arrange(Container* container) override;

private:
    float margin_;
    std::map<Widget*, DockEdge> dockEdges_;
};

} // namespace gui