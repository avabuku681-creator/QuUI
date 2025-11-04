#pragma once
#include <vector>
#include <functional>
#include <memory>
#include "../core/math_types.hpp"

namespace gui {

// Базовый класс для жестов
class GestureRecognizer {
public:
    virtual ~GestureRecognizer() = default;

    // Основные методы распознавания
    virtual void reset() = 0;
    virtual void update(const std::vector<TouchPoint>& points, float deltaTime) = 0;
    virtual bool isRecognized() const = 0;
    virtual float getProgress() const = 0;

    // События жестов
    std::function<void()> onGestureBegin;
    std::function<void()> onGestureUpdate;
    std::function<void()> onGestureEnd;
    std::function<void()> onGestureCancel;

protected:
    bool recognized_ = false;
    float progress_ = 0.0f;
};

// Точка касания
struct TouchPoint {
    int id;
    Vector2f position;
    Vector2f previousPosition;
    float pressure;
    float time;
};

// Распознаватель тапа
class TapGestureRecognizer : public GestureRecognizer {
public:
    TapGestureRecognizer(int requiredTaps = 1, float maxDelay = 0.3f);
    
    void reset() override;
    void update(const std::vector<TouchPoint>& points, float deltaTime) override;
    
    void setRequiredTaps(int count);
    void setMaxDelay(float delay);

private:
    int requiredTaps_;
    float maxDelay_;
    int currentTaps_ = 0;
    float timeSinceLastTap_ = 0;
};

// Распознаватель долгого нажатия
class LongPressGestureRecognizer : public GestureRecognizer {
public:
    LongPressGestureRecognizer(float duration = 0.5f, float maxMovement = 10.0f);
    
    void reset() override;
    void update(const std::vector<TouchPoint>& points, float deltaTime) override;
    
    void setDuration(float duration);
    void setMaxMovement(float maxMovement);

private:
    float duration_;
    float maxMovement_;
    float currentTime_ = 0;
    Vector2f initialPosition_;
};

// Распознаватель перетаскивания
class PanGestureRecognizer : public GestureRecognizer {
public:
    PanGestureRecognizer(float minDistance = 10.0f);
    
    void reset() override;
    void update(const std::vector<TouchPoint>& points, float deltaTime) override;
    
    Vector2f getTranslation() const;
    Vector2f getVelocity() const;

private:
    float minDistance_;
    Vector2f initialPosition_;
    Vector2f currentTranslation_;
    Vector2f velocity_;
};

// Распознаватель масштабирования
class PinchGestureRecognizer : public GestureRecognizer {
public:
    void reset() override;
    void update(const std::vector<TouchPoint>& points, float deltaTime) override;
    
    float getScale() const;
    float getRotation() const;

private:
    float initialDistance_ = 0;
    float currentScale_ = 1.0f;
    float currentRotation_ = 0;
};

// Распознаватель свайпа
class SwipeGestureRecognizer : public GestureRecognizer {
public:
    enum class Direction {
        Left,
        Right,
        Up,
        Down
    };

    SwipeGestureRecognizer(Direction direction, float minVelocity = 500.0f, float maxTime = 0.3f);
    
    void reset() override;
    void update(const std::vector<TouchPoint>& points, float deltaTime) override;
    
    Direction getDirection() const;
    float getVelocity() const;

private:
    Direction direction_;
    float minVelocity_;
    float maxTime_;
    Vector2f initialPosition_;
    float currentTime_ = 0;
};

// Менеджер жестов
class GestureManager {
public:
    void addRecognizer(std::shared_ptr<GestureRecognizer> recognizer);
    void removeRecognizer(std::shared_ptr<GestureRecognizer> recognizer);
    void update(const std::vector<TouchPoint>& points, float deltaTime);
    void reset();

private:
    std::vector<std::shared_ptr<GestureRecognizer>> recognizers_;
};

} // namespace gui