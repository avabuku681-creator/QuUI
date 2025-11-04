#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "../core/math_types.hpp"

namespace gui {

// Функции плавности
class EasingFunction {
public:
    virtual ~EasingFunction() = default;
    virtual float calculate(float t) const = 0;
};

// Стандартные функции плавности
namespace Easing {
    class Linear : public EasingFunction {
        float calculate(float t) const override { return t; }
    };

    class QuadraticEaseIn : public EasingFunction {
        float calculate(float t) const override { return t * t; }
    };

    class QuadraticEaseOut : public EasingFunction {
        float calculate(float t) const override { return -(t * (t - 2)); }
    };

    class CubicEaseInOut : public EasingFunction {
        float calculate(float t) const override {
            t *= 2;
            if (t < 1) return 0.5f * t * t * t;
            t -= 2;
            return 0.5f * (t * t * t + 2);
        }
    };

    class ElasticEaseOut : public EasingFunction {
        float calculate(float t) const override {
            if (t == 0 || t == 1) return t;
            return std::pow(2, -10 * t) * std::sin((t * 10 - 0.75f) * 2 * M_PI / 3) + 1;
        }
    };

    class BounceEaseOut : public EasingFunction {
        float calculate(float t) const override {
            if (t < 1 / 2.75f) {
                return 7.5625f * t * t;
            } else if (t < 2 / 2.75f) {
                t -= 1.5f / 2.75f;
                return 7.5625f * t * t + 0.75f;
            } else if (t < 2.5f / 2.75f) {
                t -= 2.25f / 2.75f;
                return 7.5625f * t * t + 0.9375f;
            } else {
                t -= 2.625f / 2.75f;
                return 7.5625f * t * t + 0.984375f;
            }
        }
    };
}

// Базовый класс для анимируемых значений
template<typename T>
class Animatable {
public:
    T value;
    T startValue;
    T endValue;
    
    virtual void interpolate(float progress) = 0;
};

// Специализации для разных типов
template<>
class Animatable<float> {
public:
    float value;
    float startValue;
    float endValue;

    void interpolate(float progress) {
        value = startValue + (endValue - startValue) * progress;
    }
};

template<>
class Animatable<Vector2f> {
public:
    Vector2f value;
    Vector2f startValue;
    Vector2f endValue;

    void interpolate(float progress) {
        value = Vector2f::lerp(startValue, endValue, progress);
    }
};

template<>
class Animatable<Color> {
public:
    Color value;
    Color startValue;
    Color endValue;

    void interpolate(float progress) {
        value = startValue.lerp(endValue, progress);
    }
};

// Улучшенный класс анимации
class Animation {
public:
    Animation(float duration, std::shared_ptr<EasingFunction> easing = std::make_shared<Easing::Linear>());
    virtual ~Animation() = default;

    // Управление анимацией
    void start();
    void pause();
    void resume();
    void stop();
    void reset();
    void reverse();

    // Состояние анимации
    bool isPlaying() const;
    bool isFinished() const;
    float getProgress() const;
    float getDuration() const;

    // Настройки анимации
    void setDelay(float delay);
    void setRepeatCount(int count);
    void setRepeatMode(bool alternate);
    void setSpeed(float speed);

    // События анимации
    void setOnStart(std::function<void()> callback);
    void setOnUpdate(std::function<void(float)> callback);
    void setOnComplete(std::function<void()> callback);

    // Обновление
    void update(float deltaTime);

protected:
    float duration_;
    float currentTime_;
    float delay_;
    float speed_;
    int repeatCount_;
    bool alternate_;
    bool playing_;
    bool finished_;
    bool reversed_;
    
    std::shared_ptr<EasingFunction> easingFunction_;
    std::function<void()> onStart_;
    std::function<void(float)> onUpdate_;
    std::function<void()> onComplete_;

    virtual void updateAnimation(float progress);
};

// Составная анимация
class CompositeAnimation : public Animation {
public:
    void addAnimation(std::shared_ptr<Animation> animation);
    void removeAnimation(std::shared_ptr<Animation> animation);

protected:
    void updateAnimation(float progress) override;

private:
    std::vector<std::shared_ptr<Animation>> animations_;
};

// Последовательная анимация
class SequentialAnimation : public Animation {
public:
    void addAnimation(std::shared_ptr<Animation> animation);
    void removeAnimation(std::shared_ptr<Animation> animation);

protected:
    void updateAnimation(float progress) override;

private:
    std::vector<std::shared_ptr<Animation>> animations_;
    size_t currentAnimation_;
};

// Менеджер анимаций
class AnimationManager {
public:
    static AnimationManager& getInstance() {
        static AnimationManager instance;
        return instance;
    }

    void addAnimation(std::shared_ptr<Animation> animation);
    void removeAnimation(std::shared_ptr<Animation> animation);
    void update(float deltaTime);
    void pauseAll();
    void resumeAll();
    void stopAll();

private:
    AnimationManager() = default;
    std::vector<std::shared_ptr<Animation>> animations_;
};

} // namespace gui