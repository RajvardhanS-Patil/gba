#pragma once
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "../entities/Entity.h"
#include "Interactable.h"
#include <raylib.h>

class Room {
public:
    Room(std::string id, std::string name, int w, int h);
    ~Room() = default;

    void loadFromJson(const nlohmann::json& data);
    void update(float dt);
    void draw();

    void addEntity(std::unique_ptr<Entity> entity);
    void addInteractable(std::unique_ptr<Interactable> interactable);

    bool isWalkable(int tileX, int tileY) const;
    std::vector<Entity*> getEntitiesInRange(Vector2 pos, float range);
    int getTileAt(float worldX, float worldY) const;

private:
    std::string id_;
    std::string name_;
    int width_;
    int height_;
    int tileSize_;
    std::vector<std::vector<int>> tileMap_;
    std::vector<std::unique_ptr<Entity>> entities_;
    std::vector<std::unique_ptr<Interactable>> interactables_;
    Color floorColor_;
    Color wallColor_;
    Color accentColor_;
};
