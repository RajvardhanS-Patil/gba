#pragma once
#include "SaveData.h"
#include <optional>
#include <string>

class SaveManager {
public:
    SaveManager();
    ~SaveManager() = default;

    void save(int slot, const SaveData& data);
    std::optional<SaveData> load(int slot);
    void deleteSave(int slot);
    bool hasSave(int slot) const;
    
    std::string getSaveInfo(int slot) const;

private:
    std::string getSavePath(int slot) const;
    const std::string SAVE_DIR = "data/saves/";
};
