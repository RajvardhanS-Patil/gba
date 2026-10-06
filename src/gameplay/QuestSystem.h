#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

enum class QuestState { INACTIVE, ACTIVE, COMPLETED, FAILED };

struct Objective {
    std::string description;
    bool completed;
    std::string type;
};

struct Quest {
    std::string id;
    std::string name;
    std::string description;
    QuestState state;
    std::vector<Objective> objectives;
    std::string rewardItemId;
    int rewardXP;
};

class QuestSystem {
public:
    QuestSystem() = default;
    ~QuestSystem() = default;

    void addQuest(const Quest& quest);
    void activateQuest(const std::string& id);
    void completeObjective(const std::string& questId, int objIndex);
    
    bool isQuestComplete(const std::string& id) const;
    std::vector<Quest*> getActiveQuests();
    Quest* getQuest(const std::string& id);

    void loadFromJson(const nlohmann::json& data);

private:
    std::vector<Quest> quests_;
};
