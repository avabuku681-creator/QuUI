# Example 2: Todo List Application

This example demonstrates a more complex application with data management, custom widgets, and animations.

```cpp
#include "QuUI.h"

// Todo item data structure
struct TodoItem {
    std::string text;
    bool completed;
    std::chrono::system_clock::time_point created;
};

// Custom todo item widget
class TodoItemWidget : public Widget {
public:
    TodoItemWidget(const TodoItem& item) : item_(item) {
        // Create layout
        auto layout = std::make_shared<FlowLayout>();
        layout->SetDirection(FlowLayout::Horizontal);
        layout->SetSpacing(10.0f);
        layout->SetAlignment(Alignment::CenterVertical);
        SetLayout(layout);
        
        // Create checkbox
        checkbox_ = std::make_shared<CheckBox>();
        checkbox_->SetChecked(item.completed);
        checkbox_->OnToggle([this](bool checked) {
            item_.completed = checked;
            if (onStatusChanged_) {
                onStatusChanged_(checked);
            }
        });
        Add(checkbox_);
        
        // Create text label
        text_ = std::make_shared<Label>();
        text_->SetText(item.text);
        Add(text_);
        
        // Create delete button
        deleteBtn_ = std::make_shared<Button>();
        deleteBtn_->SetText("×");
        deleteBtn_->OnClick([this](const MouseEvent&) {
            if (onDelete_) {
                onDelete_();
            }
        });
        Add(deleteBtn_);
        
        // Set up animations
        setupAnimations();
    }
    
    void SetOnDelete(std::function<void()> callback) {
        onDelete_ = callback;
    }
    
    void SetOnStatusChanged(std::function<void(bool)> callback) {
        onStatusChanged_ = callback;
    }
    
    const TodoItem& GetItem() const { return item_; }
    
private:
    void setupAnimations() {
        // Fade in animation
        Animate()
            .Property("opacity", 0.0f, 1.0f)
            .Duration(0.3f)
            .Start();
            
        // Hover animation
        OnMouseEnter([this](const MouseEvent&) {
            Animate()
                .Property("scale", 1.0f, 1.05f)
                .Duration(0.2f)
                .Start();
        });
        
        OnMouseLeave([this](const MouseEvent&) {
            Animate()
                .Property("scale", 1.05f, 1.0f)
                .Duration(0.2f)
                .Start();
        });
    }
    
    TodoItem item_;
    std::shared_ptr<CheckBox> checkbox_;
    std::shared_ptr<Label> text_;
    std::shared_ptr<Button> deleteBtn_;
    std::function<void()> onDelete_;
    std::function<void(bool)> onStatusChanged_;
};

int main() {
    // Initialize SDK
    QuUI::UIConfig config;
    config.renderer.width = 400;
    config.renderer.height = 600;
    config.appearance.enableAnimations = true;
    
    if (!QUUI_INIT_WITH_CONFIG(config)) {
        return -1;
    }
    
    auto& ui = QUUI_INSTANCE();
    auto context = ui.CreateContext();
    
    // Create main container with flow layout
    auto container = std::make_shared<Container>();
    auto mainLayout = std::make_shared<FlowLayout>();
    mainLayout->SetDirection(FlowLayout::Vertical);
    mainLayout->SetSpacing(20.0f);
    mainLayout->SetPadding(20.0f);
    container->SetLayout(mainLayout);
    
    // Create header
    auto header = std::make_shared<Label>();
    header->SetText("Todo List");
    header->SetFont("Arial", 24.0f);
    container->Add(header);
    
    // Create input container
    auto inputContainer = std::make_shared<Container>();
    auto inputLayout = std::make_shared<FlowLayout>();
    inputLayout->SetDirection(FlowLayout::Horizontal);
    inputLayout->SetSpacing(10.0f);
    inputContainer->SetLayout(inputLayout);
    
    auto input = std::make_shared<TextInput>();
    input->SetPlaceholder("Add new todo...");
    inputContainer->Add(input);
    
    auto addButton = std::make_shared<Button>();
    addButton->SetText("Add");
    inputContainer->Add(addButton);
    
    container->Add(inputContainer);
    
    // Create todo list container
    auto listContainer = std::make_shared<Container>();
    auto listLayout = std::make_shared<FlowLayout>();
    listLayout->SetDirection(FlowLayout::Vertical);
    listLayout->SetSpacing(10.0f);
    listContainer->SetLayout(listLayout);
    
    // Create scroll view for list
    auto scrollView = std::make_shared<ScrollView>();
    scrollView->SetContent(listContainer);
    scrollView->SetSize(400, 400);
    container->Add(scrollView);
    
    // Create status bar
    auto statusBar = std::make_shared<Label>();
    statusBar->SetText("0 items");
    container->Add(statusBar);
    
    // Todo list management
    std::vector<TodoItem> todos;
    
    auto updateStatusBar = [&]() {
        int total = todos.size();
        int completed = std::count_if(todos.begin(), todos.end(),
            [](const TodoItem& item) { return item.completed; });
        statusBar->SetText(
            std::to_string(completed) + "/" +
            std::to_string(total) + " completed"
        );
    };
    
    auto addTodo = [&](const std::string& text) {
        if (text.empty()) return;
        
        TodoItem item{text, false, std::chrono::system_clock::now()};
        todos.push_back(item);
        
        auto widget = std::make_shared<TodoItemWidget>(item);
        widget->SetOnDelete([widget, &todos, &listContainer, updateStatusBar]() {
            // Find and remove todo item
            auto it = std::find_if(todos.begin(), todos.end(),
                [&](const TodoItem& item) {
                    return item.text == widget->GetItem().text;
                });
            if (it != todos.end()) {
                todos.erase(it);
            }
            
            // Animate removal
            widget->Animate()
                .Property("opacity", 1.0f, 0.0f)
                .Duration(0.3f)
                .OnComplete([widget, &listContainer]() {
                    listContainer->Remove(widget);
                })
                .Start();
            
            updateStatusBar();
        });
        
        widget->SetOnStatusChanged([updateStatusBar](bool) {
            updateStatusBar();
        });
        
        listContainer->Add(widget);
        updateStatusBar();
    };
    
    // Set up input handling
    addButton->OnClick([&](const MouseEvent&) {
        addTodo(input->GetText());
        input->SetText("");
    });
    
    input->OnKeyPress([&](const KeyEvent& e) {
        if (e.key == KeyCode::Enter) {
            addTodo(input->GetText());
            input->SetText("");
        }
    });
    
    // Main loop
    bool running = true;
    while (running) {
        float deltaTime = 1.0f / 60.0f;
        
        Event event;
        while (pollEvents(&event)) {
            if (event.GetType() == Event::Type::WindowClose) {
                running = false;
            }
            ui.ProcessEvent(event);
        }
        
        ui.Update(deltaTime);
        ui.Render();
    }
    
    QUUI_SHUTDOWN();
    return 0;
}
```

## Building and Running

1. Create a new directory for the project:
```bash
mkdir todo_list
cd todo_list
```

2. Create a `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.15)
project(QuUITodoList)

# Add QuUI
add_subdirectory(QuUI)

# Add todo list executable
add_executable(todo_list main.cpp)
target_link_libraries(todo_list PRIVATE QuUI)
```

3. Build the project:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

4. Run the application:
```bash
./todo_list
```

## Features Demonstrated

1. **Custom Widgets**
   - TodoItemWidget implementation
   - Complex widget composition

2. **Layouts**
   - Flow layouts (vertical and horizontal)
   - Proper spacing and padding
   - ScrollView for large lists

3. **Event Handling**
   - Button clicks
   - Keyboard input
   - Custom callbacks

4. **Animations**
   - Fade in/out
   - Hover effects
   - Smooth transitions

5. **Data Management**
   - Todo item structure
   - List management
   - Status tracking

6. **User Input**
   - Text input with placeholder
   - Enter key handling
   - Button interaction

7. **Styling**
   - Custom fonts
   - Proper alignment
   - Visual feedback

This example demonstrates more advanced features of QuUI, including:
- Custom widget creation
- Complex layouts
- Animation system
- Event handling
- Data management
- User input processing
- Visual styling