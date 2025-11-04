#pragma once
#include "../core/widget_base.hpp"
#include <functional>

namespace gui {

// Интерфейс для компоновщиков
class LayoutManager {
public:
    virtual ~LayoutManager() = default;
    virtual void layout(Widget* container) = 0;
    virtual Vector2f getMinimumSize() const = 0;
    virtual Vector2f getPreferredSize() const = 0;
};

// Параметры компоновки для виджетов
struct LayoutParams {
    // Выравнивание
    enum class Alignment {
        Start,
        Center,
        End,
        Stretch
    };

    // Привязки
    bool expandHorizontal = false;
    bool expandVertical = false;
    Alignment horizontalAlignment = Alignment::Start;
    Alignment verticalAlignment = Alignment::Start;
    
    // Отступы
    float marginLeft = 0;
    float marginTop = 0;
    float marginRight = 0;
    float marginBottom = 0;
    
    // Минимальные и максимальные размеры
    Vector2f minSize;
    Vector2f maxSize;
    
    // Вес для распределения пространства
    float weight = 1.0f;
};

// Сетка
class GridLayoutManager : public LayoutManager {
public:
    GridLayoutManager(int columns, int rows);
    
    void setSpacing(float horizontal, float vertical);
    void setCellAlignment(int column, int row, LayoutParams::Alignment halign, LayoutParams::Alignment valign);
    void setColumnWeight(int column, float weight);
    void setRowWeight(int row, float weight);
    
    void layout(Widget* container) override;
    Vector2f getMinimumSize() const override;
    Vector2f getPreferredSize() const override;

private:
    int columns_;
    int rows_;
    float horizontalSpacing_;
    float verticalSpacing_;
    std::vector<float> columnWeights_;
    std::vector<float> rowWeights_;
    std::vector<std::pair<LayoutParams::Alignment, LayoutParams::Alignment>> cellAlignments_;
};

// Flex-подобная компоновка
class FlexLayoutManager : public LayoutManager {
public:
    enum class Direction {
        Row,
        Column
    };

    enum class JustifyContent {
        Start,
        Center,
        End,
        SpaceBetween,
        SpaceAround,
        SpaceEvenly
    };

    enum class AlignItems {
        Start,
        Center,
        End,
        Stretch,
        Baseline
    };

    FlexLayoutManager(Direction direction = Direction::Row);
    
    void setDirection(Direction direction);
    void setJustifyContent(JustifyContent justify);
    void setAlignItems(AlignItems align);
    void setWrap(bool wrap);
    void setSpacing(float spacing);
    
    void layout(Widget* container) override;
    Vector2f getMinimumSize() const override;
    Vector2f getPreferredSize() const override;

private:
    Direction direction_;
    JustifyContent justifyContent_;
    AlignItems alignItems_;
    bool wrap_;
    float spacing_;
};

// Компоновка с привязками (как в WPF)
class AnchorLayoutManager : public LayoutManager {
public:
    struct AnchorConstraint {
        Widget* target = nullptr;
        float offset = 0;
    };

    void setAnchor(Widget* widget, const AnchorConstraint& left, const AnchorConstraint& top,
                   const AnchorConstraint& right, const AnchorConstraint& bottom);
    
    void layout(Widget* container) override;
    Vector2f getMinimumSize() const override;
    Vector2f getPreferredSize() const override;

private:
    struct WidgetConstraints {
        AnchorConstraint left, top, right, bottom;
    };
    std::unordered_map<Widget*, WidgetConstraints> constraints_;
};

// Компоновка с процентными размерами
class PercentLayoutManager : public LayoutManager {
public:
    void setWidgetConstraints(Widget* widget, float x, float y, float width, float height);
    
    void layout(Widget* container) override;
    Vector2f getMinimumSize() const override;
    Vector2f getPreferredSize() const override;

private:
    struct PercentConstraints {
        float x, y, width, height;
    };
    std::unordered_map<Widget*, PercentConstraints> constraints_;
};

// Вспомогательные функции для работы с компоновкой
namespace LayoutUtils {
    Vector2f calculateAlignedPosition(const Vector2f& containerSize, const Vector2f& widgetSize,
                                    LayoutParams::Alignment hAlign, LayoutParams::Alignment vAlign);
    
    Vector2f calculateExpandedSize(const Vector2f& availableSize, const Vector2f& minSize,
                                 const Vector2f& maxSize, bool expandH, bool expandV);
    
    void distributeSpace(std::vector<float>& sizes, const std::vector<float>& weights,
                        float totalSpace, float totalWeight);
}

} // namespace gui