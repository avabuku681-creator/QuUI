#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <unordered_map>
#include "../core/math_types.hpp"

namespace gui::utils {

// Утилиты для работы со строками
class StringUtils {
public:
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string join(const std::vector<std::string>& parts, const std::string& delimiter);
    static std::string trim(const std::string& str);
    static std::string toLower(const std::string& str);
    static std::string toUpper(const std::string& str);
    static bool startsWith(const std::string& str, const std::string& prefix);
    static bool endsWith(const std::string& str, const std::string& suffix);
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    static std::string format(const char* fmt, ...);
    
    // Конвертация типов
    template<typename T>
    static std::string toString(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    template<typename T>
    static T fromString(const std::string& str) {
        T value;
        std::istringstream iss(str);
        iss >> value;
        return value;
    }
};

// Утилиты для работы с файлами
class FileUtils {
public:
    static bool fileExists(const std::string& path);
    static bool createDirectory(const std::string& path);
    static bool removeFile(const std::string& path);
    static bool removeDirectory(const std::string& path);
    static std::string getFileExtension(const std::string& path);
    static std::string getFileName(const std::string& path);
    static std::string getFilePath(const std::string& path);
    static std::vector<std::string> listFiles(const std::string& directory, const std::string& extension = "");
    static std::string readFileToString(const std::string& path);
    static std::vector<uint8_t> readFileToBuffer(const std::string& path);
    static bool writeStringToFile(const std::string& path, const std::string& content);
    static bool writeBufferToFile(const std::string& path, const std::vector<uint8_t>& buffer);
};

// Утилиты для математических операций
class MathUtils {
public:
    static constexpr float PI = 3.14159265358979323846f;
    static constexpr float TWO_PI = PI * 2.0f;
    static constexpr float HALF_PI = PI * 0.5f;
    
    template<typename T>
    static T clamp(T value, T min, T max) {
        return std::min(std::max(value, min), max);
    }
    
    static float lerp(float a, float b, float t) {
        return a + (b - a) * clamp(t, 0.0f, 1.0f);
    }
    
    static float smoothStep(float edge0, float edge1, float x) {
        float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }
    
    static float radiansToDegrees(float radians) {
        return radians * 180.0f / PI;
    }
    
    static float degreesToRadians(float degrees) {
        return degrees * PI / 180.0f;
    }
    
    static Vector2f rotateVector(const Vector2f& vector, float angle) {
        float rad = degreesToRadians(angle);
        float cos = std::cos(rad);
        float sin = std::sin(rad);
        return Vector2f(
            vector.x * cos - vector.y * sin,
            vector.x * sin + vector.y * cos
        );
    }
};

// Генератор случайных чисел
class Random {
public:
    static void seed(unsigned int seed) {
        generator.seed(seed);
    }
    
    static void seedFromTime() {
        generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    }
    
    static int getInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(generator);
    }
    
    static float getFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(generator);
    }
    
    static bool getBool(float probability = 0.5f) {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(generator) < probability;
    }
    
    static Vector2f getVector2f(float minX, float maxX, float minY, float maxY) {
        return Vector2f(getFloat(minX, maxX), getFloat(minY, maxY));
    }
    
    static Color getColor(bool includeAlpha = false) {
        return Color(
            getFloat(0.0f, 1.0f),
            getFloat(0.0f, 1.0f),
            getFloat(0.0f, 1.0f),
            includeAlpha ? getFloat(0.0f, 1.0f) : 1.0f
        );
    }

private:
    static std::mt19937 generator;
};

// Профилировщик для измерения производительности
class Profiler {
public:
    class ScopedTimer {
    public:
        ScopedTimer(const std::string& name) 
            : name_(name), start_(std::chrono::high_resolution_clock::now()) {}
        
        ~ScopedTimer() {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_).count();
            Profiler::getInstance().addMeasurement(name_, duration);
        }
    
    private:
        std::string name_;
        std::chrono::high_resolution_clock::time_point start_;
    };

    static Profiler& getInstance() {
        static Profiler instance;
        return instance;
    }
    
    void addMeasurement(const std::string& name, int64_t microseconds);
    void reset();
    void printReport();

private:
    struct Measurement {
        int64_t total = 0;
        int64_t min = std::numeric_limits<int64_t>::max();
        int64_t max = 0;
        int count = 0;
    };
    
    std::unordered_map<std::string, Measurement> measurements_;
};

// Макрос для профилирования блока кода
#define PROFILE_SCOPE(name) gui::utils::Profiler::ScopedTimer timer##__LINE__(name)

} // namespace gui::utils