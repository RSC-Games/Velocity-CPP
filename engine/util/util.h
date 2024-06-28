#pragma once

#include <cstdint>

namespace Velocity {
struct Vec2i {
  public:
    Vec2i() = delete;
    Vec2i(int x, int y);

  public:
    int X;
    int Y;
};

struct Vec2 {
  public:
    Vec2() = delete;
    Vec2(float x, float y);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(const Vec2& other) const;
    Vec2 operator*(const float factor) const;
    Vec2 operator/(const Vec2& other) const;

    float Dot(const Vec2 other) const;
    float Length() const;
    float SqrLength() const;
    bool Equals(const Vec2& other) const;

    static Vec2 Lerp(const Vec2 a, const Vec2 b, float t);
    static float Distance(const Vec2 a, const Vec2 b);

    inline static Vec2 Zero() { return Vec2(0.0f, 0.0f); }

    inline static Vec2 One() { return Vec2(1.0f, 1.0f); }

    inline static Vec2 Up() { return Vec2(0.0f, 1.0f); }

    inline static Vec2 Down() { return Vec2(0.0f, -1.0f); }

    inline static Vec2 Left() { return Vec2(-1.0f, 0.0f); }

    inline static Vec2 Right() { return Vec2(1.0f, 0.0f); }

  public:
    float X;
    float Y;
};

struct Color {
  public:
    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    Color SetOpacity(float new_alpha) const;
    Color SetBrightness(float brightness) const;

    static Color FromHexValue(int hex);

    inline static Color Red() { return Color(255, 0, 0, 255); }
    inline static Color Green() { return Color(0, 255, 0, 255); }
    inline static Color Blue() { return Color(0, 0, 255, 255); }
    inline static Color Yellow() { return Color(255, 255, 0, 255); }
    inline static Color Magenta() { return Color(255, 0, 255, 255); }
    inline static Color Cyan() { return Color(0, 255, 255, 255); }
    inline static Color Orange() { return Color(255, 127, 0, 255); }
    inline static Color White() { return Color(255, 255, 255, 255); }
    inline static Color Black() { return Color(0, 0, 0, 255); }
    inline static Color Blank() { return Color(0, 0, 0, 0); }

  public:
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
};

inline float Clampf(float min, float value, float max) {
    float res = value;
    if (res < min)
        res = min;
    if (res > max)
        res = max;
    return res;
}

} // namespace Velocity
