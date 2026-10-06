#pragma once
#include <string>
#include <map>
#include <nlohmann/json.hpp>

struct DSAMastery {
    std::string conceptName;
    float masteryPercent;
    int challengesCompleted;
    int challengesTotal;
    bool unlocked;
    std::string tier;
};

class ProgressionSystem {
public:
    ProgressionSystem();
    ~ProgressionSystem() = default;

    void initConcept(const std::string& name, int totalChallenges);
    void recordSuccess(const std::string& concept);
    void recordFailure(const std::string& concept);

    DSAMastery getMastery(const std::string& concept) const;
    std::map<std::string, DSAMastery> getAllMasteries() const;

    void addXP(int amount);
    int getXP() const;
    int getLevel() const;

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);

private:
    std::string computeTier(float percent) const;
    
    std::map<std::string, DSAMastery> masteryMap_;
    int playerXP_;
    int playerLevel_;
};
