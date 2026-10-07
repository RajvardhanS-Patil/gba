#include "Tilemap.h"
#include "core/GBAEngine.h"
#include "utils/Logger.h"
#include <fstream>

Tilemap::Tilemap() {
    if (FileExists("assets/tilesets/overworld16.png")) {
        tileset_ = LoadTexture("assets/tilesets/overworld16.png");
        tilesetLoaded_ = true;
    }
}

Tilemap::~Tilemap() {
    if (tilesetLoaded_ && tileset_.id > 0) {
        UnloadTexture(tileset_);
        tilesetLoaded_ = false;
    }
}

bool Tilemap::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("Could not open map file: " + filepath);
        return false;
    }

    try {
        nlohmann::json j;
        file >> j;

        name_ = j.value("name", "Unknown Area");
        width_ = j.value("width", 20);
        height_ = j.value("height", 15);
        tileSize_ = j.value("tileSize", 16);

        groundLayer_ = j["ground"].get<std::vector<int>>();
        collisionLayer_ = j["collision"].get<std::vector<int>>();

        warps_.clear();
        if (j.contains("warps")) {
            for (const auto& w : j["warps"]) {
                warps_.push_back({
                    w["x"].get<int>(),
                    w["y"].get<int>(),
                    w["targetMap"].get<std::string>(),
                    w["targetX"].get<int>(),
                    w["targetY"].get<int>()
                });
            }
        }

        interactables_.clear();
        if (j.contains("interactables")) {
            for (const auto& it : j["interactables"]) {
                interactables_.push_back({
                    it["x"].get<int>(),
                    it["y"].get<int>(),
                    it["type"].get<std::string>(),
                    it["text"].get<std::string>()
                });
            }
        }

        LOG_INFO("Loaded GBA Tilemap: " + name_ + TextFormat(" (%dx%d)", width_, height_));
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("Error parsing map JSON: ") + e.what());
        return false;
    }
}

void Tilemap::draw(const Camera2D& camera) const {
    (void)camera;
    if (!tilesetLoaded_ || tileset_.id == 0) return;

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            int idx = y * width_ + x;
            if (idx >= static_cast<int>(groundLayer_.size())) continue;
            int tileId = groundLayer_[idx];

            int tx = tileId % 8;
            int ty = tileId / 8;

            Rectangle srcRec = {
                static_cast<float>(tx * tileSize_),
                static_cast<float>(ty * tileSize_),
                static_cast<float>(tileSize_),
                static_cast<float>(tileSize_)
            };
            Vector2 pos = {
                static_cast<float>(x * tileSize_),
                static_cast<float>(y * tileSize_)
            };

            DrawTextureRec(tileset_, srcRec, pos, WHITE);
        }
    }
}

bool Tilemap::isSolid(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= width_ || tileY < 0 || tileY >= height_) return true;
    int idx = tileY * width_ + tileX;
    if (idx >= static_cast<int>(collisionLayer_.size())) return true;
    return collisionLayer_[idx] != 0;
}

bool Tilemap::isTallGrass(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= width_ || tileY < 0 || tileY >= height_) return false;
    int idx = tileY * width_ + tileX;
    if (idx >= static_cast<int>(groundLayer_.size())) return false;
    return groundLayer_[idx] == 1; // Tile ID 1 is wild grass
}

const WarpPoint* Tilemap::checkWarp(int tileX, int tileY) const {
    for (const auto& w : warps_) {
        if (w.x == tileX && w.y == tileY) return &w;
    }
    return nullptr;
}

const MapInteractable* Tilemap::checkInteractable(int tileX, int tileY) const {
    for (const auto& it : interactables_) {
        if (it.x == tileX && it.y == tileY) return &it;
    }
    return nullptr;
}

int Tilemap::getGroundTile(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= width_ || tileY < 0 || tileY >= height_) return -1;
    int idx = tileY * width_ + tileX;
    if (idx >= static_cast<int>(groundLayer_.size())) return -1;
    return groundLayer_[idx];
}

void Tilemap::setTile(int tileX, int tileY, int groundTileId, int collisionVal) {
    if (tileX < 0 || tileX >= width_ || tileY < 0 || tileY >= height_) return;
    int idx = tileY * width_ + tileX;
    if (idx < static_cast<int>(groundLayer_.size())) {
        groundLayer_[idx] = groundTileId;
    }
    if (idx < static_cast<int>(collisionLayer_.size())) {
        collisionLayer_[idx] = collisionVal;
    }
}
