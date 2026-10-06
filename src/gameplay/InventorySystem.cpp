#include "InventorySystem.h"
#include <algorithm>

void InventorySystem::addItem(const Item& item) {
    for (auto& i : items_) {
        if (i.id == item.id) {
            i.quantity += item.quantity;
            return;
        }
    }
    items_.push_back(item);
}

void InventorySystem::removeItem(const std::string& id) {
    items_.erase(std::remove_if(items_.begin(), items_.end(),
        [&id](const Item& item) { return item.id == id; }), items_.end());
}

bool InventorySystem::hasItem(const std::string& id) const {
    for (const auto& item : items_) {
        if (item.id == id && item.quantity > 0) return true;
    }
    return false;
}

const std::vector<Item>& InventorySystem::getItems() const {
    return items_;
}

std::vector<Item> InventorySystem::getDSACores() const {
    std::vector<Item> cores;
    for (const auto& item : items_) {
        if (item.id.find("_CORE") != std::string::npos) {
            cores.push_back(item);
        }
    }
    return cores;
}
