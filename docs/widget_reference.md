# Widget Reference

This document provides a comprehensive reference for all widgets available in QuUI SDK.

## Basic Widgets

### Label
Text display widget with various styling options.

```cpp
auto label = std::make_shared<Label>();
label->SetText("Hello, World!");
label->SetFont("Arial", 16);
label->SetColor(Color(1.0f, 1.0f, 1.0f));
```

Properties:
- text: The text content
- font: Font family and size
- color: Text color
- alignment: Text alignment
- wordWrap: Word wrapping mode

### Button
Interactive button widget with various states.

```cpp
auto button = std::make_shared<Button>();
button->SetText("Click Me");
button->OnClick([](const MouseEvent& e) {
    // Handle click
});
```

Properties:
- text: Button text
- icon: Optional button icon
- backgroundColor: Background color
- hoverColor: Color when hovered
- pressedColor: Color when pressed
- disabled: Disabled state

### TextInput
Single-line text input field.

```cpp
auto input = std::make_shared<TextInput>();
input->SetPlaceholder("Enter text...");
input->OnTextChanged([](const std::string& text) {
    // Handle text change
});
```

Properties:
- text: Current text content
- placeholder: Placeholder text
- maxLength: Maximum text length
- password: Password mode
- readonly: Read-only mode

### CheckBox
Toggle widget with two states.

```cpp
auto checkbox = std::make_shared<CheckBox>();
checkbox->SetText("Enable Feature");
checkbox->OnToggle([](bool checked) {
    // Handle state change
});
```

Properties:
- checked: Current state
- text: Label text
- disabled: Disabled state

## Container Widgets

### Container
Basic container for other widgets.

```cpp
auto container = std::make_shared<Container>();
container->SetLayout(std::make_shared<FlowLayout>());
container->Add(widget1);
container->Add(widget2);
```

Properties:
- layout: Layout manager
- padding: Internal padding
- spacing: Space between children
- background: Background color/image

### ScrollView
Scrollable container.

```cpp
auto scroll = std::make_shared<ScrollView>();
scroll->SetContent(content);
scroll->SetScrollDirection(ScrollView::Both);
```

Properties:
- content: Scrollable content
- direction: Scroll direction
- scrollbarVisible: Show/hide scrollbars
- smooth: Smooth scrolling

### TabView
Tab-based container.

```cpp
auto tabs = std::make_shared<TabView>();
tabs->AddTab("Tab 1", content1);
tabs->AddTab("Tab 2", content2);
```

Properties:
- tabs: Tab list
- activeTab: Current active tab
- tabPosition: Tab bar position
- closeable: Allow tab closing

## Advanced Widgets

### ListView
Virtualized list view for large datasets.

```cpp
auto list = std::make_shared<ListView>();
list->SetItemTemplate([](const auto& item) {
    return CreateItemWidget(item);
});
list->SetItems(items);
```

Properties:
- items: Data items
- template: Item template
- selection: Selection mode
- virtual: Virtualization enabled

### TreeView
Hierarchical data view.

```cpp
auto tree = std::make_shared<TreeView>();
tree->SetRoot(rootNode);
tree->OnNodeSelected([](TreeNode* node) {
    // Handle selection
});
```

Properties:
- root: Root node
- expanded: Expanded nodes
- selection: Selection mode
- icons: Node icons

### ProgressBar
Progress indicator widget.

```cpp
auto progress = std::make_shared<ProgressBar>();
progress->SetValue(0.75f);
progress->SetShowPercentage(true);
```

Properties:
- value: Current progress (0-1)
- showPercentage: Show percentage text
- color: Progress color
- animation: Progress animation

### Menu
Dropdown menu widget.

```cpp
auto menu = std::make_shared<Menu>();
menu->AddItem("Open", []() { /* Handle */ });
menu->AddSeparator();
menu->AddItem("Exit", []() { /* Handle */ });
```

Properties:
- items: Menu items
- shortcuts: Keyboard shortcuts
- icons: Item icons
- disabled: Disabled items

## Custom Widgets

Creating custom widgets:

```cpp
class CustomWidget : public Widget {
public:
    void Update(float deltaTime) override {
        // Update logic
    }
    
    void Render(Renderer* renderer) override {
        // Render logic
    }
    
    void ProcessEvent(const Event& event) override {
        // Event handling
    }
};
```

## Widget Properties

Common properties for all widgets:

```cpp
widget->SetPosition(x, y);
widget->SetSize(width, height);
widget->SetVisible(true);
widget->SetEnabled(true);
widget->SetOpacity(1.0f);
widget->SetTooltip("Helpful tip");
```

## Event Handling

Widget event handling:

```cpp
widget->OnMouseEnter([](const MouseEvent& e) {
    // Handle mouse enter
});

widget->OnMouseLeave([](const MouseEvent& e) {
    // Handle mouse leave
});

widget->OnClick([](const MouseEvent& e) {
    // Handle click
});

widget->OnKeyPress([](const KeyEvent& e) {
    // Handle key press
});
```

## Best Practices

1. Use appropriate widget for the task
2. Keep widget hierarchy shallow
3. Use layouts for positioning
4. Handle events at appropriate level
5. Clean up event handlers
6. Use widget pooling for large lists
7. Implement custom widgets when needed