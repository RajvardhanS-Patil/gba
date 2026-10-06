#pragma once
#include <string>
#include <raylib.h>

enum class InteractableType { MEMORY_NODE, TERMINAL, DATA_CORE, DOOR, ITEM };

class Interactable {
public:
    Interactable(std::string id, std::string type, float x, float y, float w, float h);
    virtual ~Interactable() = default;

    virtual void interact();
    virtual void draw();

    bool isActivated() const;
    std::string getLinkedAction() const;
    bool isInRange(Vector2 playerPos, float range) const;

protected:
    std::string id_;
    Vector2 position_;
    Rectangle bounds_;
    std::string type_;
    bool activated_;
    std::string linkedAction_;
};
