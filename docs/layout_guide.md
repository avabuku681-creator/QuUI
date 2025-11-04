# Layout Guide

The QuUI Layout System provides flexible and powerful ways to arrange widgets in your UI.

## Layout Types

### FlowLayout
Arranges widgets in a flow, similar to text:

```cpp
auto container = std::make_shared<Container>();
auto flowLayout = std::make_shared<FlowLayout>();
flowLayout->SetDirection(FlowLayout::Horizontal);
flowLayout->SetSpacing(10.0f);
container->SetLayout(flowLayout);
```

Properties:
- Direction (Horizontal/Vertical)
- Spacing
- Alignment
- Wrap behavior

### GridLayout
Arranges widgets in a grid:

```cpp
auto gridLayout = std::make_shared<GridLayout>(3, 2); // 3 columns, 2 rows
gridLayout->SetCellSpacing(5.0f, 5.0f);
gridLayout->SetColumnWidths({100, 100, 100});
container->SetLayout(gridLayout);
```

Properties:
- Columns and rows
- Cell spacing
- Column/row sizes
- Cell alignment

### StackLayout
Stacks widgets with optional overlapping:

```cpp
auto stackLayout = std::make_shared<StackLayout>();
stackLayout->SetStackingOrder(StackLayout::BackToFront);
container->SetLayout(stackLayout);
```

Properties:
- Stacking order
- Z-index management
- Alignment
- Overlap behavior

### FlexLayout
Flexible box layout system:

```cpp
auto flexLayout = std::make_shared<FlexLayout>();
flexLayout->SetDirection(FlexLayout::Row);
flexLayout->SetJustifyContent(FlexLayout::SpaceBetween);
flexLayout->SetAlignItems(FlexLayout::Center);
container->SetLayout(flexLayout);
```

Properties:
- Direction (Row/Column)
- Justify content
- Align items
- Flex grow/shrink
- Wrap behavior

## Layout Properties

### Size Properties
```cpp
widget->SetPreferredSize(200, 100);
widget->SetMinimumSize(100, 50);
widget->SetMaximumSize(300, 150);
widget->SetAspectRatio(16.0f/9.0f);
```

### Margin and Padding
```cpp
widget->SetMargin(10);  // All sides
widget->SetMargin(10, 20);  // Horizontal, Vertical
widget->SetMargin(10, 20, 10, 20);  // Left, Top, Right, Bottom

widget->SetPadding(5);  // All sides
widget->SetPadding(5, 10);  // Horizontal, Vertical
widget->SetPadding(5, 10, 5, 10);  // Left, Top, Right, Bottom
```

### Alignment
```cpp
widget->SetAlignment(Alignment::Center);
widget->SetVerticalAlignment(Alignment::Top);
widget->SetHorizontalAlignment(Alignment::Left);
```

## Layout Constraints

```cpp
// Fixed size constraint
auto constraint = std::make_shared<FixedConstraint>(200, 100);
widget->SetConstraint(constraint);

// Aspect ratio constraint
auto aspectConstraint = std::make_shared<AspectRatioConstraint>(16.0f/9.0f);
widget->SetConstraint(aspectConstraint);

// Relative size constraint
auto relativeConstraint = std::make_shared<RelativeConstraint>(0.5f, 0.5f);
widget->SetConstraint(relativeConstraint);
```

## Responsive Layout

### Breakpoints
```cpp
auto layout = std::make_shared<ResponsiveLayout>();
layout->AddBreakpoint(768, [](Container* container) {
    // Mobile layout
    auto mobileLayout = std::make_shared<FlowLayout>();
    mobileLayout->SetDirection(FlowLayout::Vertical);
    return mobileLayout;
});

layout->AddBreakpoint(1024, [](Container* container) {
    // Tablet layout
    auto tabletLayout = std::make_shared<GridLayout>(2, 2);
    return tabletLayout;
});

layout->AddBreakpoint(1920, [](Container* container) {
    // Desktop layout
    auto desktopLayout = std::make_shared<GridLayout>(3, 2);
    return desktopLayout;
});
```

### Dynamic Layouts
```cpp
auto dynamicLayout = std::make_shared<DynamicLayout>();
dynamicLayout->OnLayoutChange([](const Size& newSize) {
    if (newSize.width < 768) {
        return CreateMobileLayout();
    } else if (newSize.width < 1024) {
        return CreateTabletLayout();
    } else {
        return CreateDesktopLayout();
    }
});
```

## Layout Animations

```cpp
// Animate layout changes
container->AnimateLayout()
    .Duration(0.3f)
    .Easing(Easing::EaseInOut)
    .OnComplete([]() {
        // Animation completed
    });

// Specific widget layout animation
widget->AnimateLayout()
    .Property("x")
    .From(0)
    .To(100)
    .Duration(0.3f)
    .Start();
```

## Performance Optimization

### Layout Caching
```cpp
// Enable layout caching
container->SetLayoutCaching(true);

// Invalidate layout when needed
container->InvalidateLayout();
```

### Batch Updates
```cpp
container->BeginUpdate();
// Make multiple changes
widget1->SetSize(100, 100);
widget2->SetPosition(200, 200);
widget3->SetVisible(false);
container->EndUpdate(); // Layout recalculated once
```

### Virtual Layout
```cpp
auto virtualLayout = std::make_shared<VirtualLayout>();
virtualLayout->SetItemSize(100, 30);
virtualLayout->SetItemCount(1000);
virtualLayout->SetItemGenerator([](int index) {
    return CreateItemWidget(index);
});
```

## Best Practices

1. **Choose the Right Layout**
   - Use FlowLayout for simple linear arrangements
   - Use GridLayout for tabular data
   - Use FlexLayout for complex responsive layouts
   - Use StackLayout for overlapping elements

2. **Optimize Performance**
   - Minimize layout depth
   - Use layout caching when possible
   - Batch multiple updates
   - Use virtual layouts for large lists

3. **Responsive Design**
   - Define clear breakpoints
   - Test layouts at different sizes
   - Use relative sizes when possible
   - Consider mobile-first approach

4. **Maintenance**
   - Keep layouts modular
   - Document layout decisions
   - Use consistent naming
   - Regular testing at different sizes