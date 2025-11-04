#include "image.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include <stdexcept>

namespace gui::utils {

Image::Image() : width_(0), height_(0), format_(Format::RGBA) {}

Image::Image(unsigned int width, unsigned int height, Format format)
    : width_(width), height_(height), format_(format) {
    data_.resize(width * height * getBytesPerPixel());
}

Image::~Image() = default;

void Image::create(unsigned int width, unsigned int height, Format format) {
    width_ = width;
    height_ = height;
    format_ = format;
    data_.resize(width * height * getBytesPerPixel());
}

void Image::resize(unsigned int width, unsigned int height) {
    if (width == width_ && height == height_)
        return;

    Image newImage(width, height, format_);
    
    float scaleX = static_cast<float>(width_) / width;
    float scaleY = static_cast<float>(height_) / height;
    
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            unsigned int srcX = static_cast<unsigned int>(x * scaleX);
            unsigned int srcY = static_cast<unsigned int>(y * scaleY);
            newImage.setPixel(x, y, getPixel(srcX, srcY));
        }
    }
    
    *this = std::move(newImage);
}

void Image::setPixel(unsigned int x, unsigned int y, const Color& color) {
    if (x < width_ && y < height_)
        setPixelUnsafe(x, y, color);
}

Color Image::getPixel(unsigned int x, unsigned int y) const {
    if (x < width_ && y < height_)
        return getPixelUnsafe(x, y);
    return Color();
}

void Image::flipHorizontally() {
    const unsigned int bytesPerPixel = getBytesPerPixel();
    const unsigned int rowSize = width_ * bytesPerPixel;
    std::vector<uint8_t> row(rowSize);
    
    for (unsigned int y = 0; y < height_; ++y) {
        uint8_t* rowStart = &data_[y * rowSize];
        std::memcpy(row.data(), rowStart, rowSize);
        
        for (unsigned int x = 0; x < width_; ++x) {
            unsigned int flippedX = width_ - 1 - x;
            std::memcpy(rowStart + x * bytesPerPixel,
                       row.data() + flippedX * bytesPerPixel,
                       bytesPerPixel);
        }
    }
}

void Image::flipVertically() {
    const unsigned int rowSize = width_ * getBytesPerPixel();
    std::vector<uint8_t> row(rowSize);
    
    for (unsigned int y = 0; y < height_ / 2; ++y) {
        unsigned int flippedY = height_ - 1 - y;
        uint8_t* topRow = &data_[y * rowSize];
        uint8_t* bottomRow = &data_[flippedY * rowSize];
        
        std::memcpy(row.data(), topRow, rowSize);
        std::memcpy(topRow, bottomRow, rowSize);
        std::memcpy(bottomRow, row.data(), rowSize);
    }
}

void Image::rotate(float angle) {
    // Convert angle to radians
    float radians = angle * 3.14159f / 180.0f;
    float cosAngle = std::cos(radians);
    float sinAngle = std::sin(radians);
    
    Image rotated(width_, height_, format_);
    
    float centerX = width_ / 2.0f;
    float centerY = height_ / 2.0f;
    
    for (unsigned int y = 0; y < height_; ++y) {
        for (unsigned int x = 0; x < width_; ++x) {
            float dx = x - centerX;
            float dy = y - centerY;
            
            int srcX = static_cast<int>(centerX + (dx * cosAngle - dy * sinAngle));
            int srcY = static_cast<int>(centerY + (dx * sinAngle + dy * cosAngle));
            
            if (srcX >= 0 && srcX < static_cast<int>(width_) &&
                srcY >= 0 && srcY < static_cast<int>(height_)) {
                rotated.setPixel(x, y, getPixel(srcX, srcY));
            }
        }
    }
    
    *this = std::move(rotated);
}

void Image::applyGrayscale() {
    const unsigned int numPixels = width_ * height_;
    const unsigned int bytesPerPixel = getBytesPerPixel();
    
    for (unsigned int i = 0; i < numPixels; ++i) {
        uint8_t* pixel = &data_[i * bytesPerPixel];
        
        // Calculate luminance using standard weights
        uint8_t gray = static_cast<uint8_t>(
            0.299f * pixel[0] + 0.587f * pixel[1] + 0.114f * pixel[2]
        );
        
        pixel[0] = pixel[1] = pixel[2] = gray;
    }
}

