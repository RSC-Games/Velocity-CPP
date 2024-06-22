#include "util.h"
#include <cwchar>

namespace Velocity {
Vec2i::Vec2i(int x, int y) : X(x), Y(y) {}

Vec2::Vec2(float x, float y) : X(x), Y(y) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : R(r), G(g), B(b), A(a) {}

Color::Color(float r, float g, float b, float a) {
    R = (uint8_t)(Clampf(0.0f, r, 1.0f) * 255.0f);
    G = (uint8_t)(Clampf(0.0f, g, 1.0f) * 255.0f);
    B = (uint8_t)(Clampf(0.0f, b, 1.0f) * 255.0f);
    A = (uint8_t)(Clampf(0.0f, a, 1.0f) * 255.0f);
}

} // namespace Velocity
