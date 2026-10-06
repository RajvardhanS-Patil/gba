#include "Room.h"
#include "../utils/MathUtils.h"

Room::Room(std::string id, std::string name, int w, int h)
    : id_(id), name_(name), width_(w), height_(h), tileSize_(32), floorColor_(DARKGRAY), wallColor_(DARKBLUE), accentColor_(PURPLE) {
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
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            Rectangle rect = {static_cast<float>(x * tileSize_), static_cast<float>(y * tileSize_), static_cast<float>(tileSize_), static_cast<float>(tileSize_)};
            if (tileMap_[y][x] == 1) DrawRectangleRec(rect, wallColor_);
            else if (tileMap_[y][x] == 2) DrawRectangleRec(rect, accentColor_);
            else DrawRectangleRec(rect, floorColor_);
        }
    }
    
    for (auto& interactable : interactables_) {
        interactable->draw();
    }
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
    int x = static_cast<int>(worldX) / tileSize_;
    int y = static_cast<int>(worldY) / tileSize_;
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return -1;
    return tileMap_[y][x];
}
