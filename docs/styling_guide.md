# Styling Guide

This guide covers the styling and customization features available in QuUI SDK.

## Basic Styling

### Widget Styles

```cpp
// Basic widget styling
widget->SetStyle(Style()
    .Background(Color(0.2f, 0.2f, 0.2f))
    .TextColor(Color(1.0f, 1.0f, 1.0f))
    .BorderColor(Color(0.3f, 0.3f, 0.3f))
    .BorderWidth(1.0f)
    .BorderRadius(4.0f)
    .Padding(8.0f)
    .Margin(4.0f)
    .FontFamily("Arial")
    .FontSize(14.0f)
    .FontWeight(FontWeight::Regular)
);

// Style with hover state
widget->SetHoverStyle(Style()
    .Background(Color(0.25f, 0.25f, 0.25f))
    .Scale(1.05f)
);

// Style with active state
widget->SetActiveStyle(Style()
    .Background(Color(0.3f, 0.3f, 0.3f))
    .Scale(0.95f)
);
```

### Theme System

```cpp
// Create theme
auto theme = std::make_shared<Theme>();

// Set colors
theme->SetColor("background", Color(0.1f, 0.1f, 0.1f));
theme->SetColor("primary", Color(0.2f, 0.6f, 1.0f));
theme->SetColor("secondary", Color(0.8f, 0.2f, 0.2f));
theme->SetColor("text", Color(1.0f, 1.0f, 1.0f));

// Set fonts
theme->SetFont("heading", Font("Arial", 24.0f, FontWeight::Bold));
theme->SetFont("body", Font("Arial", 14.0f, FontWeight::Regular));

// Set sizes
theme->SetSize("padding-small", 4.0f);
theme->SetSize("padding", 8.0f);
theme->SetSize("padding-large", 16.0f);

// Apply theme
ui.SetTheme(theme);
```

## Advanced Styling

### Style Classes

```cpp
// Define style class
auto buttonClass = StyleClass()
    .Background(colors.primary)
    .TextColor(colors.text)
    .Padding(8.0f)
    .BorderRadius(4.0f);

// Apply to multiple widgets
widget1->AddStyleClass("button", buttonClass);
widget2->AddStyleClass("button", buttonClass);

// Modify style class
StyleManager::GetClass("button")
    .SetProperty("background", colors.secondary);
```

### Style Inheritance

```cpp
// Base style
auto baseStyle = Style()
    .Background(colors.background)
    .TextColor(colors.text);

// Derived style
auto derivedStyle = baseStyle
    .BorderColor(colors.border)
    .BorderWidth(1.0f);

// Apply style
widget->SetStyle(derivedStyle);
```

### Style Animations

```cpp
// Transition definition
widget->SetTransition("background", 0.3f, Easing::EaseInOut);
widget->SetTransition("scale", 0.2f, Easing::EaseOut);

// Multiple transitions
widget->SetTransitions({
    {"background", 0.3f, Easing::EaseInOut},
    {"scale", 0.2f, Easing::EaseOut},
    {"opacity", 0.2f, Easing::Linear}
});
```

## Layout Styling

### Grid System

```cpp
// Create grid container
auto grid = std::make_shared<Grid>();
grid->SetColumns(12);
grid->SetGutter(10.0f);

// Style grid items
auto item = std::make_shared<Widget>();
item->SetGridSpan(4);  // Span 4 columns
item->SetGridOffset(2);  // Offset by 2 columns
```

### Flexbox

```cpp
// Create flex container
auto flex = std::make_shared<FlexContainer>();
flex->SetDirection(FlexDirection::Row);
flex->SetJustify(FlexJustify::SpaceBetween);
flex->SetAlign(FlexAlign::Center);

// Style flex items
auto item = std::make_shared<Widget>();
item->SetFlexGrow(1);
item->SetFlexShrink(0);
item->SetFlexBasis("auto");
```

## Visual Effects