void Image::applyGaussianBlur(float sigma) {
    const int kernelSize = static_cast<int>(std::ceil(sigma * 6));
    std::vector<float> kernel(kernelSize);
    float sum = 0.0f;
    
    // Generate 1D Gaussian kernel
    for (int i = 0; i < kernelSize; ++i) {
        float x = i - (kernelSize - 1) / 2.0f;
        kernel[i] = std::exp(-(x * x) / (2 * sigma * sigma));
        sum += kernel[i];
    }
    
    // Normalize kernel
    for (float& k : kernel)
        k /= sum;
    
    // Horizontal pass
    Image temp(width_, height_, format_);
    for (unsigned int y = 0; y < height_; ++y) {
        for (unsigned int x = 0; x < width_; ++x) {
            Vector4f sum(0.0f);
            float weightSum = 0.0f;
            
            for (int i = 0; i < kernelSize; ++i) {
                int srcX = x + i - kernelSize / 2;
                if (srcX >= 0 && srcX < static_cast<int>(width_)) {
                    Color pixel = getPixel(srcX, y);
                    sum += Vector4f(pixel.r, pixel.g, pixel.b, pixel.a) * kernel[i];
                    weightSum += kernel[i];
                }
            }
            
            sum /= weightSum;
            temp.setPixel(x, y, Color(
                static_cast<uint8_t>(sum.x),
                static_cast<uint8_t>(sum.y),
                static_cast<uint8_t>(sum.z),
                static_cast<uint8_t>(sum.w)
            ));
        }
    }
    
    // Vertical pass
    for (unsigned int y = 0; y < height_; ++y) {
        for (unsigned int x = 0; x < width_; ++x) {
            Vector4f sum(0.0f);
            float weightSum = 0.0f;
            
            for (int i = 0; i < kernelSize; ++i) {
                int srcY = y + i - kernelSize / 2;
                if (srcY >= 0 && srcY < static_cast<int>(height_)) {
                    Color pixel = temp.getPixel(x, srcY);
                    sum += Vector4f(pixel.r, pixel.g, pixel.b, pixel.a) * kernel[i];
                    weightSum += kernel[i];
                }
            }
            
            sum /= weightSum;
            setPixel(x, y, Color(
                static_cast<uint8_t>(sum.x),
                static_cast<uint8_t>(sum.y),
                static_cast<uint8_t>(sum.z),
                static_cast<uint8_t>(sum.w)
            ));
        }
    }
}

void Image::drawLine(const Vector2f& start, const Vector2f& end, 
                    const Color& color, float thickness) {
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float length = std::sqrt(dx * dx + dy * dy);
    
    if (length < 0.0001f)
        return;
    
    float nx = -dy / length;
    float ny = dx / length;
    
    float halfThickness = thickness * 0.5f;
    std::vector<Vector2f> points = {
        Vector2f(start.x + nx * halfThickness, start.y + ny * halfThickness),
        Vector2f(end.x + nx * halfThickness, end.y + ny * halfThickness),
        Vector2f(end.x - nx * halfThickness, end.y - ny * halfThickness),
        Vector2f(start.x - nx * halfThickness, start.y - ny * halfThickness)
    };
    
    fillTriangle(points[0], points[1], points[2], color);
    fillTriangle(points[0], points[2], points[3], color);
}

void Image::fillTriangle(const Vector2f& p1, const Vector2f& p2, 
                        const Vector2f& p3, const Color& color) {
    auto minX = static_cast<int>(std::min({p1.x, p2.x, p3.x}));
    auto minY = static_cast<int>(std::min({p1.y, p2.y, p3.y}));
    auto maxX = static_cast<int>(std::ceil(std::max({p1.x, p2.x, p3.x})));
    auto maxY = static_cast<int>(std::ceil(std::max({p1.y, p2.y, p3.y})));
    
    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    maxX = std::min(maxX, static_cast<int>(width_));
    maxY = std::min(maxY, static_cast<int>(height_));
    
    auto edge = [](const Vector2f& a, const Vector2f& b, const Vector2f& p) {
        return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
    };
    
    for (int y = minY; y < maxY; ++y) {
        for (int x = minX; x < maxX; ++x) {
            Vector2f p(static_cast<float>(x) + 0.5f, 
                      static_cast<float>(y) + 0.5f);
            
            float w1 = edge(p2, p3, p);
            float w2 = edge(p3, p1, p);
            float w3 = edge(p1, p2, p);
            
            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                setPixel(x, y, color);
            }
        }
    }
}

