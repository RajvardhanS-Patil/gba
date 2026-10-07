#include "NPC.h"
#include "../utils/MathUtils.h"
#include "ui/UISystem.h"
#include <cmath>

static Texture2D s_novaTexture = {0};
static bool s_novaTextureLoaded = false;

NPC::NPC(std::string id, std::string name, float x, float y, std::string dialogueId)
    : Character(id, name, x, y, 100, 100), dialogueId_(dialogueId), interactionRadius_(75.0f), hasInteracted_(false) 
{
    bounds_ = {x, y, 28.0f, 36.0f};
    if (!s_novaTextureLoaded && IsWindowReady()) {
        if (FileExists("assets/characters/nova.png")) {
            s_novaTexture = LoadTexture("assets/characters/nova.png");
            s_novaTextureLoaded = true;
        }
    }
}

void NPC::update(float dt) {
    setAnimState(AnimState::IDLE);
    Character::update(dt);
}

void NPC::draw() {
    float time = (float)GetTime();
    // Floating hover animation for companion bot
    float hover = sinf(time * 3.5f) * 6.0f;

    // Soft energy glow under floating bot
    DrawEllipse((int)(position_.x + 14), (int)(position_.y + 40), 10, 4, Color{0, 0, 0, 70});
    DrawCircleGradient((int)(position_.x + 14), (int)(position_.y + 18 + hover), 18.0f, Color{180, 120, 255, 75}, Color{180, 120, 255, 0});

    if (s_novaTextureLoaded && s_novaTexture.id > 0) {
        DrawTexture(s_novaTexture, (int)position_.x, (int)(position_.y + hover), WHITE);
    } else {
        DrawCircle((int)(position_.x + 14), (int)(position_.y + 18 + hover), 14, CYBER_VIOLET);
        DrawCircle((int)(position_.x + 14), (int)(position_.y + 18 + hover), 7, WHITE);
    }

    // Overhead nameplate
    int nw = MeasureText(name_.c_str(), 12);
    DrawRectangle((int)(position_.x + 14 - nw/2 - 4), (int)(position_.y - 14 + hover), nw + 8, 14, Color{10, 14, 23, 200});
    DrawText(name_.c_str(), (int)(position_.x + 14 - nw/2), (int)(position_.y - 13 + hover), 12, CYBER_VIOLET);
}

std::string NPC::getType() const { return "NPC"; }

std::string NPC::getDialogueId() const { return dialogueId_; }

bool NPC::isInRange(Vector2 playerPos) const {
    Vector2 center = {position_.x + bounds_.width/2, position_.y + bounds_.height/2};
    float dist = MathUtils::distance(center, playerPos);
    return dist <= interactionRadius_;
}
