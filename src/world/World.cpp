#include "World.h"

World::World() : playerRef_(nullptr) {
    camera_.target = {0.0f, 0.0f};
    camera_.offset = {static_cast<float>(GetScreenWidth()/2), static_cast<float>(GetScreenHeight()/2)};
    camera_.rotation = 0.0f;
    camera_.zoom = 1.0f;
}

void World::loadRoom(const std::string& roomId, const nlohmann::json& data) {
    int w = data.contains("width") ? (int)data["width"] : 20;
    int h = data.contains("height") ? (int)data["height"] : 15;
    currentRoom_ = std::make_unique<Room>(roomId, "Room", w, h);
    currentRoom_->loadFromJson(data);
}

void World::update(float dt) {
    if (currentRoom_) {
        currentRoom_->update(dt);
    }
    if (playerRef_) {
        cameraTarget_ = playerRef_->getPosition();
        camera_.target.x += (cameraTarget_.x - camera_.target.x) * 5.0f * dt;
        camera_.target.y += (cameraTarget_.y - camera_.target.y) * 5.0f * dt;
    }
}

void World::draw() {
    if (!currentRoom_) return;
    BeginMode2D(camera_);
    currentRoom_->draw();
    if (playerRef_) {
        playerRef_->draw();
    }
    EndMode2D();
}

void World::setPlayer(Player* player) { playerRef_ = player; }
Room* World::getCurrentRoom() const { return currentRoom_.get(); }
