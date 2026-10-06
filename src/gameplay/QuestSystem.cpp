#include "QuestSystem.h"

void QuestSystem::addQuest(const Quest& quest) {
    quests_.push_back(quest);
}

void QuestSystem::activateQuest(const std::string& id) {
    Quest* q = getQuest(id);
    if (q && q->state == QuestState::INACTIVE) {
        q->state = QuestState::ACTIVE;
    }
}

void QuestSystem::completeObjective(const std::string& questId, int objIndex) {
    Quest* q = getQuest(questId);
    if (q && objIndex >= 0 && objIndex < q->objectives.size()) {
        q->objectives[objIndex].completed = true;
        
        bool allDone = true;
        for (const auto& obj : q->objectives) {
            if (!obj.completed) allDone = false;
        }
        if (allDone) {
            q->state = QuestState::COMPLETED;
        }
    }
}

bool QuestSystem::isQuestComplete(const std::string& id) const {
    for (const auto& q : quests_) {
        if (q.id == id) return q.state == QuestState::COMPLETED;
    }
    return false;
}

std::vector<Quest*> QuestSystem::getActiveQuests() {
    std::vector<Quest*> active;
    for (auto& q : quests_) {
        if (q.state == QuestState::ACTIVE) active.push_back(&q);
    }
    return active;
}

Quest* QuestSystem::getQuest(const std::string& id) {
    for (auto& q : quests_) {
        if (q.id == id) return &q;
    }
    return nullptr;
}

void QuestSystem::loadFromJson(const nlohmann::json& data) {
    if (data.contains("quests")) {
        for (const auto& qData : data["quests"]) {
            Quest q;
            q.id = qData["id"];
            q.name = qData["name"];
            q.description = qData["description"];
            q.state = QuestState::INACTIVE;
            q.rewardItemId = qData.value("rewardItemId", "");
            q.rewardXP = qData.value("rewardXP", 0);
            
            if (qData.contains("objectives")) {
                for (const auto& oData : qData["objectives"]) {
                    q.objectives.push_back({oData["description"], false, oData["type"]});
                }
            }
            quests_.push_back(q);
        }
    }
}
