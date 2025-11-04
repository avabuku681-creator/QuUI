# Theme System

The QuUI Theme System provides a powerful and flexible way to manage the visual appearance of your application.

## Theme Concepts

Themes in QuUI are based on:
- Colors
- Fonts
- Sizes
- Styles
- Animations

### Basic Theme Structure

```cpp
struct Theme {
    // Colors
    std::map<std::string, Color> colors;
    
    // Fonts
    std::map<std::string, Font> fonts;
    
    // Sizes
    std::map<std::string, float> sizes;
    
    // Styles
    std::map<std::string, Style> styles;
    
    // Animations
    std::map<std::string, Animation> animations;
};
```

## Using Themes

### Creating a Theme

```cpp
auto theme = std::make_shared<Theme>();

// Set colors
theme->SetColor("background", Color(0.2f, 0.2f, 0.2f));
theme->SetColor("primary", Color(0.3f, 0.6f, 0.9f));
theme->SetColor("text", Color(1.0f, 1.0f, 1.0f));

// Set fonts
theme->SetFont("heading", Font("Arial", 24.0f));
theme->SetFont("body", Font("Arial", 14.0f));

// Set sizes
theme->SetSize("padding", 8.0f);
theme->SetSize("margin", 16.0f);

// Apply theme
ui.SetTheme(theme);
```

### Theme Variables

```cpp
// Colors
"background"      // Main background
"primary"         // Primary color
"secondary"       // Secondary color
"accent"          // Accent color
"text"            // Main text color
"text-secondary"  // Secondary text
"border"          // Border color
"error"           // Error state
"success"         // Success state
"warning"         // Warning state

// Fonts
"heading"         // Heading font
"body"            // Body text font
"monospace"       // Code font

// Sizes
"padding-small"   // Small padding
"padding"         // Normal padding
"padding-large"   // Large padding
"margin-small"    // Small margin
"margin"          // Normal margin
"margin-large"    // Large margin
"border-width"    // Border width
"border-radius"   // Border radius
```

## Theme Templates

### Light Theme

```cpp
auto lightTheme = std::make_shared<Theme>();

// Colors
lightTheme->SetColor("background", Color(0.98f, 0.98f, 0.98f));
lightTheme->SetColor("primary", Color(0.2f, 0.6f, 0.9f));
lightTheme->SetColor("secondary", Color(0.7f, 0.7f, 0.7f));
lightTheme->SetColor("text", Color(0.1f, 0.1f, 0.1f));
lightTheme->SetColor("border", Color(0.8f, 0.8f, 0.8f));

// Fonts
lightTheme->SetFont("heading", Font("Arial", 24.0f, FontWeight::Bold));
lightTheme->SetFont("body", Font("Arial", 14.0f, FontWeight::Regular));

// Apply theme
ui.SetTheme(lightTheme);
```

### Dark Theme

```cpp
auto darkTheme = std::make_shared<Theme>();

// Colors
darkTheme->SetColor("background", Color(0.1f, 0.1f, 0.1f));
darkTheme->SetColor("primary", Color(0.3f, 0.7f, 1.0f));
darkTheme->SetColor("secondary", Color(0.3f, 0.3f, 0.3f));
darkTheme->SetColor("text", Color(0.9f, 0.9f, 0.9f));
darkTheme->SetColor("border", Color(0.2f, 0.2f, 0.2f));

// Apply theme
ui.SetTheme(darkTheme);
```

## Widget-Specific Styling

### Button Styling

```cpp
// In theme
theme->SetStyle("button", Style()
    .Background(colors.primary)
    .TextColor(colors.text)
    .Padding(8.0f)
    .BorderRadius(4.0f)
    .Transition("background", 0.2f));

theme->SetStyle("button:hover", Style()
    .Background(colors.primaryLight)
    .Scale(1.05f));

theme->SetStyle("button:active", Style()
    .Background(colors.primaryDark)
    .Scale(0.95f));
```

### Input Styling

```cpp
theme->SetStyle("input", Style()
    .Background(colors.background)
    .Border(colors.border)
    .BorderWidth(1.0f)
    .BorderRadius(4.0f)
    .Padding(8.0f));

theme->SetStyle("input:focus", Style()
    .BorderColor(colors.primary)
    .BoxShadow(0, 0, 8, colors.primaryAlpha));
```

## Dynamic Theming

### Theme Switching

```cpp
// Create themes
auto lightTheme = CreateLightTheme();
auto darkTheme = CreateDarkTheme();

// Switch themes
bool isDarkMode = false;
auto switchTheme = [&]() {
    isDarkMode = !isDarkMode;
    ui.SetTheme(isDarkMode ? darkTheme : lightTheme);
};

// Create theme toggle
auto themeToggle = std::make_shared<Button>();
themeToggle->SetText("Toggle Theme");
themeToggle->OnClick([switchTheme](const MouseEvent&) {
    switchTheme();
});
```

### Animated Theme Transitions

```cpp
// Enable theme transitions
theme->SetTransitionDuration(0.3f);
theme->SetTransitionEasing(Easing::EaseInOut);

// Properties that will animate
theme->SetTransitionProperties({
    "background",
    "color",
    "border-color",
    "opacity"
});
```

## Best Practices

1. **Consistent Naming**
   - Use clear, descriptive names for theme variables
   - Follow a naming convention
   - Document custom variables

2. **Color Management**
   - Use color schemes
   - Define color relationships
   - Consider accessibility

3. **Responsive Design**
   - Use relative units
   - Define breakpoints
   - Scale appropriately

4. **Performance**
   - Cache theme values
   - Minimize runtime calculations
   - Use efficient selectors

5. **Maintenance**
   - Keep themes modular
   - Document changes
   - Version control themes

## Theme Inheritance

```cpp
// Base theme
auto baseTheme = std::make_shared<Theme>();
// ... set base properties

// Derived theme
auto customTheme = std::make_shared<Theme>(baseTheme);
// Override or add specific properties
```

## Theme Export/Import

```cpp
// Export theme to JSON
std::string json = theme->ToJSON();

// Import theme from JSON
auto theme = Theme::FromJSON(json);
```

## Advanced Features

### Theme Variants

```cpp
theme->AddVariant("compact", ThemeVariant()
    .SetSize("padding", 4.0f)
    .SetSize("margin", 8.0f));

// Apply variant
ui.SetThemeVariant("compact");
```

### Custom Properties

```cpp
// Add custom property
theme->SetCustomProperty("card-elevation", 2.0f);

// Use custom property
auto value = theme->GetCustomProperty<float>("card-elevation");
```