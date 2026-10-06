#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct DialogueChoice {
    std::string text;
    std::string nextNodeId;
    std::string condition;
};

class DialogueNode {
public:
    std::string id;
    std::string speaker;
    std::string text;
    std::string expression;
    std::string voiceId;
    std::string nextNodeId;
    std::vector<DialogueChoice> choices;
    std::string action;

    static DialogueNode fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;
};
