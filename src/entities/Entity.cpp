#include "Entity.h"

Entity::Entity(std::string id, float x, float y) : id_(id), position_{x, y}, velocity_{0, 0}, bounds_{x, y, 32, 32}, active_(true) {}

Vector2 Entity::getPosition() const { return position_; }
void Entity::setPosition(Vector2 pos) { 
    position_ = pos; 
    bounds_.x = position_.x;
    bounds_.y = position_.y;
}

Vector2 Entity::getVelocity() const { return velocity_; }
void Entity::setVelocity(Vector2 vel) { velocity_ = vel; }

Rectangle Entity::getBounds() const { return bounds_; }
void Entity::setBounds(Rectangle b) { bounds_ = b; }

bool Entity::isActive() const { return active_; }
void Entity::setActive(bool a) { active_ = a; }

std::string Entity::getId() const { return id_; }
