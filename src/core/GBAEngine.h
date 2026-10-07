#pragma once
#include <raylib.h>

// Game Boy Advance Logical Specifications
constexpr int GBA_SCREEN_WIDTH = 240;
constexpr int GBA_SCREEN_HEIGHT = 160;

// Authentic GBA Handheld Palette Constants
inline const Color GBA_BLACK       = {16, 20, 24, 255};
inline const Color GBA_WHITE       = {240, 244, 240, 255};
inline const Color GBA_CREAM       = {232, 228, 208, 255};
inline const Color GBA_DARK_BLUE   = {32, 48, 72, 255};
inline const Color GBA_MID_BLUE    = {56, 96, 136, 255};
inline const Color GBA_LIGHT_BLUE  = {120, 168, 208, 255};
inline const Color GBA_GREEN_DARK  = {34, 72, 44, 255};
inline const Color GBA_GREEN_MID   = {68, 128, 64, 255};
inline const Color GBA_GREEN_LIGHT = {128, 184, 88, 255};
inline const Color GBA_EARTH_BROWN = {112, 72, 48, 255};
inline const Color GBA_SAND_LIGHT  = {216, 192, 144, 255};
inline const Color GBA_CRIMSON     = {192, 48, 48, 255};
inline const Color GBA_GOLD        = {232, 168, 40, 255};
inline const Color GBA_FRAME_DARK  = {24, 30, 42, 255};
inline const Color GBA_FRAME_LIGHT = {200, 208, 216, 255};

#include <functional>

class GBAEngine {
public:
    static GBAEngine& instance();

    void init(int windowScale = 4);
    void beginFrame();
    void endFrame(std::function<void()> overlay = nullptr);
    void shutdown();

    Vector2 getMouseInGBACoordinates() const;
    int getScale() const { return scale_; }

private:
    GBAEngine() = default;
    ~GBAEngine() = default;

    RenderTexture2D target_{0};
    int scale_ = 4;
    int windowWidth_ = 960;
    int windowHeight_ = 640;
    Rectangle srcRect_{0, 0, 240, -160};
    Rectangle dstRect_{0, 0, 960, 640};
};
