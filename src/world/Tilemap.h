#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct WarpPoint {
    int x;
    int y;
    std::string targetMap;
    int targetX;
    int targetY;
};

struct MapInteractable {
    int x;
    int y;
    std::string type;
    std::string text;
};

class Tilemap {
public:
    Tilemap();
    ~Tilemap();

    bool load(const std::string& filepath);
    void draw(const Camera2D& camera) const;

    bool isSolid(int tileX, int tileY) const;
    bool isTallGrass(int tileX, int tileY) const;
    const WarpPoint* checkWarp(int tileX, int tileY) const;
    const MapInteractable* checkInteractable(int tileX, int tileY) const;

    int getGroundTile(int tileX, int tileY) const;
    void setTile(int tileX, int tileY, int groundTileId, int collisionVal);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    std::string getName() const { return name_; }

private:
    std::string name_;
    int width_ = 0;
    int height_ = 0;
    int tileSize_ = 16;
    std::vector<int> groundLayer_;
    std::vector<int> collisionLayer_;
    std::vector<WarpPoint> warps_;
    std::vector<MapInteractable> interactables_;

    Texture2D tileset_{0};
    bool tilesetLoaded_ = false;
};
