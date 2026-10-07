#include "GBAEngine.h"
#include "utils/Logger.h"

GBAEngine& GBAEngine::instance() {
    static GBAEngine inst;
    return inst;
}

void GBAEngine::init(int windowScale) {
    scale_ = (windowScale < 1) ? 4 : windowScale;
    windowWidth_ = GBA_SCREEN_WIDTH * scale_;
    windowHeight_ = GBA_SCREEN_HEIGHT * scale_;

    target_ = LoadRenderTexture(GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT);
    SetTextureFilter(target_.texture, TEXTURE_FILTER_POINT);

    srcRect_ = {0.0f, 0.0f, static_cast<float>(GBA_SCREEN_WIDTH), -static_cast<float>(GBA_SCREEN_HEIGHT)};
    dstRect_ = {0.0f, 0.0f, static_cast<float>(windowWidth_), static_cast<float>(windowHeight_)};

    LOG_INFO(TextFormat("GBA Engine initialized: 240x160 scaled %dx to %dx%d", scale_, windowWidth_, windowHeight_));
}

void GBAEngine::beginFrame() {
    BeginTextureMode(target_);
}

void GBAEngine::endFrame(std::function<void()> overlay) {
    EndTextureMode();

    BeginDrawing();
    ClearBackground(GBA_BLACK);
    // Integer point-filtered draw to screen
    DrawTexturePro(target_.texture, srcRect_, dstRect_, Vector2{0.0f, 0.0f}, 0.0f, WHITE);

    // High-resolution screen overlay (e.g. LeetCode / Codédex coding IDE)
    if (overlay) {
        overlay();
    }

    EndDrawing();
}

void GBAEngine::shutdown() {
    if (target_.id > 0) {
        UnloadRenderTexture(target_);
        target_.id = 0;
    }
}

Vector2 GBAEngine::getMouseInGBACoordinates() const {
    Vector2 m = GetMousePosition();
    return Vector2{m.x / scale_, m.y / scale_};
}
