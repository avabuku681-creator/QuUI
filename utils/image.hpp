#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <cstdint>
#include "../core/math_types.hpp"

namespace gui::utils {

// Класс для работы с изображениями
class Image {
public:
    enum class Format {
        RGB,
        RGBA,
        BGR,
        BGRA,
        Grayscale
    };

    Image();
    Image(unsigned int width, unsigned int height, Format format = Format::RGBA);
    ~Image();

    // Загрузка и сохранение
    bool loadFromFile(const std::string& filename);
    bool loadFromMemory(const void* data, size_t size);
    bool saveToFile(const std::string& filename) const;
    
    // Создание и изменение размера
    void create(unsigned int width, unsigned int height, Format format = Format::RGBA);
    void resize(unsigned int width, unsigned int height);
    
    // Манипуляции с пикселями
    void setPixel(unsigned int x, unsigned int y, const Color& color);
    Color getPixel(unsigned int x, unsigned int y) const;
    
    // Трансформации
    void flipHorizontally();
    void flipVertically();
    void rotate(float angle);
    void scale(float scaleX, float scaleY);
    
    // Фильтры
    void applyGrayscale();
    void applyBlur(float radius);
    void applyGaussianBlur(float sigma);
    void applySharpen();
    void applyEdgeDetection();
    void applySepia();
    void adjustBrightness(float factor);
    void adjustContrast(float factor);
    void adjustGamma(float gamma);
    void adjustHue(float angle);
    void adjustSaturation(float factor);
    
    // Рисование примитивов
    void drawLine(const Vector2f& start, const Vector2f& end, const Color& color, float thickness = 1.0f);
    void drawRect(const Rect& rect, const Color& color, float thickness = 1.0f);
    void fillRect(const Rect& rect, const Color& color);
    void drawCircle(const Vector2f& center, float radius, const Color& color, float thickness = 1.0f);
    void fillCircle(const Vector2f& center, float radius, const Color& color);
    void drawTriangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, 
                     const Color& color, float thickness = 1.0f);
    void fillTriangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Color& color);
    
    // Наложение и смешивание
    void copy(const Image& source, const Vector2f& destPos);
    void blend(const Image& source, const Vector2f& destPos, float opacity = 1.0f);
    void applyMask(const Image& mask);
    
    // Геттеры
    unsigned int getWidth() const { return width_; }
    unsigned int getHeight() const { return height_; }
    Format getFormat() const { return format_; }
    const uint8_t* getData() const { return data_.data(); }
    uint8_t* getData() { return data_.data(); }
    size_t getSize() const { return data_.size(); }

private:
    unsigned int width_;
    unsigned int height_;
    Format format_;
    std::vector<uint8_t> data_;
    
    void setPixelUnsafe(unsigned int x, unsigned int y, const Color& color);
    Color getPixelUnsafe(unsigned int x, unsigned int y) const;
    size_t getPixelIndex(unsigned int x, unsigned int y) const;
    unsigned int getBytesPerPixel() const;
};

// Генератор процедурных текстур
class TextureGenerator {
public:
    static Image generateNoise(unsigned int width, unsigned int height, 
                             float min = 0.0f, float max = 1.0f);
    
    static Image generatePerlinNoise(unsigned int width, unsigned int height,
                                   float scale = 1.0f, int octaves = 4);
    
    static Image generateGradient(unsigned int width, unsigned int height,
                                const Color& startColor, const Color& endColor,
                                float angle = 0.0f);
    
    static Image generateCheckerboard(unsigned int width, unsigned int height,
                                    unsigned int tileSize,
                                    const Color& color1, const Color& color2);
    
    static Image generateCircle(unsigned int size, const Color& fillColor,
                              const Color& outlineColor, float thickness = 1.0f);
    
    static Image generateStarfield(unsigned int width, unsigned int height,
                                 unsigned int starCount, float maxStarSize = 3.0f);
};

// Класс для работы с атласами текстур
class TextureAtlas {
public:
    struct Region {
        Rect bounds;
        std::string name;
    };

    TextureAtlas(unsigned int width, unsigned int height);
    
    bool addTexture(const std::string& name, const Image& texture);
    bool removeTexture(const std::string& name);
    
    const Region* getRegion(const std::string& name) const;
    const Image& getAtlasImage() const { return atlasImage_; }
    
    void clear();
    void optimize();
    
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

private:
    Image atlasImage_;
    std::vector<Region> regions_;
    std::unordered_map<std::string, size_t> regionMap_;
};

} // namespace gui::utils