#pragma once
#include "../core/math_types.hpp"
#include <memory>
#include <vector>

namespace gui {

// Физические свойства
struct PhysicsProperties {
    float mass = 1.0f;
    float friction = 0.5f;
    float restitution = 0.5f;
    float linearDamping = 0.01f;
    float angularDamping = 0.01f;
    bool fixedRotation = false;
    bool isSensor = false;
};

// Физическое тело
class PhysicsBody {
public:
    PhysicsBody(const PhysicsProperties& props = PhysicsProperties());
    virtual ~PhysicsBody() = default;

    void setPosition(const Vector2f& position);
    void setRotation(float rotation);
    void setLinearVelocity(const Vector2f& velocity);
    void setAngularVelocity(float velocity);
    
    Vector2f getPosition() const;
    float getRotation() const;
    Vector2f getLinearVelocity() const;
    float getAngularVelocity() const;
    
    void applyForce(const Vector2f& force, const Vector2f& point);
    void applyLinearImpulse(const Vector2f& impulse, const Vector2f& point);
    void applyTorque(float torque);
    void applyAngularImpulse(float impulse);

    void setEnabled(bool enabled);
    bool isEnabled() const;
    
    const PhysicsProperties& getProperties() const;

private:
    PhysicsProperties properties_;
    Vector2f position_;
    float rotation_ = 0.0f;
    Vector2f linearVelocity_;
    float angularVelocity_ = 0.0f;
    bool enabled_ = true;
};

// Физические формы
class PhysicsShape {
public:
    virtual ~PhysicsShape() = default;
    virtual bool containsPoint(const Vector2f& point) const = 0;
};

class BoxShape : public PhysicsShape {
public:
    BoxShape(const Vector2f& size);
    bool containsPoint(const Vector2f& point) const override;
    const Vector2f& getSize() const;

private:
    Vector2f size_;
};

class CircleShape : public PhysicsShape {
public:
    CircleShape(float radius);
    bool containsPoint(const Vector2f& point) const override;
    float getRadius() const;

private:
    float radius_;
};

// Пружинное соединение
class SpringJoint {
public:
    SpringJoint(PhysicsBody* bodyA, PhysicsBody* bodyB,
               const Vector2f& anchorA, const Vector2f& anchorB);
    
    void setFrequency(float hz);
    void setDampingRatio(float ratio);
    void setLength(float length);
    
    void update(float deltaTime);

private:
    PhysicsBody* bodyA_;
    PhysicsBody* bodyB_;
    Vector2f anchorA_;
    Vector2f anchorB_;
    float frequency_ = 4.0f;
    float dampingRatio_ = 0.5f;
    float length_;
};

// Система физической симуляции
class PhysicsWorld {
public:
    PhysicsWorld(const Vector2f& gravity = Vector2f(0, 9.81f));
    
    void setGravity(const Vector2f& gravity);
    void setTimeStep(float timeStep);
    
    void update(float deltaTime);
    
    void addBody(std::shared_ptr<PhysicsBody> body);
    void removeBody(std::shared_ptr<PhysicsBody> body);
    void addJoint(std::shared_ptr<SpringJoint> joint);
    void removeJoint(std::shared_ptr<SpringJoint> joint);
    
    void queryAABB(const Rect& aabb, std::vector<PhysicsBody*>& bodies);
    PhysicsBody* rayCast(const Vector2f& start, const Vector2f& end);

private:
    Vector2f gravity_;
    float timeStep_ = 1.0f / 60.0f;
    float accumulator_ = 0.0f;
    
    std::vector<std::shared_ptr<PhysicsBody>> bodies_;
    std::vector<std::shared_ptr<SpringJoint>> joints_;
};

// Интерфейс для физических объектов UI
class PhysicsWidget {
public:
    virtual ~PhysicsWidget() = default;
    
    void enablePhysics(const PhysicsProperties& props = PhysicsProperties());
    void disablePhysics();
    
    PhysicsBody* getPhysicsBody();
    void setPhysicsShape(std::shared_ptr<PhysicsShape> shape);
    
    virtual void onPhysicsUpdate();

protected:
    std::shared_ptr<PhysicsBody> physicsBody_;
    std::shared_ptr<PhysicsShape> physicsShape_;
};

} // namespace gui