// TextureGenerator implementation

Image TextureGenerator::generatePerlinNoise(unsigned int width, unsigned int height,
                                          float scale, int octaves) {
    Image result(width, height, Image::Format::Grayscale);
    std::vector<std::vector<float>> noise(width, std::vector<float>(height));
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    auto fade = [](float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    };
    
    auto lerp = [](float t, float a, float b) {
        return a + t * (b - a);
    };
    
    // Generate gradient vectors
    std::vector<Vector2f> gradients;
    for (int i = 0; i < 256; ++i) {
        float angle = dis(gen) * 2 * 3.14159f;
        gradients.emplace_back(std::cos(angle), std::sin(angle));
    }
    
    for (int octave = 0; octave < octaves; ++octave) {
        float frequency = std::pow(2.0f, octave);
        float amplitude = std::pow(0.5f, octave);
        
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                float fx = x * scale * frequency / width;
                float fy = y * scale * frequency / height;
                
                int x0 = static_cast<int>(std::floor(fx));
                int y0 = static_cast<int>(std::floor(fy));
                int x1 = x0 + 1;
                int y1 = y0 + 1;
                
                float tx = fx - x0;
                float ty = fy - y0;
                
                tx = fade(tx);
                ty = fade(ty);
                
                // Get gradients
                Vector2f g00 = gradients[(x0 + y0) & 255];
                Vector2f g10 = gradients[(x1 + y0) & 255];
                Vector2f g01 = gradients[(x0 + y1) & 255];
                Vector2f g11 = gradients[(x1 + y1) & 255];
                
                // Dot products
                float n00 = g00.x * (fx - x0) + g00.y * (fy - y0);
                float n10 = g10.x * (fx - x1) + g10.y * (fy - y0);
                float n01 = g01.x * (fx - x0) + g01.y * (fy - y1);
                float n11 = g11.x * (fx - x1) + g11.y * (fy - y1);
                
                // Interpolation
                float nx0 = lerp(tx, n00, n10);
                float nx1 = lerp(tx, n01, n11);
                float value = lerp(ty, nx0, nx1);
                
                noise[x][y] += value * amplitude;
            }
        }
    }
    
    // Normalize and set pixels
    float minVal = std::numeric_limits<float>::max();
    float maxVal = std::numeric_limits<float>::lowest();
    
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            minVal = std::min(minVal, noise[x][y]);
            maxVal = std::max(maxVal, noise[x][y]);
        }
    }
    
    float range = maxVal - minVal;
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            float normalizedValue = (noise[x][y] - minVal) / range;
            uint8_t pixel = static_cast<uint8_t>(normalizedValue * 255);
            result.setPixel(x, y, Color(pixel, pixel, pixel));
        }
    }
    
    return result;
}

// TextureAtlas implementation

TextureAtlas::TextureAtlas(unsigned int width, unsigned int height)
    : atlasImage_(width, height, Image::Format::RGBA) {}

bool TextureAtlas::addTexture(const std::string& name, const Image& texture) {
    if (regionMap_.find(name) != regionMap_.end())
        return false;
        
    // Simple packing algorithm (can be improved with better bin packing)
    unsigned int currentX = 0;
    unsigned int currentY = 0;
    unsigned int maxRowHeight = 0;
    
    for (const auto& region : regions_) {
        if (currentX + texture.getWidth() > atlasImage_.getWidth()) {
            currentX = 0;
            currentY += maxRowHeight;
            maxRowHeight = 0;
        }
        
        maxRowHeight = std::max(maxRowHeight, region.bounds.height);
        currentX += region.bounds.width;
    }
    
    if (currentY + texture.getHeight() > atlasImage_.getHeight())
        return false;
        
    // Copy texture to atlas
    for (unsigned int y = 0; y < texture.getHeight(); ++y) {
        for (unsigned int x = 0; x < texture.getWidth(); ++x) {
            atlasImage_.setPixel(currentX + x, currentY + y, 
                               texture.getPixel(x, y));
        }
    }
    
    // Add region
    Region region;
    region.bounds = Rect(currentX, currentY, 
                        texture.getWidth(), texture.getHeight());
    region.name = name;
    
    regions_.push_back(region);
    regionMap_[name] = regions_.size() - 1;
    
    return true;
}

