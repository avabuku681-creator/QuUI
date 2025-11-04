# Animation Guide

QuUI's animation system provides powerful tools for creating smooth, performant animations in your UI.

## Basic Animations

### Property Animation
```cpp
widget->Animate()
    .Property("opacity")
    .From(0.0f)
    .To(1.0f)
    .Duration(0.3f)
    .Easing(Easing::EaseInOut)
    .Start();
```

### Multiple Properties
```cpp
widget->Animate()
    .Property("opacity", 0.0f, 1.0f)
    .Property("scale", 0.8f, 1.0f)
    .Duration(0.3f)
    .Start();
```

### Chained Animations
```cpp
widget->Animate()
    .Property("x", 0.0f, 100.0f)
    .Duration(0.3f)
    .Then()
    .Property("y", 0.0f, 100.0f)
    .Duration(0.3f)
    .Start();
```

## Animation Types

### Transform Animations
```cpp
// Translation
widget->AnimateTranslation(Vector2(100, 100))
    .Duration(0.3f)
    .Start();

// Rotation
widget->AnimateRotation(45.0f)
    .Duration(0.3f)
    .Start();

// Scale
widget->AnimateScale(Vector2(1.5f, 1.5f))
    .Duration(0.3f)
    .Start();
```

### Color Animations
```cpp
widget->AnimateColor(Color(1.0f, 0.0f, 0.0f))
    .Duration(0.3f)
    .Start();
```

### Layout Animations
```cpp
widget->AnimateSize(Size(200, 200))
    .Duration(0.3f)
    .Start();

widget->AnimatePosition(Point(100, 100))
    .Duration(0.3f)
    .Start();
```

## Easing Functions

```cpp
// Built-in easing functions
widget->Animate()
    .Property("opacity", 0.0f, 1.0f)
    .Easing(Easing::Linear)
    .Start();

// Available easing functions:
enum class Easing {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    EaseInQuad,
    EaseOutQuad,
    EaseInOutQuad,
    EaseInCubic,
    EaseOutCubic,
    EaseInOutCubic,
    EaseInElastic,
    EaseOutElastic,
    EaseInOutElastic,
    EaseInBounce,
    EaseOutBounce,
    EaseInOutBounce
};

// Custom easing function
widget->Animate()
    .Property("opacity", 0.0f, 1.0f)
    .CustomEasing([](float t) {
        return t * t; // Custom quadratic easing
    })
    .Start();
```

## Animation Groups

### Parallel Animations
```cpp
auto group = AnimationGroup::Parallel();
group->Add(widget1->Animate().Property("opacity", 0.0f, 1.0f));
group->Add(widget2->Animate().Property("scale", 0.8f, 1.0f));
group->Duration(0.3f)
    .Start();
```

### Sequential Animations
```cpp
auto group = AnimationGroup::Sequential();
group->Add(widget1->Animate().Property("opacity", 0.0f, 1.0f));
group->Add(widget2->Animate().Property("scale", 0.8f, 1.0f));
group->Start();
```

### Staggered Animations
```cpp
auto group = AnimationGroup::Staggered();
group->Add(widget1->Animate().Property("opacity", 0.0f, 1.0f));
group->Add(widget2->Animate().Property("opacity", 0.0f, 1.0f));
group->Add(widget3->Animate().Property("opacity", 0.0f, 1.0f));
group->Stagger(0.1f) // 0.1s delay between each animation
    .Start();
```

## Animation Controllers

```cpp
// Create controller
auto controller = std::make_shared<AnimationController>();

// Add animations
controller->Add("show", widget->Animate()
    .Property("opacity", 0.0f, 1.0f)
    .Duration(0.3f));

controller->Add("hide", widget->Animate()
    .Property("opacity", 1.0f, 0.0f)
    .Duration(0.3f));

// Play animations
controller->Play("show");
controller->Play("hide");

// Control playback
controller->Pause();
controller->Resume();
controller->Stop();
controller->Reverse();
```

## State Transitions

```cpp
// Define states
widget->DefineState("normal")
    .Opacity(1.0f)
    .Scale(1.0f);

widget->DefineState("hover")
    .Opacity(0.8f)
    .Scale(1.1f);

// Transition between states
widget->TransitionToState("hover")
    .Duration(0.2f)
    .Start();
```

## Performance Optimization

### GPU Acceleration
```cpp
// Enable GPU acceleration for specific properties
widget->SetGPUAccelerated(true);
widget->SetGPUAcceleratedProperties({
    "transform",
    "opacity"
});
```

### Animation Batching
```cpp
AnimationBatch batch;
batch.Add(widget1->Animate().Property("opacity", 0.0f, 1.0f));
batch.Add(widget2->Animate().Property("scale", 0.8f, 1.0f));
batch.Commit(); // Animations are batched for better performance
```

### Frame Rate Control
```cpp
// Set target frame rate for animations
Animation::SetTargetFrameRate(60);

// Enable adaptive frame rate
Animation::SetAdaptiveFrameRate(true);
```

## Event Handling

```cpp
widget->Animate()
    .Property("opacity", 0.0f, 1.0f)
    .OnStart([]() {
        // Animation started
    })
    .OnUpdate([](float progress) {
        // Animation progress
    })
    .OnComplete([]() {
        // Animation completed
    })
    .Start();
```

## Best Practices

1. **Performance**
   - Use GPU-accelerated properties when possible
   - Batch animations
   - Limit number of concurrent animations
   - Use appropriate frame rate

2. **User Experience**
   - Keep animations short (0.2s - 0.4s)
   - Use appropriate easing functions
   - Provide visual feedback
   - Consider reduced motion preferences

3. **Code Organization**
   - Use animation controllers for complex sequences
   - Group related animations
   - Reuse animation definitions
   - Document animation timing and easing

4. **Testing**
   - Test on different devices
   - Verify performance
   - Check edge cases
   - Ensure animations complete properly