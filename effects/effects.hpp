#pragma once
#include "../core/math_types.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace gui {

// Базовый класс для эффектов
class Effect {
public:
    virtual ~Effect() = default;
    virtual void apply(const Rect& bounds) = 0;
    virtual void update(float deltaTime) = 0;
    
    void setEnabled(bool enabled) { enabled_ = enabled; }
    bool isEnabled() const { return enabled_; }

protected:
    bool enabled_ = true;
};

// Эффект тени
class ShadowEffect : public Effect {
public:
    ShadowEffect(const Vector2f& offset = Vector2f(2.0f, 2.0f), float blur = 4.0f,
                 const Color& color = Color(0, 0, 0, 0.25f));
    
    void apply(const Rect& bounds) override;
    void update(float deltaTime) override;
    
    void setOffset(const Vector2f& offset) { offset_ = offset; }
    void setBlur(float blur) { blur_ = blur; }
    void setColor(const Color& color) { color_ = color; }

private:
    Vector2f offset_;
    float blur_;
    Color color_;
};

// Эффект размытия
class BlurEffect : public Effect {
public:
    explicit BlurEffect(float radius = 5.0f);
    
    void apply(const Rect& bounds) override;
    void update(float deltaTime) override;
    
    void setRadius(float radius) { radius_ = radius; }
    void setQuality(int quality) { quality_ = quality; }

private:
    float radius_;
    int quality_ = 3;
};

// Эффект свечения
class GlowEffect : public Effect {
public:
    GlowEffect(float intensity = 1.0f, const Color& color = Color(1, 1, 1, 0.5f));
    
    void apply(const Rect& bounds) override;
    void update(float deltaTime) override;
    
    void setIntensity(float intensity) { intensity_ = intensity; }
    void setColor(const Color& color) { color_ = color; }
    void setPulse(bool enabled, float speed = 1.0f);

private:
    float intensity_;
    Color color_;
    bool pulse_ = false;
    float pulseSpeed_ = 1.0f;
    float pulseTime_ = 0.0f;
};

// Эффект отражения
class ReflectionEffect : public Effect {
public:
    ReflectionEffect(float intensity = 0.5f, float fadeDistance = 0.5f);
    
    void apply(const Rect& bounds) override;
    void update(float deltaTime) override;
    
    void setIntensity(float intensity) { intensity_ = intensity; }
    void setFadeDistance(float distance) { fadeDistance_ = distance; }
    void setDistortion(float distortion) { distortion_ = distortion; }

private:
    float intensity_;
    float fadeDistance_;
    float distortion_ = 0.1f;
};

// Эффект частиц
class ParticleEffect : public Effect {
public:
    struct Particle {
        Vector2f position;
        Vector2f velocity;
        Color color;
        float life;
        float size;
    };

    ParticleEffect(int maxParticles = 100);
    
    void apply(const Rect& bounds) override;
    void update(float deltaTime) override;
    
    void setEmissionRate(float rate) { emissionRate_ = rate; }
    void setParticleLife(float min, float max);
    void setParticleSize(float min, float max);
    void setParticleSpeed(float min, float max);
    void setParticleColor(const Color& start, const Color& end);

private:
    void emitParticle(const Vector2f& position);
    
    std::vector<Particle> particles_;
    float emissionRate_;
    float emissionAccumulator_ = 0.0f;
    float particleLifeMin_ = 1.0f;
    float particleLifeMax_ = 2.0f;
    float particleSizeMin_ = 1.0f;
    float particleSizeMax_ = 3.0f;
    float particleSpeedMin_ = 50.0f;
    float particleSpeedMax_ = 100.0f;
    Color particleColorStart_ = Color(1, 1, 1, 1);
    Color particleColorEnd_ = Color(1, 1, 1, 0);
};

// Менеджер эффектов
class EffectManager {
public:
    static EffectManager& getInstance() {
        static EffectManager instance;
        return instance;
    }

    void addEffect(const std::string& name, std::shared_ptr<Effect> effect);
    void removeEffect(const std::string& name);
    Effect* getEffect(const std::string& name);
    
    void updateEffects(float deltaTime);
    void applyEffects(const Rect& bounds);

private:
    EffectManager() = default;
    std::unordered_map<std::string, std::shared_ptr<Effect>> effects_;
};

} // namespace gui