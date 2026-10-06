#pragma once
#include <string>
#include <vector>

struct Item {
    std::string id;
    std::string name;
    std::string description;
    std::string icon;
    int quantity;
};

class InventorySystem {
public:
    InventorySystem() = default;
    ~InventorySystem() = default;

    void addItem(const Item& item);
    void removeItem(const std::string& id);
    bool hasItem(const std::string& id) const;

    const std::vector<Item>& getItems() const;
    std::vector<Item> getDSACores() const;

private:
    std::vector<Item> items_;
};
