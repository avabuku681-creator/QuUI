#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "../core/math_types.hpp"

namespace gui {

class Theme {
public:
    struct Colors {
        Color primary;
        Color secondary;
        Color background;
        Color foreground;
        Color accent;
        Color success;
        Color warning;
        Color error;
        Color disabled;
    };

    struct Metrics {
        float borderWidth;
        float cornerRadius;
        float spacing;
        float padding;
        float iconSize;
        float fontSize;
    };

    struct Fonts {
        std::string regular;
        std::string bold;
        std::string italic;
        std::string light;
    };

    virtual ~Theme() = default;

    virtual Colors getColors() const = 0;
    virtual Metrics getMetrics() const = 0;
    virtual Fonts getFonts() const = 0;

    virtual Color getColor(const std::string& name) const = 0;
    virtual float getMetric(const std::string& name) const = 0;
    virtual std::string getFont(const std::string& name) const = 0;
};

class DarkTheme : public Theme {
public:
    Colors getColors() const override;
    Metrics getMetrics() const override;
    Fonts getFonts() const override;

    Color getColor(const std::string& name) const override;
    float getMetric(const std::string& name) const override;
    std::string getFont(const std::string& name) const override;
};

class LightTheme : public Theme {
public:
    Colors getColors() const override;
    Metrics getMetrics() const override;
    Fonts getFonts() const override;

    Color getColor(const std::string& name) const override;
    float getMetric(const std::string& name) const override;
    std::string getFont(const std::string& name) const override;
};

class CustomTheme : public Theme {
public:
    void setColor(const std::string& name, const Color& color);
    void setMetric(const std::string& name, float value);
    void setFont(const std::string& name, const std::string& fontPath);

    Colors getColors() const override;
    Metrics getMetrics() const override;
    Fonts getFonts() const override;

    Color getColor(const std::string& name) const override;
    float getMetric(const std::string& name) const override;
    std::string getFont(const std::string& name) const override;

private:
    std::unordered_map<std::string, Color> colors_;
    std::unordered_map<std::string, float> metrics_;
    std::unordered_map<std::string, std::string> fonts_;
};

} // namespace gui