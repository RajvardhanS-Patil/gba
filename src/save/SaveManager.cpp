#include "SaveManager.h"
#include "../utils/Logger.h"
#include <fstream>
#include <filesystem>

SaveManager::SaveManager() {
    if (!std::filesystem::exists(SAVE_DIR)) {
        std::filesystem::create_directories(SAVE_DIR);
    }
}

std::string SaveManager::getSavePath(int slot) const {
    return SAVE_DIR + "save_" + std::to_string(slot) + ".json";
}

void SaveManager::save(int slot, const SaveData& data) {
    try {
        nlohmann::json j = data.toJson();
        std::ofstream file(getSavePath(slot));
        if (file.is_open()) {
            file << j.dump(4);
            LOG_INFO("Saved game to slot " + std::to_string(slot));
        } else {
            LOG_ERROR("Failed to open save file for writing: slot " + std::to_string(slot));
        }
    } catch (const std::exception& e) {
        LOG_ERROR("Error saving game: " + std::string(e.what()));
    }
}

std::optional<SaveData> SaveManager::load(int slot) {
    if (!hasSave(slot)) return std::nullopt;

    try {
        std::ifstream file(getSavePath(slot));
        if (file.is_open()) {
            nlohmann::json j;
            file >> j;
            LOG_INFO("Loaded game from slot " + std::to_string(slot));
            return SaveData::fromJson(j);
        }
    } catch (const std::exception& e) {
        LOG_ERROR("Error loading game (corrupted file): " + std::string(e.what()));
    }
    return std::nullopt;
}

void SaveManager::deleteSave(int slot) {
    std::string path = getSavePath(slot);
    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
        LOG_INFO("Deleted save slot " + std::to_string(slot));
    }
}

bool SaveManager::hasSave(int slot) const {
    return std::filesystem::exists(getSavePath(slot));
}

std::string SaveManager::getSaveInfo(int slot) const {
    if (!hasSave(slot)) return "Empty Slot";

    auto dataOpt = const_cast<SaveManager*>(this)->load(slot);
    if (dataOpt) {
        const SaveData& data = dataOpt.value();
        return "Chapter " + std::to_string(data.chapter) + " - " + data.timestamp;
    }
    return "Corrupted Save";
}
