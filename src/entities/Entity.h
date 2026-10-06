#pragma once
#include <string>
#include <raylib.h>

class Entity {
public:
    Entity(std::string id, float x, float y);
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual std::string getType() const = 0;

    Vector2 getPosition() const;
    void setPosition(Vector2 pos);

    Vector2 getVelocity() const;
    void setVelocity(Vector2 vel);

    Rectangle getBounds() const;
    void setBounds(Rectangle b);

    bool isActive() const;
    void setActive(bool a);
    
    std::string getId() const;

protected:
    std::string id_;
    Vector2 position_;
    Vector2 velocity_;
    Rectangle bounds_;
    bool active_;
};
