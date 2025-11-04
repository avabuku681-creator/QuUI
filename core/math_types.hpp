#pragma once
#include <cmath>

namespace gui {

struct Vector2f {
    float x, y;
    
    Vector2f(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    
    Vector2f operator+(const Vector2f& other) const { return Vector2f(x + other.x, y + other.y); }
    Vector2f operator-(const Vector2f& other) const { return Vector2f(x - other.x, y - other.y); }
    Vector2f operator*(float scalar) const { return Vector2f(x * scalar, y * scalar); }
    Vector2f operator/(float scalar) const { return Vector2f(x / scalar, y / scalar); }
    
    float length() const { return std::sqrt(x * x + y * y); }
    float lengthSquared() const { return x * x + y * y; }
    
    void normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }
    
    Vector2f normalized() const {
        Vector2f result = *this;
        result.normalize();
        return result;
    }
    
    float dot(const Vector2f& other) const {
        return x * other.x + y * other.y;
    }
    
    float cross(const Vector2f& other) const {
        return x * other.y - y * other.x;
    }
    
    static Vector2f lerp(const Vector2f& a, const Vector2f& b, float t) {
        return a + (b - a) * t;
    }
};

struct Rect {
    Vector2f position;
    Vector2f size;
    
    Rect(const Vector2f& pos = Vector2f(), const Vector2f& sz = Vector2f())
        : position(pos), size(sz) {}
    
    bool contains(const Vector2f& point) const {
        return point.x >= position.x && point.x <= position.x + size.x &&
               point.y >= position.y && point.y <= position.y + size.y;
    }
    
    bool intersects(const Rect& other) const {
        return position.x < other.position.x + other.size.x &&
               position.x + size.x > other.position.x &&
               position.y < other.position.y + other.size.y &&
               position.y + size.y > other.position.y;
    }
    
    Vector2f getCenter() const {
        return position + size * 0.5f;
    }
    
    void setCenter(const Vector2f& center) {
        position = center - size * 0.5f;
    }
};

struct Color {
    float r, g, b, a;
    
    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {}
    
    static Color fromRGB(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
        return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }
    
    static Color fromHex(unsigned int hex) {
        return Color(
            ((hex >> 16) & 0xFF) / 255.0f,
            ((hex >> 8) & 0xFF) / 255.0f,
            (hex & 0xFF) / 255.0f,
            ((hex >> 24) & 0xFF) / 255.0f
        );
    }
    
    Color lerp(const Color& other, float t) const {
        return Color(
            r + (other.r - r) * t,
            g + (other.g - g) * t,
            b + (other.b - b) * t,
            a + (other.a - a) * t
        );
    }
    
    static Color black() { return Color(0, 0, 0, 1); }
    static Color white() { return Color(1, 1, 1, 1); }
    static Color red() { return Color(1, 0, 0, 1); }
    static Color green() { return Color(0, 1, 0, 1); }
    static Color blue() { return Color(0, 0, 1, 1); }
    static Color transparent() { return Color(0, 0, 0, 0); }
};

struct Transform {
    Vector2f position;
    Vector2f scale;
    float rotation;
    
    Transform(const Vector2f& pos = Vector2f(), 
             const Vector2f& scl = Vector2f(1, 1),
             float rot = 0.0f)
        : position(pos), scale(scl), rotation(rot) {}
    
    Vector2f transformPoint(const Vector2f& point) const {
        // Применяем масштаб
        Vector2f scaled(point.x * scale.x, point.y * scale.y);
        
        // Применяем поворот
        float rad = rotation * 3.14159f / 180.0f;
        float cos_r = std::cos(rad);
        float sin_r = std::sin(rad);
        Vector2f rotated(
            scaled.x * cos_r - scaled.y * sin_r,
            scaled.x * sin_r + scaled.y * cos_r
        );
        
        // Применяем перемещение
        return rotated + position;
    }
    
    Transform& combine(const Transform& other) {
        // Комбинируем масштаб
        scale.x *= other.scale.x;
        scale.y *= other.scale.y;
        
        // Комбинируем поворот
        rotation += other.rotation;
        
        // Комбинируем позицию с учётом масштаба и поворота
        position = other.transformPoint(position);
        
        return *this;
    }
};

} // namespace gui