# Example 1: Basic Calculator

This example demonstrates how to create a simple calculator using QuUI.

```cpp
#include "QuUI.h"

int main() {
    // Initialize SDK with custom configuration
    QuUI::UIConfig config;
    config.renderer.width = 300;
    config.renderer.height = 400;
    config.appearance.enableAnimations = true;
    
    if (!QUUI_INIT_WITH_CONFIG(config)) {
        return -1;
    }
    
    auto& ui = QUUI_INSTANCE();
    auto context = ui.CreateContext();
    
    // Create main container with grid layout
    auto container = std::make_shared<Container>();
    auto layout = std::make_shared<GridLayout>(4, 5);  // 4 columns, 5 rows
    layout->SetSpacing(5.0f);
    container->SetLayout(layout);
    
    // Create display
    auto display = std::make_shared<TextInput>();
    display->SetReadOnly(true);
    display->SetAlignment(Alignment::Right);
    
    // Create number buttons
    std::vector<std::shared_ptr<Button>> numberButtons;
    for (int i = 0; i < 10; i++) {
        auto button = std::make_shared<Button>();
        button->SetText(std::to_string(i));
        button->OnClick([display, i](const MouseEvent&) {
            display->SetText(display->GetText() + std::to_string(i));
        });
        numberButtons.push_back(button);
    }
    
    // Create operator buttons
    auto plusBtn = std::make_shared<Button>();
    plusBtn->SetText("+");
    
    auto minusBtn = std::make_shared<Button>();
    minusBtn->SetText("-");
    
    auto mulBtn = std::make_shared<Button>();
    mulBtn->SetText("*");
    
    auto divBtn = std::make_shared<Button>();
    divBtn->SetText("/");
    
    auto equalBtn = std::make_shared<Button>();
    equalBtn->SetText("=");
    
    auto clearBtn = std::make_shared<Button>();
    clearBtn->SetText("C");
    
    // Add display to first row (spans all columns)
    container->Add(display);
    
    // Add number buttons
    for (int i = 1; i <= 9; i++) {
        container->Add(numberButtons[i]);
    }
    
    // Add operator buttons
    container->Add(plusBtn);
    container->Add(numberButtons[0]);
    container->Add(equalBtn);
    container->Add(minusBtn);
    container->Add(mulBtn);
    container->Add(divBtn);
    container->Add(clearBtn);
    
    // Set up calculator logic
    float currentValue = 0.0f;
    char currentOperator = ' ';
    
    auto calculateResult = [&]() {
        float displayValue = std::stof(display->GetText());
        switch (currentOperator) {
            case '+': currentValue += displayValue; break;
            case '-': currentValue -= displayValue; break;
            case '*': currentValue *= displayValue; break;
            case '/': currentValue /= displayValue; break;
            default: currentValue = displayValue;
        }
        display->SetText(std::to_string(currentValue));
    };
    
    // Set up operator button handlers
    plusBtn->OnClick([&](const MouseEvent&) {
        calculateResult();
        currentOperator = '+';
        display->SetText("");
    });
    
    minusBtn->OnClick([&](const MouseEvent&) {
        calculateResult();
        currentOperator = '-';
        display->SetText("");
    });
    
    mulBtn->OnClick([&](const MouseEvent&) {
        calculateResult();
        currentOperator = '*';
        display->SetText("");
    });
    
    divBtn->OnClick([&](const MouseEvent&) {
        calculateResult();
        currentOperator = '/';
        display->SetText("");
    });
    
    equalBtn->OnClick([&](const MouseEvent&) {
        calculateResult();
        currentOperator = ' ';
    });
    
    clearBtn->OnClick([&](const MouseEvent&) {
        currentValue = 0.0f;
        currentOperator = ' ';
        display->SetText("");
    });
    
    // Main loop
    bool running = true;
    while (running) {
        float deltaTime = 1.0f / 60.0f;
        
        // Process events
        Event event;
        while (pollEvents(&event)) {
            if (event.GetType() == Event::Type::WindowClose) {
                running = false;
            }
            ui.ProcessEvent(event);
        }
        
        // Update and render
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
mkdir calculator
cd calculator
```

2. Create a `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.15)
project(QuUICalculator)

# Add QuUI
add_subdirectory(QuUI)

# Add calculator executable
add_executable(calculator main.cpp)
target_link_libraries(calculator PRIVATE QuUI)
```

3. Build the project:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

4. Run the calculator:
```bash
./calculator
```

## Features Demonstrated

1. **Layout System**
   - Grid layout for calculator buttons
   - Proper spacing and alignment

2. **Event Handling**
   - Button click events
   - Window close event

3. **Widgets**
   - Buttons
   - Text input (display)
   - Container with layout

4. **Theme Integration**
   - Default theme applied
   - Consistent look and feel

5. **Main Loop**
   - Event processing
   - Update and render cycle