const TextureAtlas::Region* TextureAtlas::getRegion(const std::string& name) const {
    auto it = regionMap_.find(name);
    if (it != regionMap_.end())
        return &regions_[it->second];
    return nullptr;
}

void TextureAtlas::optimize() {
    // Sort regions by height
    std::sort(regions_.begin(), regions_.end(),
              [](const Region& a, const Region& b) {
                  return a.bounds.height > b.bounds.height;
              });
              
    // Create new atlas with optimized layout
    Image newAtlas(atlasImage_.getWidth(), atlasImage_.getHeight(), 
                  atlasImage_.getFormat());
                  
    unsigned int currentX = 0;
    unsigned int currentY = 0;
    unsigned int rowHeight = 0;
    
    for (auto& region : regions_) {
        if (currentX + region.bounds.width > newAtlas.getWidth()) {
            currentX = 0;
            currentY += rowHeight;
            rowHeight = 0;
        }
        
        // Copy region to new position
        for (unsigned int y = 0; y < region.bounds.height; ++y) {
            for (unsigned int x = 0; x < region.bounds.width; ++x) {
                newAtlas.setPixel(currentX + x, currentY + y,
                                atlasImage_.getPixel(
                                    region.bounds.x + x,
                                    region.bounds.y + y));
            }
        }
        
        // Update region position
        region.bounds.x = currentX;
        region.bounds.y = currentY;
        
        currentX += region.bounds.width;
        rowHeight = std::max(rowHeight, region.bounds.height);
    }
    
    atlasImage_ = std::move(newAtlas);
    
    // Update region map
    regionMap_.clear();
    for (size_t i = 0; i < regions_.size(); ++i) {
        regionMap_[regions_[i].name] = i;
    }
}

void TextureAtlas::clear() {
    regions_.clear();
    regionMap_.clear();
    atlasImage_.create(atlasImage_.getWidth(), atlasImage_.getHeight(),
                      atlasImage_.getFormat());
}

unsigned int Image::getBytesPerPixel() const {
    switch (format_) {
        case Format::RGB: return 3;
        case Format::RGBA: return 4;
        case Format::BGR: return 3;
        case Format::BGRA: return 4;
        case Format::Grayscale: return 1;
        default: return 0;
    }
}

void Image::setPixelUnsafe(unsigned int x, unsigned int y, const Color& color) {
    size_t index = getPixelIndex(x, y);
    switch (format_) {
        case Format::RGB:
            data_[index] = color.r;
            data_[index + 1] = color.g;
            data_[index + 2] = color.b;
            break;
        case Format::RGBA:
            data_[index] = color.r;
            data_[index + 1] = color.g;
            data_[index + 2] = color.b;
            data_[index + 3] = color.a;
            break;
        case Format::BGR:
            data_[index] = color.b;
            data_[index + 1] = color.g;
            data_[index + 2] = color.r;
            break;
        case Format::BGRA:
            data_[index] = color.b;
            data_[index + 1] = color.g;
            data_[index + 2] = color.r;
            data_[index + 3] = color.a;
            break;
        case Format::Grayscale:
            data_[index] = static_cast<uint8_t>(
                0.299f * color.r + 0.587f * color.g + 0.114f * color.b
            );
            break;
    }
}

Color Image::getPixelUnsafe(unsigned int x, unsigned int y) const {
    size_t index = getPixelIndex(x, y);
    switch (format_) {
        case Format::RGB:
            return Color(data_[index], data_[index + 1], data_[index + 2]);
        case Format::RGBA:
            return Color(data_[index], data_[index + 1], 
                       data_[index + 2], data_[index + 3]);
        case Format::BGR:
            return Color(data_[index + 2], data_[index + 1], data_[index]);
        case Format::BGRA:
            return Color(data_[index + 2], data_[index + 1],
                       data_[index], data_[index + 3]);
        case Format::Grayscale:
            return Color(data_[index], data_[index], data_[index]);
        default:
            return Color();
    }
}

size_t Image::getPixelIndex(unsigned int x, unsigned int y) const {
    return (y * width_ + x) * getBytesPerPixel();
}

} // namespace gui::utils