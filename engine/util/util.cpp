#include <cmath>

#include "util.h"

namespace Velocity {
Vec2i::Vec2i(int x, int y) : X(x), Y(y) {}

Vec2::Vec2(float x, float y) : X(x), Y(y) {}

Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(X + other.X, Y + other.Y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(X - other.X, Y - other.Y);
}

Vec2 Vec2::operator*(const Vec2& other) const {
    return Vec2(X * other.X, Y * other.Y);
}

Vec2 Vec2::operator*(const float factor) const {
    return Vec2(X * factor, Y * factor);
}

Vec2 Vec2::operator/(const Vec2& other) const {
    // TODO: handle division by zero error
    return Vec2(X / other.X, Y - other.Y);
}

float Vec2::Dot(const Vec2 other) const { return X * other.X + Y * other.Y; }

float Vec2::Length() const { return std::sqrt(X * X + Y * Y); }

float Vec2::SqrLength() const { return X * X + Y * Y; }

bool Vec2::Equals(const Vec2& other) const {
    return (X == other.X) && (Y == other.Y);
}

float Vec2::Distance(const Vec2 a, const Vec2 b) { return (a - b).Length(); }

// NOTE: the `t` value will be clamped between 0.0 and 1.0
Vec2 Vec2::Lerp(const Vec2 a, const Vec2 b, float t) {
    // start + (diff * t)
    return a + ((a - b) * Clampf(0.0f, t, 1.0f));
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : R(r), G(g), B(b), A(a) {}

Color Color::SetOpacity(float new_alpha) const {
    float a = Clampf(0.0f, new_alpha, 1.0f) * 255.0f;
    return Color(R, G, B, (uint8_t)a);
}

Color Color::FromHexValue(int hex) {
    uint8_t r = (hex & 0xFF) >> (8 * 3);
    uint8_t g = (hex & 0xFF) >> (8 * 2);
    uint8_t b = (hex & 0xFF) >> (8 * 1);
    uint8_t a = (hex & 0xFF) >> (8 * 0);
    return Color(r, g, b, a);
}

} // namespace Velocity
