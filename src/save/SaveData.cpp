#include "SaveData.h"

nlohmann::json SaveData::toJson() const {
    nlohmann::json j;
    j["chapter"] = chapter;
    j["currentRoomId"] = currentRoomId;
    
    j["player"] = {
        {"health", playerHealth},
        {"energy", playerEnergy},
        {"xp", playerXP},
        {"level", playerLevel},
        {"x", playerPosition.x},
        {"y", playerPosition.y}
    };
    
    j["dsaMastery"] = dsaMastery;
    j["unlockedAbilities"] = unlockedAbilities;
    j["completedQuests"] = completedQuests;
    j["discoveredDialogue"] = discoveredDialogue;
    j["collectedItems"] = collectedItems;
    
    j["settings"] = {
        {"masterVolume", masterVolume},
        {"musicVolume", musicVolume},
        {"sfxVolume", sfxVolume},
        {"voiceVolume", voiceVolume},
        {"textSpeed", textSpeed}
    };
    
    j["timestamp"] = timestamp;
    return j;
}

SaveData SaveData::fromJson(const nlohmann::json& j) {
    SaveData data;
    if (j.contains("chapter") && j["chapter"].is_number()) data.chapter = j["chapter"].get<int>();
    if (j.contains("currentRoomId") && j["currentRoomId"].is_string()) data.currentRoomId = j["currentRoomId"].get<std::string>();
    
    if (j.contains("player") && j["player"].is_object()) {
        auto p = j["player"];
        if (p.contains("health") && p["health"].is_number()) data.playerHealth = p["health"].get<int>();
        if (p.contains("energy") && p["energy"].is_number()) data.playerEnergy = p["energy"].get<int>();
        if (p.contains("xp") && p["xp"].is_number()) data.playerXP = p["xp"].get<int>();
        if (p.contains("level") && p["level"].is_number()) data.playerLevel = p["level"].get<int>();
        if (p.contains("x") && p["x"].is_number()) data.playerPosition.x = p["x"].get<float>();
        if (p.contains("y") && p["y"].is_number()) data.playerPosition.y = p["y"].get<float>();
    }
    
    if (j.contains("dsaMastery") && j["dsaMastery"].is_object()) {
        for (auto& [key, val] : j["dsaMastery"].items()) {
            if (val.is_number()) {
                data.dsaMastery[key] = val.get<float>();
            }
        }
    }
    
    if (j.contains("unlockedAbilities") && j["unlockedAbilities"].is_array()) {
        for (const auto& item : j["unlockedAbilities"]) {
            if (item.is_string()) data.unlockedAbilities.push_back(item.get<std::string>());
        }
    }
    if (j.contains("completedQuests") && j["completedQuests"].is_array()) {
        for (const auto& item : j["completedQuests"]) {
            if (item.is_string()) data.completedQuests.push_back(item.get<std::string>());
        }
    }
    if (j.contains("discoveredDialogue") && j["discoveredDialogue"].is_array()) {
        for (const auto& item : j["discoveredDialogue"]) {
            if (item.is_string()) data.discoveredDialogue.push_back(item.get<std::string>());
        }
    }
    if (j.contains("collectedItems") && j["collectedItems"].is_array()) {
        for (const auto& item : j["collectedItems"]) {
            if (item.is_string()) data.collectedItems.push_back(item.get<std::string>());
        }
    }
    
    if (j.contains("settings") && j["settings"].is_object()) {
        auto s = j["settings"];
        if (s.contains("masterVolume") && s["masterVolume"].is_number()) data.masterVolume = s["masterVolume"].get<float>();
        if (s.contains("musicVolume") && s["musicVolume"].is_number()) data.musicVolume = s["musicVolume"].get<float>();
        if (s.contains("sfxVolume") && s["sfxVolume"].is_number()) data.sfxVolume = s["sfxVolume"].get<float>();
        if (s.contains("voiceVolume") && s["voiceVolume"].is_number()) data.voiceVolume = s["voiceVolume"].get<float>();
        if (s.contains("textSpeed") && s["textSpeed"].is_number()) data.textSpeed = s["textSpeed"].get<float>();
    }
    
    if (j.contains("timestamp") && j["timestamp"].is_string()) data.timestamp = j["timestamp"].get<std::string>();
    return data;
}
