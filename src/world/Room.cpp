#include "Room.h"
#include "../utils/MathUtils.h"
#include "ui/UISystem.h"
#include <cmath>

Room::Room(std::string id, std::string name, int w, int h)
    : id_(id), name_(name), width_(w), height_(h), tileSize_(32), 
      floorColor_(Color{16, 20, 32, 255}), 
      wallColor_(Color{30, 38, 58, 255}), 
      accentColor_(Color{45, 25, 65, 255}) 
{
    tileMap_.resize(height_, std::vector<int>(width_, 0));
}

void Room::loadFromJson(const nlohmann::json& data) {
    if (data.contains("map")) {
        auto mapData = data["map"];
        for (int y = 0; y < height_ && static_cast<size_t>(y) < mapData.size(); ++y) {
            for (int x = 0; x < width_ && static_cast<size_t>(x) < mapData[y].size(); ++x) {
                tileMap_[y][x] = mapData[y][x];
            }
        }
    }
}

void Room::update(float dt) {
    for (auto& entity : entities_) {
        entity->update(dt);
    }
}

void Room::draw() {
    float time = (float)GetTime();

    // 1. Draw stylish cyber grid floor and walls
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            Rectangle rect = {
                static_cast<float>(x * tileSize_), 
                static_cast<float>(y * tileSize_), 
                static_cast<float>(tileSize_), 
                static_cast<float>(tileSize_)
            };
            int tileType = tileMap_[y][x];

            if (tileType == 1) {
                // Cyber Wall Block with beveled corner
                DrawRectangleRec(rect, wallColor_);
                DrawRectangleLinesEx(rect, 1.0f, Color{50, 65, 95, 255});
                // Wall tech detail
                DrawLine(rect.x + 4, rect.y + 4, rect.x + tileSize_ - 4, rect.y + 4, Color{70, 90, 130, 180});
            } else if (tileType == 2) {
                // Hazard / Energy conduit tile
                DrawRectangleRec(rect, accentColor_);
                float pulse = (sinf(time * 3.0f + (x + y)*0.5f) + 1.0f) * 0.5f;
                DrawRectangleLinesEx(rect, 1.5f, Color{180, 50, 240, (unsigned char)(100 + pulse * 120)});
                DrawLine(rect.x + tileSize_/2, rect.y, rect.x + tileSize_/2, rect.y + tileSize_, Color{0, 229, 255, (unsigned char)(80 + pulse * 100)});
            } else {
                // High-tech floor panel
                DrawRectangleRec(rect, floorColor_);
                DrawRectangleLinesEx(rect, 1.0f, Color{24, 30, 48, 255});
                // Floor circuit dots every few tiles
                if ((x * 7 + y * 13) % 11 == 0) {
                    DrawCircle(rect.x + tileSize_/2, rect.y + tileSize_/2, 1.5f, Color{0, 229, 255, 60});
                }
            }
        }
    }

    // 2. Draw memory bus data lines on the floor connecting nodes
    DrawLineEx(Vector2{340.0f, 330.0f}, Vector2{1020.0f, 330.0f}, 3.0f, Color{0, 229, 255, 70});
    // Animated traveling packet on the bus
    float packetX = 350.0f + fmodf(time * 180.0f, 650.0f);
    DrawCircle((int)packetX, 330, 4.0f, CYBER_CYAN);
    DrawCircleGradient((int)packetX, 330, 12.0f, Color{0, 229, 255, 120}, Color{0, 229, 255, 0});
    
    // 3. Draw interactables
    for (auto& interactable : interactables_) {
        interactable->draw();
    }

    // 4. Draw entities
    for (auto& entity : entities_) {
        entity->draw();
    }
}

void Room::addEntity(std::unique_ptr<Entity> entity) {
    entities_.push_back(std::move(entity));
}

void Room::addInteractable(std::unique_ptr<Interactable> interactable) {
    interactables_.push_back(std::move(interactable));
}

bool Room::isWalkable(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= width_ || tileY < 0 || tileY >= height_) return false;
    return tileMap_[tileY][tileX] != 1;
}

std::vector<Entity*> Room::getEntitiesInRange(Vector2 pos, float range) {
    std::vector<Entity*> inRange;
    for (auto& e : entities_) {
        if (MathUtils::distance(e->getPosition(), pos) <= range) {
            inRange.push_back(e.get());
        }
    }
    return inRange;
}

int Room::getTileAt(float worldX, float worldY) const {
    int tx = static_cast<int>(worldX / tileSize_);
    int ty = static_cast<int>(worldY / tileSize_);
    if (tx < 0 || tx >= width_ || ty < 0 || ty >= height_) return 1;
    return tileMap_[ty][tx];
}
