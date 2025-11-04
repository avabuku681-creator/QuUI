# Text Editor Example

This example demonstrates a more complex application with text editing capabilities, syntax highlighting, and file operations.

```cpp
#include "QuUI.h"
#include <fstream>
#include <sstream>

// Custom text editor widget
class TextEditorWidget : public Widget {
public:
    TextEditorWidget() {
        setupEditor();
        setupSyntaxHighlighting();
        setupKeyboardHandling();
    }

private:
    void setupEditor() {
        // Create text buffer
        textBuffer_ = std::make_shared<TextBuffer>();
        textBuffer_->OnChange([this](const TextBuffer::ChangeEvent& e) {
            // Handle text changes
            updateSyntaxHighlighting(e.startLine, e.endLine);
            if (onTextChanged_) {
                onTextChanged_(GetText());
            }
        });

        // Create line numbers
        lineNumbers_ = std::make_shared<LineNumbersWidget>();
        lineNumbers_->SetWidth(50.0f);
        Add(lineNumbers_);

        // Create scroll bars
        vscrollBar_ = std::make_shared<ScrollBar>(ScrollBar::Vertical);
        hscrollBar_ = std::make_shared<ScrollBar>(ScrollBar::Horizontal);
        Add(vscrollBar_);
        Add(hscrollBar_);

        // Create cursor
        cursor_ = std::make_shared<TextCursor>();
        Add(cursor_);

        // Setup selection handling
        SetupSelection();
    }

    void setupSyntaxHighlighting() {
        // Create syntax highlighter
        syntaxHighlighter_ = std::make_shared<SyntaxHighlighter>();

        // Add syntax rules
        syntaxHighlighter_->AddRule("keyword", 
            {"int", "float", "if", "else", "for", "while", "return"},
            Color(0.2f, 0.6f, 1.0f));

        syntaxHighlighter_->AddRule("string",
            std::regex("\"[^\"]*\""),
            Color(0.9f, 0.6f, 0.2f));

        syntaxHighlighter_->AddRule("number",
            std::regex("\\b\\d+(\\.\\d+)?\\b"),
            Color(0.4f, 0.8f, 0.4f));

        syntaxHighlighter_->AddRule("comment",
            std::regex("//.*$"),
            Color(0.5f, 0.5f, 0.5f));
    }

    void setupKeyboardHandling() {
        OnKeyPress([this](const KeyEvent& e) {
            if (e.ctrl) {
                switch (e.key) {
                    case KeyCode::C: copySelection(); break;
                    case KeyCode::V: paste(); break;
                    case KeyCode::X: cut(); break;
                    case KeyCode::Z: undo(); break;
                    case KeyCode::Y: redo(); break;
                    case KeyCode::F: find(); break;
                    case KeyCode::S: save(); break;
                }
            } else {
                handleRegularKeyPress(e);
            }
        });
    }

    void SetupSelection() {
        OnMouseDown([this](const MouseEvent& e) {
            selectionStart_ = getPositionFromPoint(e.position);
            selectionEnd_ = selectionStart_;
            selecting_ = true;
        });

        OnMouseMove([this](const MouseEvent& e) {
            if (selecting_) {
                selectionEnd_ = getPositionFromPoint(e.position);
                updateSelection();
            }
        });

        OnMouseUp([this](const MouseEvent& e) {
            selecting_ = false;
        });
    }

    TextPosition getPositionFromPoint(const Point& point) {
        // Convert screen coordinates to text position
        int line = (point.y - GetPadding().top) / lineHeight_;
        int column = (point.x - GetPadding().left) / charWidth_;
        return {line, column};
    }

    void updateSelection() {
        // Update visual selection
        if (selectionStart_ != selectionEnd_) {
            auto start = std::min(selectionStart_, selectionEnd_);
            auto end = std::max(selectionStart_, selectionEnd_);
            
            selection_->SetRange(start, end);
            selection_->SetVisible(true);
        } else {
            selection_->SetVisible(false);
        }
    }

    void updateSyntaxHighlighting(int startLine, int endLine) {
        for (int line = startLine; line <= endLine; ++line) {
            auto text = textBuffer_->GetLine(line);
            auto highlights = syntaxHighlighter_->Highlight(text);
            
            for (const auto& highlight : highlights) {
                textBuffer_->SetStyle(
                    line,
                    highlight.start,
                    highlight.end,
                    TextStyle().TextColor(highlight.color)
                );
            }
        }
    }

    // Edit operations
    void copySelection() {
        if (selectionStart_ != selectionEnd_) {
            auto text = textBuffer_->GetText(selectionStart_, selectionEnd_);
            ui.GetClipboard()->SetText(text);
        }
    }

    void paste() {
        auto text = ui.GetClipboard()->GetText();
        if (!text.empty()) {
            if (selectionStart_ != selectionEnd_) {
                textBuffer_->Replace(selectionStart_, selectionEnd_, text);
            } else {
                textBuffer_->Insert(cursor_->GetPosition(), text);
            }
        }
    }

    void cut() {
        copySelection();
        if (selectionStart_ != selectionEnd_) {
            textBuffer_->Delete(selectionStart_, selectionEnd_);
        }
    }

    void undo() {
        textBuffer_->Undo();
    }

    void redo() {
        textBuffer_->Redo();
    }

    void find() {
        // Show find dialog
        auto dialog = std::make_shared<FindDialog>();
        dialog->OnFind([this](const std::string& text) {
            auto pos = textBuffer_->Find(text, cursor_->GetPosition());
            if (pos.valid) {
                cursor_->SetPosition(pos);
                selectionStart_ = pos;
                selectionEnd_ = {pos.line, pos.column + text.length()};
                updateSelection();
                scrollToCursor();
            }
        });
        GetParent()->Add(dialog);
    }

    void save() {
        if (filePath_.empty()) {
            auto dialog = std::make_shared<SaveDialog>();
            dialog->OnSave([this](const std::string& path) {
                filePath_ = path;
                saveToFile();
            });
            GetParent()->Add(dialog);
        } else {
            saveToFile();
        }
    }

    void saveToFile() {
        std::ofstream file(filePath_);
        if (file.is_open()) {
            file << textBuffer_->GetText();
            file.close();
        }
    }

    std::shared_ptr<TextBuffer> textBuffer_;
    std::shared_ptr<LineNumbersWidget> lineNumbers_;
    std::shared_ptr<ScrollBar> vscrollBar_;
    std::shared_ptr<ScrollBar> hscrollBar_;
    std::shared_ptr<TextCursor> cursor_;
    std::shared_ptr<Selection> selection_;
    std::shared_ptr<SyntaxHighlighter> syntaxHighlighter_;

    float lineHeight_ = 20.0f;
    float charWidth_ = 8.0f;
    bool selecting_ = false;
    TextPosition selectionStart_;
    TextPosition selectionEnd_;
    std::string filePath_;
    std::function<void(const std::string&)> onTextChanged_;
};

int main() {
    // Initialize SDK
    QuUI::UIConfig config;
    config.renderer.width = 800;
    config.renderer.height = 600;
    config.appearance.enableAnimations = true;

    if (!QUUI_INIT_WITH_CONFIG(config)) {
        return -1;
    }

    auto& ui = QUUI_INSTANCE();
    auto context = ui.CreateContext();

    // Create main layout
    auto mainLayout = std::make_shared<Container>();
    auto layout = std::make_shared<FlowLayout>();
    layout->SetDirection(FlowLayout::Vertical);
    mainLayout->SetLayout(layout);

    // Create menu bar
    auto menuBar = std::make_shared<MenuBar>();
    auto fileMenu = menuBar->AddMenu("File");
    fileMenu->AddItem("New", []() { /* Handle new */ });
    fileMenu->AddItem("Open", []() { /* Handle open */ });
    fileMenu->AddItem("Save", []() { /* Handle save */ });
    fileMenu->AddSeparator();
    fileMenu->AddItem("Exit", []() { /* Handle exit */ });

    auto editMenu = menuBar->AddMenu("Edit");
    editMenu->AddItem("Undo", []() { /* Handle undo */ });
    editMenu->AddItem("Redo", []() { /* Handle redo */ });
    editMenu->AddSeparator();
    editMenu->AddItem("Cut", []() { /* Handle cut */ });
    editMenu->AddItem("Copy", []() { /* Handle copy */ });
    editMenu->AddItem("Paste", []() { /* Handle paste */ });

    mainLayout->Add(menuBar);

    // Create toolbar
    auto toolbar = std::make_shared<Toolbar>();
    toolbar->AddButton("New", "new.png", []() { /* Handle new */ });
    toolbar->AddButton("Open", "open.png", []() { /* Handle open */ });
    toolbar->AddButton("Save", "save.png", []() { /* Handle save */ });
    toolbar->AddSeparator();
    toolbar->AddButton("Undo", "undo.png", []() { /* Handle undo */ });
    toolbar->AddButton("Redo", "redo.png", []() { /* Handle redo */ });
    mainLayout->Add(toolbar);

    // Create editor
    auto editor = std::make_shared<TextEditorWidget>();
    mainLayout->Add(editor);

    // Create status bar
    auto statusBar = std::make_shared<StatusBar>();
    auto lineColumn = statusBar->AddField("Line: 1, Col: 1");
    auto fileType = statusBar->AddField("Plain Text");
    auto encoding = statusBar->AddField("UTF-8");
    mainLayout->Add(statusBar);

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
mkdir text_editor
cd text_editor
```

2. Create a `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.15)
project(QuUITextEditor)

# Add QuUI
add_subdirectory(QuUI)

# Add text editor executable
add_executable(text_editor main.cpp)
target_link_libraries(text_editor PRIVATE QuUI)
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
./text_editor
```

## Features Demonstrated

1. **Complex Widget Creation**
   - Custom text editor widget
   - Multiple child widgets (line numbers, scrollbars, cursor)
   - Event handling

2. **Text Handling**
   - Text buffer management
   - Selection handling
   - Clipboard operations
   - Syntax highlighting

3. **UI Components**
   - Menu bar
   - Toolbar
   - Status bar
   - Dialog boxes

4. **File Operations**
   - File loading
   - File saving
   - New file creation

5. **Advanced Features**
   - Syntax highlighting
   - Find/Replace
   - Undo/Redo
   - Line numbers
   - Scroll bars

This example shows how to create a more complex application using QuUI, demonstrating:
- Custom widget creation
- Text handling
- File operations
- UI composition
- Event handling
- Visual styling