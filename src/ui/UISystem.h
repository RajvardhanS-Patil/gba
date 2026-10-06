#pragma once
#include "raylib.h"
#include <string>
#include <functional>

extern const Color CYBER_CYAN;
extern const Color CYBER_VIOLET;
extern const Color CYBER_MAGENTA;
extern const Color CYBER_DARK;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;
extern const Color CYBER_SUCCESS;
extern const Color CYBER_WARNING;
extern const Color CYBER_DANGER;

namespace UISystem {
    void drawGlow(Rectangle rect, Color color, float intensity);

    enum class ButtonState { IDLE, HOVERED, PRESSED, DISABLED };

    class Button {
    private:
        Rectangle bounds_;
        std::string text_;
        ButtonState state_ = ButtonState::IDLE;
        float hoverAnim_ = 0.0f;
        float pressAnim_ = 0.0f;
        std::function<void()> onClick_;

    public:
        Button(Rectangle bounds, std::string text, std::function<void()> onClick);
        void update();
        void draw();
        void setPosition(float x, float y);
        void setSize(float w, float h);
        void setDisabled(bool disabled);
    };

    class Panel {
    public:
        Rectangle bounds;
        Color bgColor = CYBER_PANEL;
        Color borderColor = CYBER_DIM;
        float borderWidth = 2.0f;
        float cornerRadius = 0.1f;
        float glowIntensity = 0.0f;

        void draw();
    };

    class Slider {
    public:
        Rectangle bounds;
        float value = 0.0f;
        float min = 0.0f;
        float max = 1.0f;
        std::string label;

        void update();
        void draw();
        float getValue() const { return value; }
        void setValue(float v) { value = v; }
    };

    class ProgressBar {
    public:
        Rectangle bounds;
        float value = 0.0f;
        Color fillColor = CYBER_CYAN;
        Color bgColor = CYBER_DARK;
        std::string label;

        void draw();
    };

    class Label {
    public:
        Vector2 position;
        std::string text;
        int fontSize = 20;
        Color color = CYBER_TEXT;

        void draw();
    };
}