### Shadows

```cpp
// Add shadow
widget->SetStyle(Style()
    .BoxShadow(0, 2, 4, Color(0, 0, 0, 0.2f))
);

// Multiple shadows
widget->SetStyle(Style()
    .BoxShadow({
        {0, 2, 4, Color(0, 0, 0, 0.1f)},
        {0, 4, 8, Color(0, 0, 0, 0.2f)}
    })
);
```

### Gradients

```cpp
// Linear gradient
widget->SetStyle(Style()
    .Background(Gradient()
        .Linear()
        .AddStop(0.0f, Color(1, 0, 0))
        .AddStop(1.0f, Color(0, 0, 1))
        .Angle(45.0f)
    )
);

// Radial gradient
widget->SetStyle(Style()
    .Background(Gradient()
        .Radial()
        .AddStop(0.0f, Color(1, 1, 1))
        .AddStop(1.0f, Color(0, 0, 0))
        .Center(0.5f, 0.5f)
    )
);
```

### Filters

```cpp
// Add blur
widget->SetStyle(Style()
    .Filter(Filter().Blur(5.0f))
);

// Multiple filters
widget->SetStyle(Style()
    .Filter(Filter()
        .Blur(5.0f)
        .Brightness(1.2f)
        .Contrast(1.1f)
        .Saturate(1.2f)
    )
);
```

## Responsive Styling

### Media Queries

```cpp
// Add responsive styles
StyleManager::AddMediaQuery("(max-width: 768px)", [](Theme* theme) {
    theme->SetSize("padding", 4.0f);
    theme->SetSize("margin", 4.0f);
    theme->SetFont("body", Font("Arial", 12.0f));
});

// Multiple breakpoints
StyleManager::AddMediaQuery("(min-width: 768px) and (max-width: 1024px)", 
    [](Theme* theme) {
        // Tablet styles
    }
);
```

### Conditional Styling

```cpp
// Style based on condition
widget->SetConditionalStyle("dark-mode", Style()
    .Background(colors.darkBackground)
    .TextColor(colors.darkText)
);

// Apply condition
ui.SetCondition("dark-mode", true);
```

## Best Practices

1. **Theme Organization**
```cpp
// Create theme with sections
auto theme = std::make_shared<Theme>();

// Colors
theme->AddSection("colors", {
    {"primary", Color(0.2f, 0.6f, 1.0f)},
    {"secondary", Color(0.8f, 0.2f, 0.2f)},
    {"background", Color(0.1f, 0.1f, 0.1f)},
    {"text", Color(1.0f, 1.0f, 1.0f)}
});

// Typography
theme->AddSection("typography", {
    {"heading", Font("Arial", 24.0f, FontWeight::Bold)},
    {"body", Font("Arial", 14.0f, FontWeight::Regular)}
});

// Spacing
theme->AddSection("spacing", {
    {"small", 4.0f},
    {"medium", 8.0f},
    {"large", 16.0f}
});
```

2. **Style Reuse**
```cpp
// Create reusable styles
auto styles = StyleLibrary::Create();

styles.Add("button-primary", Style()
    .Background(colors.primary)
    .TextColor(colors.text)
    .Padding(8.0f)
);

styles.Add("button-secondary", Style()
    .Background(colors.secondary)
    .TextColor(colors.text)
    .Padding(8.0f)
);

// Apply styles
widget->ApplyStyle(styles.Get("button-primary"));
```

3. **Performance**
```cpp
// Batch style updates
StyleBatch batch;
batch.Begin();

for (auto& widget : widgets) {
    widget->SetStyle(newStyle);
}

batch.Commit();
```

4. **Maintenance**
```cpp
// Document styles
StyleLibrary::AddDescription("button-primary", 
    "Primary button style with brand colors");

// Version styles
StyleLibrary::SetVersion("button-primary", "1.0.0");

// Track usage
StyleLibrary::TrackUsage(true);
```