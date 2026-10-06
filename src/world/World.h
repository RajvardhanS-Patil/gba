#pragma once
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include "Room.h"
#include "../entities/Player.h"

class World {
public:
    World();
    ~World() = default;

    void loadRoom(const std::string& roomId, const nlohmann::json& data);
    void update(float dt);
    void draw();

    void setPlayer(Player* player);
    Room* getCurrentRoom() const;

private:
    std::unique_ptr<Room> currentRoom_;
    Camera2D camera_;
    Vector2 cameraTarget_;
    Player* playerRef_;
};
