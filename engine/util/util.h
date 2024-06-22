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

  public:
    float X;
    float Y;
};

struct Color {
  public:
    Color() = delete;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    Color(float r, float g, float b, float a);

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
