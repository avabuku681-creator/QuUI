# Event System Reference

The QuUI Event System provides a robust and flexible way to handle user interactions and system events.

## Event Types

### Basic Events

```cpp
// Mouse Events
struct MouseEvent : public Event {
    enum class Type {
        Move,
        Enter,
        Leave,
        Down,
        Up,
        Click,
        DoubleClick,
        Wheel
    };
    
    Type type;
    Point position;
    MouseButton button;
    float wheelDelta;
    bool ctrl;
    bool shift;
    bool alt;
};

// Keyboard Events
struct KeyEvent : public Event {
    enum class Type {
        Press,
        Release,
        Repeat
    };
    
    Type type;
    KeyCode key;
    bool ctrl;
    bool shift;
    bool alt;
};

// Touch Events
struct TouchEvent : public Event {
    enum class Type {
        Begin,
        Move,
        End,
        Cancel
    };
    
    Type type;
    std::vector<TouchPoint> points;
};

// Gesture Events
struct GestureEvent : public Event {
    enum class Type {
        Tap,
        DoubleTap,
        LongPress,
        Swipe,
        Pinch,
        Rotate
    };
    
    Type type;
    Point position;
    Vector2 direction;
    float scale;
    float rotation;
};
```

## Event Handling

### Basic Event Handlers
```cpp
// Widget event handlers
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

### Global Event Handlers
```cpp
// Add global event handler
ui.AddEventHandler<MouseEvent>([](const MouseEvent& e) {
    // Handle any mouse event
});

ui.AddEventHandler<KeyEvent>([](const KeyEvent& e) {
    // Handle any key event
});
```

### Event Filtering
```cpp
// Filter specific events
widget->AddEventFilter([](const Event& e) -> bool {
    if (e.GetType() == Event::Type::MouseMove) {
        // Filter out mouse move events
        return true;
    }
    return false;
});
```

## Custom Events

### Creating Custom Events
```cpp
// Define custom event
class CustomEvent : public Event {
public:
    enum class Type {
        Custom1,
        Custom2
    };
    
    Type type;
    std::string data;
    
    Event::Type GetType() const override {
        return Event::Type::Custom;
    }
};

// Handle custom event
widget->AddEventHandler<CustomEvent>([](const CustomEvent& e) {
    // Handle custom event
});

// Dispatch custom event
CustomEvent event;
event.type = CustomEvent::Type::Custom1;
event.data = "Custom data";
ui.DispatchEvent(event);
```

## Event Propagation

### Event Bubbling
```cpp
// Event bubbling (bottom-up)
widget->OnEvent([](const Event& e) {
    // Handle event
    return true; // Continue bubbling
    // return false; // Stop bubbling
});
```

### Event Capturing
```cpp
// Event capturing (top-down)
widget->OnEventCapture([](const Event& e) {
    // Handle event during capture phase
    return true; // Continue capturing
    // return false; // Stop capturing
});
```

## Focus Management

### Focus Events
```cpp
widget->OnFocusGained([](const FocusEvent& e) {
    // Handle focus gained
});

widget->OnFocusLost([](const FocusEvent& e) {
    // Handle focus lost
});
```

### Focus Navigation
```cpp
// Set focus
widget->SetFocus();

// Clear focus
widget->ClearFocus();

// Check focus
if (widget->HasFocus()) {
    // Widget has focus
}

// Focus navigation
widget->SetNextFocusWidget(nextWidget);
widget->SetPreviousFocusWidget(prevWidget);
```

## Gesture Recognition

### Basic Gestures
```cpp
// Tap gesture
widget->OnTap([](const GestureEvent& e) {
    // Handle tap
});

// Swipe gesture
widget->OnSwipe([](const GestureEvent& e) {
    // Handle swipe
    auto direction = e.direction;
});

// Pinch gesture
widget->OnPinch([](const GestureEvent& e) {
    // Handle pinch
    auto scale = e.scale;
});

// Rotation gesture
widget->OnRotate([](const GestureEvent& e) {
    // Handle rotation
    auto angle = e.rotation;
});
```

### Custom Gestures
```cpp
// Define custom gesture
class CustomGesture : public GestureRecognizer {
public:
    bool RecognizeGesture(const TouchEvent& e) override {
        // Custom gesture recognition logic
        return true;
    }
};

// Add custom gesture recognizer
widget->AddGestureRecognizer(std::make_shared<CustomGesture>());
```

## Event Scheduling

### Delayed Events
```cpp
// Schedule event
ui.ScheduleEvent(event, std::chrono::milliseconds(100));

// Cancel scheduled event
ui.CancelScheduledEvent(eventId);
```

### Event Queue
```cpp
// Queue event
ui.QueueEvent(event);

// Process event queue
ui.ProcessEventQueue();
```

## Performance Optimization

### Event Throttling
```cpp
// Throttle mouse move events
widget->SetEventThrottling(Event::Type::MouseMove, 16); // 60fps

// Throttle custom events
widget->SetEventThrottling(customEventType, 100); // Every 100ms
```

### Event Batching
```cpp
// Begin event batch
ui.BeginEventBatch();

// Queue multiple events
ui.QueueEvent(event1);
ui.QueueEvent(event2);
ui.QueueEvent(event3);

// Process batch
ui.EndEventBatch();
```

## Best Practices

1. **Event Handling**
   - Keep handlers lightweight
   - Avoid blocking operations
   - Use appropriate event types
   - Clean up handlers when done

2. **Performance**
   - Use event throttling for frequent events
   - Batch related events
   - Remove unused handlers
   - Profile event handling

3. **Custom Events**
   - Define clear event types
   - Document event data
   - Use appropriate base classes
   - Handle cleanup properly

4. **Testing**
   - Test all event paths
   - Verify event ordering
   - Check memory usage
   - Test edge cases