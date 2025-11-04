#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include "../core/math_types.hpp"

namespace gui {

// Базовый класс для всех ресурсов
class Resource {
public:
    virtual ~Resource() = default;
    virtual bool load(const std::string& path) = 0;
    virtual void unload() = 0;
    bool isLoaded() const { return loaded_; }
    const std::string& getPath() const { return path_; }

protected:
    bool loaded_ = false;
    std::string path_;
};

// Текстура
class Texture : public Resource {
public:
    bool load(const std::string& path) override;
    void unload() override;
    
    unsigned int getHandle() const;
    Vector2f getSize() const;
    void bind(unsigned int slot = 0) const;
    
    void setSmooth(bool smooth);
    void setRepeated(bool repeated);

private:
    unsigned int handle_ = 0;
    Vector2f size_;
    bool smooth_ = false;
    bool repeated_ = false;
};

// Шрифт
class Font : public Resource {
public:
    struct Glyph {
        Rect bounds;
        Rect textureRect;
        Vector2f advance;
    };

    bool load(const std::string& path) override;
    void unload() override;
    
    const Glyph& getGlyph(unsigned int codePoint, unsigned int characterSize) const;
    float getKerning(unsigned int first, unsigned int second, unsigned int characterSize) const;
    float getLineSpacing(unsigned int characterSize) const;
    
    std::shared_ptr<Texture> getTexture(unsigned int characterSize) const;

private:
    void createGlyphsForSize(unsigned int characterSize);
    
    struct FontData;
    std::unique_ptr<FontData> data_;
    mutable std::unordered_map<unsigned int, std::shared_ptr<Texture>> textures_;
};

// Шейдер
class Shader : public Resource {
public:
    bool load(const std::string& vertexPath, const std::string& fragmentPath);
    void unload() override;
    
    void bind() const;
    unsigned int getHandle() const;
    
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const Vector2f& value);
    void setUniform(const std::string& name, const Color& value);
    void setUniform(const std::string& name, const Transform& transform);
    void setUniformTexture(const std::string& name, const Texture& texture, unsigned int slot = 0);

private:
    bool loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader);
    bool compile(unsigned int shader, const std::string& source);
    bool link();
    int getUniformLocation(const std::string& name) const;
    
    unsigned int handle_ = 0;
    mutable std::unordered_map<std::string, int> uniformLocations_;
};

// Звук
class Sound : public Resource {
public:
    bool load(const std::string& path) override;
    void unload() override;
    
    void play();
    void pause();
    void stop();
    
    void setVolume(float volume);
    void setPitch(float pitch);
    void setLoop(bool loop);
    
    bool isPlaying() const;
    float getVolume() const;
    float getPitch() const;
    bool isLoop() const;

private:
    unsigned int handle_ = 0;
    float volume_ = 100.0f;
    float pitch_ = 1.0f;
    bool loop_ = false;
};

// Менеджер ресурсов
class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    template<typename T>
    std::shared_ptr<T> load(const std::string& path) {
        auto it = resources_.find(path);
        if (it != resources_.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        auto resource = std::make_shared<T>();
        if (resource->load(path)) {
            resources_[path] = resource;
            return resource;
        }
        return nullptr;
    }

    template<typename T>
    std::shared_ptr<T> get(const std::string& path) {
        auto it = resources_.find(path);
        if (it != resources_.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    void unload(const std::string& path);
    void unloadUnused();
    void unloadAll();

    void setBasePath(const std::string& path);
    const std::string& getBasePath() const;

    using ResourceLoadCallback = std::function<void(const std::string&, bool)>;
    void setLoadCallback(ResourceLoadCallback callback);

private:
    ResourceManager() = default;
    std::unordered_map<std::string, std::shared_ptr<Resource>> resources_;
    std::string basePath_;
    ResourceLoadCallback loadCallback_;
};

} // namespace gui