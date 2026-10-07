#include "UISystem.h"
#include "audio/SFXManager.h"

const Color CYBER_CYAN    = {0, 229, 255, 255};
const Color CYBER_VIOLET  = {124, 77, 255, 255};
const Color CYBER_MAGENTA = {255, 64, 129, 255};
const Color CYBER_DARK    = {10, 14, 23, 255};
const Color CYBER_PANEL   = {24, 32, 48, 255};
const Color CYBER_TEXT    = {220, 230, 255, 255};
const Color CYBER_DIM     = {100, 110, 140, 255};
const Color CYBER_SUCCESS = {0, 230, 118, 255};
const Color CYBER_WARNING = {255, 171, 0, 255};
const Color CYBER_DANGER  = {255, 82, 82, 255};

namespace UISystem {

void drawGlow(Rectangle rect, Color color, float intensity) {
    color.a = (unsigned char)(255 * intensity);
    DrawRectangleRounded({rect.x - 4, rect.y - 4, rect.width + 8, rect.height + 8}, 0.2f, 8, color);
}

Button::Button(Rectangle bounds, std::string text, std::function<void()> onClick) 
    : bounds_(bounds), text_(text), onClick_(onClick) {}

void Button::update() {
    if (state_ == ButtonState::DISABLED) return;
    Vector2 mousePt = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mousePt, bounds_);
    
    if (hovered) {
        if (state_ == ButtonState::IDLE) {
            SFXManager::instance().play("hover");
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            state_ = ButtonState::PRESSED;
        } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (state_ == ButtonState::PRESSED && onClick_) {
                SFXManager::instance().play("click");
                onClick_();
            }
            state_ = ButtonState::HOVERED;
        } else {
            state_ = ButtonState::HOVERED;
        }
    } else {
        state_ = ButtonState::IDLE;
    }
    
    float dt = GetFrameTime();
    if (state_ == ButtonState::HOVERED) hoverAnim_ += dt * 6.0f;
    else hoverAnim_ -= dt * 6.0f;
    
    if (state_ == ButtonState::PRESSED) pressAnim_ += dt * 12.0f;
    else pressAnim_ -= dt * 12.0f;
    
    if (hoverAnim_ > 1.0f) hoverAnim_ = 1.0f;
    if (hoverAnim_ < 0.0f) hoverAnim_ = 0.0f;
    if (pressAnim_ > 1.0f) pressAnim_ = 1.0f;
    if (pressAnim_ < 0.0f) pressAnim_ = 0.0f;
}

void Button::draw() {
    Rectangle drawBounds = bounds_;
    if (pressAnim_ > 0) {
        float offset = 2.0f * pressAnim_;
        drawBounds.x += offset; drawBounds.y += offset;
        drawBounds.width -= offset*2; drawBounds.height -= offset*2;
    }
    
    Color bg = CYBER_PANEL;
    Color border = Color{50, 70, 100, 255};
    Color txt = CYBER_TEXT;
    
    if (state_ == ButtonState::DISABLED) {
        txt = CYBER_DIM; border.a = 100; bg.a = 150;
    } else {
        if (hoverAnim_ > 0) {
            border = { (unsigned char)(50 + (CYBER_CYAN.r - 50)*hoverAnim_),
                       (unsigned char)(70 + (CYBER_CYAN.g - 70)*hoverAnim_),
                       (unsigned char)(100 + (CYBER_CYAN.b - 100)*hoverAnim_), 255 };
            bg = { (unsigned char)(CYBER_PANEL.r + 20*hoverAnim_), (unsigned char)(CYBER_PANEL.g + 20*hoverAnim_), (unsigned char)(CYBER_PANEL.b + 20*hoverAnim_), 255 };
            drawGlow(drawBounds, border, hoverAnim_ * 0.35f);
        }
    }

    DrawRectangleRounded(drawBounds, 0.2f, 8, bg);
    DrawRectangleRoundedLinesEx(drawBounds, 0.2f, 8, 2.0f, border);
    
    int fontSize = static_cast<int>(drawBounds.height * 0.42f);
    int tw = MeasureText(text_.c_str(), fontSize);
    DrawText(text_.c_str(), static_cast<int>(drawBounds.x + drawBounds.width/2 - tw/2), static_cast<int>(drawBounds.y + drawBounds.height/2 - fontSize/2), fontSize, txt);
}

void Button::setPosition(float x, float y) { bounds_.x = x; bounds_.y = y; }
void Button::setSize(float w, float h) { bounds_.width = w; bounds_.height = h; }
void Button::setDisabled(bool disabled) { state_ = disabled ? ButtonState::DISABLED : ButtonState::IDLE; }

void Panel::draw() {
    if (glowIntensity > 0) drawGlow(bounds, borderColor, glowIntensity);
    DrawRectangleRounded(bounds, cornerRadius, 10, bgColor);
    if (borderWidth > 0) DrawRectangleRoundedLinesEx(bounds, cornerRadius, 10, borderWidth, borderColor);
}

void Slider::update() {
    Vector2 mousePt = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Rectangle handleArea = {bounds.x, bounds.y - 10, bounds.width, bounds.height + 20};
        if (CheckCollisionPointRec(mousePt, handleArea)) {
            float p = (mousePt.x - bounds.x) / bounds.width;
            if (p < 0) p = 0; if (p > 1) p = 1;
            value = min + p * (max - min);
        }
    }
}

void Slider::draw() {
    DrawRectangleRounded(bounds, 0.5f, 4, CYBER_DIM);
    float p = (value - min) / (max - min);
    Rectangle fill = {bounds.x, bounds.y, bounds.width * p, bounds.height};
    DrawRectangleRounded(fill, 0.5f, 4, CYBER_CYAN);
    DrawCircle(static_cast<int>(bounds.x + bounds.width * p), static_cast<int>(bounds.y + bounds.height/2), bounds.height*1.5f, CYBER_TEXT);
    DrawText(label.c_str(), static_cast<int>(bounds.x), static_cast<int>(bounds.y - 20), 15, CYBER_TEXT);
}

void ProgressBar::draw() {
    DrawRectangleRounded(bounds, 0.2f, 8, bgColor);
    DrawRectangleRoundedLinesEx(bounds, 0.2f, 8, 2.0f, CYBER_DIM);
    Rectangle fill = {bounds.x+2, bounds.y+2, (bounds.width-4)*value, bounds.height-4};
    DrawRectangleRounded(fill, 0.2f, 8, fillColor);
    if (!label.empty()) {
        DrawText(label.c_str(), bounds.x + 10, bounds.y + bounds.height/2 - 7, 15, CYBER_TEXT);
        std::string p = std::to_string((int)(value*100)) + "%";
        DrawText(p.c_str(), bounds.x + bounds.width - 40, bounds.y + bounds.height/2 - 7, 15, CYBER_TEXT);
    }
}

void Label::draw() { DrawText(text.c_str(), position.x, position.y, fontSize, color); }
}
