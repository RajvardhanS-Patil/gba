#include "Interactable.h"
#include "../utils/MathUtils.h"
#include "../utils/Logger.h"
#include "ui/UISystem.h"
#include <cmath>

static Texture2D s_nodeTexture = {0};
static bool s_nodeTextureLoaded = false;

Interactable::Interactable(std::string id, std::string type, float x, float y, float w, float h)
    : id_(id), position_{x, y}, bounds_{x, y, w, h}, type_(type), activated_(false), linkedAction_("") 
{
    if (!s_nodeTextureLoaded && IsWindowReady()) {
        if (FileExists("assets/props/memory_node.png")) {
            s_nodeTexture = LoadTexture("assets/props/memory_node.png");
            s_nodeTextureLoaded = true;
        }
    }
}

void Interactable::interact() {
    activated_ = !activated_;
    LOG_INFO("Interacted with " + id_);
}

void Interactable::draw() {
    float time = (float)GetTime();
    float pulse = (sinf(time * 4.0f) + 1.0f) * 0.5f;

    Color aura = activated_ ? CYBER_SUCCESS : (type_ == "MEMORY_NODE" ? CYBER_CYAN : CYBER_WARNING);

    // Glowing base ring
    DrawEllipse((int)(position_.x + bounds_.width/2), (int)(position_.y + bounds_.height - 6), (int)(bounds_.width/2.2f), 8, Color{0, 0, 0, 100});
    DrawCircleGradient((int)(position_.x + bounds_.width/2), (int)(position_.y + bounds_.height/2), bounds_.width * 0.8f, 
                       Color{aura.r, aura.g, aura.b, (unsigned char)(60 + pulse * 40)}, Color{aura.r, aura.g, aura.b, 0});

    if (s_nodeTextureLoaded && s_nodeTexture.id > 0 && type_ == "MEMORY_NODE") {
        DrawTexturePro(s_nodeTexture, 
                       Rectangle{0, 0, 48, 48}, 
                       Rectangle{position_.x, position_.y, bounds_.width, bounds_.height}, 
                       Vector2{0, 0}, 0.0f, WHITE);
    } else {
        DrawRectangleRounded(bounds_, 0.25f, 6, CYBER_PANEL);
        DrawRectangleRoundedLinesEx(bounds_, 0.25f, 6, 2.0f, aura);
    }

    // Label on top
    int tw = MeasureText(id_.c_str(), 11);
    DrawRectangle((int)(position_.x + bounds_.width/2 - tw/2 - 4), (int)(position_.y - 18), tw + 8, 14, Color{10, 14, 23, 210});
    DrawText(id_.c_str(), (int)(position_.x + bounds_.width/2 - tw/2), (int)(position_.y - 17), 11, aura);
}

bool Interactable::isActivated() const { return activated_; }
std::string Interactable::getLinkedAction() const { return linkedAction_; }

bool Interactable::isInRange(Vector2 playerPos, float range) const {
    Vector2 center = {position_.x + bounds_.width/2, position_.y + bounds_.height/2};
    return MathUtils::distance(center, playerPos) <= range;
}
