#include "DialogueNode.h"

DialogueNode DialogueNode::fromJson(const nlohmann::json& j) {
    DialogueNode node;
    node.id = j.value("id", "");
    node.speaker = j.value("speaker", "Unknown");
    node.text = j.value("text", "");
    node.expression = j.value("expression", "neutral");
    node.voiceId = j.value("voiceId", "");
    node.nextNodeId = j.value("nextNodeId", "");
    node.action = j.value("action", "");

    if (j.contains("choices")) {
        for (const auto& cData : j["choices"]) {
            DialogueChoice choice;
            choice.text = cData.value("text", "");
            choice.nextNodeId = cData.value("nextNodeId", "");
            choice.condition = cData.value("condition", "");
            node.choices.push_back(choice);
        }
    }
    return node;
}

nlohmann::json DialogueNode::toJson() const {
    nlohmann::json j;
    j["id"] = id;
    j["speaker"] = speaker;
    j["text"] = text;
    j["expression"] = expression;
    j["voiceId"] = voiceId;
    j["nextNodeId"] = nextNodeId;
    j["action"] = action;

    nlohmann::json choicesArray = nlohmann::json::array();
    for (const auto& c : choices) {
        choicesArray.push_back({
            {"text", c.text},
            {"nextNodeId", c.nextNodeId},
            {"condition", c.condition}
        });
    }
    j["choices"] = choicesArray;
    return j;
}
