#pragma once
#include <memory>
#include "../core/math_types.hpp"

namespace gui {

class RenderTarget;
class RenderStates;

// Абстрактный класс для рендеринга
class Renderer {
public:
    virtual ~Renderer() = default;

    // Инициализация и очистка
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;

    // Основные операции рендеринга
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual void clear(const Color& color) = 0;

    // Примитивы рендеринга
    virtual void drawRect(const Rect& rect, const Color& color, float thickness = 1.0f) = 0;
    virtual void fillRect(const Rect& rect, const Color& color) = 0;
    virtual void drawCircle(const Vector2f& center, float radius, const Color& color, float thickness = 1.0f) = 0;
    virtual void fillCircle(const Vector2f& center, float radius, const Color& color) = 0;
    virtual void drawLine(const Vector2f& start, const Vector2f& end, const Color& color, float thickness = 1.0f) = 0;
    virtual void drawTriangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, 
                            const Color& color, float thickness = 1.0f) = 0;
    virtual void fillTriangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Color& color) = 0;

    // Текст и изображения
    virtual void drawText(const std::string& text, const Vector2f& position, 
                         const std::string& font, float size, const Color& color) = 0;
    virtual void drawImage(const std::string& imagePath, const Rect& destRect, 
                         const Color& tint = Color::white()) = 0;

    // Продвинутые функции рендеринга
    virtual void pushClipRect(const Rect& rect) = 0;
    virtual void popClipRect() = 0;
    virtual void pushTransform(const Transform& transform) = 0;
    virtual void popTransform() = 0;

    // Управление состоянием
    virtual void setBlendMode(BlendMode mode) = 0;
    virtual void setAntialiasing(bool enabled) = 0;
    virtual void setViewport(const Rect& viewport) = 0;

    // Вспомогательные функции
    virtual Vector2f getTextSize(const std::string& text, const std::string& font, float size) = 0;
    virtual Vector2f getImageSize(const std::string& imagePath) = 0;
};

// Режимы наложения
enum class BlendMode {
    None,
    Alpha,
    Additive,
    Multiply,
    Screen
};

// Состояния рендеринга
struct RenderStates {
    Transform transform;
    BlendMode blendMode = BlendMode::Alpha;
    bool antialiasing = true;
    const Texture* texture = nullptr;
    const Shader* shader = nullptr;
};

// Класс для управления текстурами
class Texture {
public:
    virtual ~Texture() = default;
    virtual bool loadFromFile(const std::string& path) = 0;
    virtual bool loadFromMemory(const void* data, size_t size) = 0;
    virtual Vector2f getSize() const = 0;
};

// Класс для шейдеров
class Shader {
public:
    virtual ~Shader() = default;
    virtual bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) = 0;
    virtual bool loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader) = 0;
    
    virtual void setUniform(const std::string& name, float value) = 0;
    virtual void setUniform(const std::string& name, const Vector2f& value) = 0;
    virtual void setUniform(const std::string& name, const Color& value) = 0;
    virtual void setUniform(const std::string& name, const Transform& value) = 0;
    virtual void setUniformTexture(const std::string& name, const Texture& texture) = 0;
};

// Фабрика для создания рендереров
class RendererFactory {
public:
    static std::unique_ptr<Renderer> createRenderer(RendererType type);
    
    enum class RendererType {
        OpenGL,
        DirectX,
        Vulkan,
        Software
    };
};

} // namespace gui