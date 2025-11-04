#pragma once
#include <functional>
#include <vector>
#include "../core/math_types.hpp"
#include "../core/widget_base.hpp"
#include <memory>

namespace gui {

class Animation {
public:
    enum class EaseType {
        Linear,
        EaseIn,
        EaseOut,
        EaseInOut,
        Bounce,
        Elastic
    };

    Animation(float duration, EaseType easeType = EaseType::Linear);
    virtual ~Animation() = default;

    void start();
    void pause();
    void resume();
    void stop();
    void reset();

    bool isPlaying() const;
    bool isFinished() const;
    float getProgress() const;

    void update(float deltaTime);
    void setCompletionCallback(std::function<void()> callback);

protected:
    virtual void updateValue(float progress) = 0;
    float ease(float t) const;

private:
    float duration_;
    float currentTime_;
    EaseType easeType_;
    bool playing_;
    bool finished_;
    std::function<void()> completionCallback_;
};

class PositionAnimation : public Animation {
public:
    PositionAnimation(Widget* target, const Vector2f& startPos, const Vector2f& endPos, float duration);

protected:
    void updateValue(float progress) override;

private:
    Widget* target_;
    Vector2f startPos_;
    Vector2f endPos_;
};

class ScaleAnimation : public Animation {
public:
    ScaleAnimation(Widget* target, const Vector2f& startScale, const Vector2f& endScale, float duration);

protected:
    void updateValue(float progress) override;

private:
    Widget* target_;
    Vector2f startScale_;
    Vector2f endScale_;
};

class ColorAnimation : public Animation {
public:
    ColorAnimation(Widget* target, const Color& startColor, const Color& endColor, float duration);

protected:
    void updateValue(float progress) override;

private:
    Widget* target_;
    Color startColor_;
    Color endColor_;
};

class AnimationGroup {
public:
    enum class PlayMode {
        Sequential,
        Parallel
    };

    AnimationGroup(PlayMode mode = PlayMode::Sequential);
    
    void addAnimation(std::shared_ptr<Animation> animation);
    void removeAnimation(std::shared_ptr<Animation> animation);
    void clear();

    void start();
    void pause();
    void resume();
    void stop();
    void reset();

    bool isPlaying() const;
    bool isFinished() const;
    
    void update(float deltaTime);
    void setCompletionCallback(std::function<void()> callback);

private:
    PlayMode mode_;
    std::vector<std::shared_ptr<Animation>> animations_;
    size_t currentAnimation_;
    bool playing_;
    bool finished_;
    std::function<void()> completionCallback_;
};

} // namespace gui