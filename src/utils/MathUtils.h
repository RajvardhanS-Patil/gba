#pragma once
// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// MathUtils — Easing functions, lerp, clamp, and animation helpers
// ═══════════════════════════════════════════════════════════════════════════

#include <cmath>
#include <algorithm>
#include "raylib.h"

namespace MathUtils {

// ─── Basic math ─────────────────────────────────────────────────────────
inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

inline float clamp(float value, float min, float max) {
    return std::max(min, std::min(max, value));
}

inline float clamp01(float value) {
    return clamp(value, 0.0f, 1.0f);
}

inline float inverseLerp(float a, float b, float value) {
    if (std::abs(b - a) < 1e-6f) return 0.0f;
    return clamp01((value - a) / (b - a));
}

inline float remap(float inMin, float inMax, float outMin, float outMax, float value) {
    float t = inverseLerp(inMin, inMax, value);
    return lerp(outMin, outMax, t);
}

// ─── Easing functions (t should be 0..1) ────────────────────────────────
inline float easeInQuad(float t)     { return t * t; }
inline float easeOutQuad(float t)    { return t * (2.0f - t); }
inline float easeInOutQuad(float t)  { return t < 0.5f ? 2*t*t : -1 + (4 - 2*t)*t; }
inline float easeInCubic(float t)    { return t * t * t; }
inline float easeOutCubic(float t)   { float f = t - 1; return f*f*f + 1; }
inline float easeInOutCubic(float t) { return t < 0.5f ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1; }

inline float easeOutElastic(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return std::pow(2.0f, -10.0f * t) * std::sin((t - 0.075f) * (2.0f * PI) / 0.3f) + 1.0f;
}

inline float easeOutBounce(float t) {
    if (t < 1.0f / 2.75f) {
        return 7.5625f * t * t;
    } else if (t < 2.0f / 2.75f) {
        t -= 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5f / 2.75f) {
        t -= 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= 2.625f / 2.75f;
        return 7.5625f * t * t + 0.984375f;
    }
}

// ─── Color helpers ──────────────────────────────────────────────────────
inline Color lerpColor(Color a, Color b, float t) {
    t = clamp01(t);
    return Color{
        static_cast<unsigned char>(lerp(a.r, b.r, t)),
        static_cast<unsigned char>(lerp(a.g, b.g, t)),
        static_cast<unsigned char>(lerp(a.b, b.b, t)),
        static_cast<unsigned char>(lerp(a.a, b.a, t))
    };
}

inline Color withAlpha(Color c, unsigned char alpha) {
    return Color{c.r, c.g, c.b, alpha};
}

inline Color withAlpha(Color c, float alpha) {
    return Color{c.r, c.g, c.b, static_cast<unsigned char>(clamp01(alpha) * 255.0f)};
}

// ─── Vector helpers ─────────────────────────────────────────────────────
inline Vector2 lerpVec2(Vector2 a, Vector2 b, float t) {
    return Vector2{lerp(a.x, b.x, t), lerp(a.y, b.y, t)};
}

inline float distance(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

inline Vector2 normalize(Vector2 v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len < 1e-6f) return Vector2{0, 0};
    return Vector2{v.x / len, v.y / len};
}

// ─── Rectangle helpers ──────────────────────────────────────────────────
inline bool pointInRect(Vector2 point, Rectangle rect) {
    return point.x >= rect.x && point.x <= rect.x + rect.width &&
           point.y >= rect.y && point.y <= rect.y + rect.height;
}

inline Rectangle centerRect(float cx, float cy, float w, float h) {
    return Rectangle{cx - w * 0.5f, cy - h * 0.5f, w, h};
}

} // namespace MathUtils
