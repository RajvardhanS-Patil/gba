#pragma once
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <raylib.h>

struct SaveData {
    int chapter = 1;
    std::string currentRoomId = "memory_district";
    
    int playerHealth = 100;
    int playerEnergy = 100;
    int playerXP = 0;
    int playerLevel = 1;
    Vector2 playerPosition = {0.0f, 0.0f};
    
    std::map<std::string, float> dsaMastery;
    std::vector<std::string> unlockedAbilities;
    std::vector<std::string> completedQuests;
    std::vector<std::string> discoveredDialogue;
    std::vector<std::string> collectedItems;
    
    float masterVolume = 1.0f;
    float musicVolume = 1.0f;
    float sfxVolume = 1.0f;
    float voiceVolume = 1.0f;
    float textSpeed = 30.0f;
    
    std::string timestamp;

    nlohmann::json toJson() const;
    static SaveData fromJson(const nlohmann::json& j);
};
