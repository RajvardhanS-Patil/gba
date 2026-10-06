#include "ProgressionSystem.h"

ProgressionSystem::ProgressionSystem() : playerXP_(0), playerLevel_(1) {}

void ProgressionSystem::initConcept(const std::string& name, int totalChallenges) {
    if (masteryMap_.find(name) == masteryMap_.end()) {
        masteryMap_[name] = {name, 0.0f, 0, totalChallenges, false, "APPRENTICE"};
    }
}

void ProgressionSystem::recordSuccess(const std::string& concept) {
    auto it = masteryMap_.find(concept);
    if (it != masteryMap_.end()) {
        it->second.challengesCompleted++;
        it->second.masteryPercent = (static_cast<float>(it->second.challengesCompleted) / it->second.challengesTotal) * 100.0f;
        if (it->second.masteryPercent > 100.0f) it->second.masteryPercent = 100.0f;
        it->second.tier = computeTier(it->second.masteryPercent);
        it->second.unlocked = true;
    }
}

void ProgressionSystem::recordFailure(const std::string& concept) {
    auto it = masteryMap_.find(concept);
    if (it != masteryMap_.end()) {
        it->second.masteryPercent -= 5.0f;
        if (it->second.masteryPercent < 0.0f) it->second.masteryPercent = 0.0f;
        it->second.tier = computeTier(it->second.masteryPercent);
    }
}

DSAMastery ProgressionSystem::getMastery(const std::string& concept) const {
    auto it = masteryMap_.find(concept);
    if (it != masteryMap_.end()) return it->second;
    return {"", 0.0f, 0, 0, false, ""};
}

std::map<std::string, DSAMastery> ProgressionSystem::getAllMasteries() const {
    return masteryMap_;
}

void ProgressionSystem::addXP(int amount) {
    playerXP_ += amount;
    playerLevel_ = 1 + (playerXP_ / 1000);
}

int ProgressionSystem::getXP() const { return playerXP_; }
int ProgressionSystem::getLevel() const { return playerLevel_; }

std::string ProgressionSystem::computeTier(float percent) const {
    if (percent < 33.0f) return "APPRENTICE";
    if (percent < 66.0f) return "OPERATOR";
    return "MASTER";
}

nlohmann::json ProgressionSystem::toJson() const {
    nlohmann::json j;
    j["xp"] = playerXP_;
    j["level"] = playerLevel_;
    nlohmann::json masteryJson;
    for (const auto& pair : masteryMap_) {
        masteryJson[pair.first] = {
            {"percent", pair.second.masteryPercent},
            {"completed", pair.second.challengesCompleted},
            {"total", pair.second.challengesTotal},
            {"unlocked", pair.second.unlocked}
        };
    }
    j["mastery"] = masteryJson;
    return j;
}

void ProgressionSystem::fromJson(const nlohmann::json& j) {
    if (j.contains("xp")) playerXP_ = j["xp"];
    if (j.contains("level")) playerLevel_ = j["level"];
    if (j.contains("mastery")) {
        for (auto it = j["mastery"].begin(); it != j["mastery"].end(); ++it) {
            std::string name = it.key();
            auto data = it.value();
            float pct = data["percent"];
            masteryMap_[name] = {
                name, pct, data["completed"], data["total"], data["unlocked"], computeTier(pct)
            };
        }
    }
}
