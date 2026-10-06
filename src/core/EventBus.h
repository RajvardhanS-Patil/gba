#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <string>
#include <any>

struct DialogueStartEvent { std::string nodeId; };
struct DialogueEndEvent {};
struct QuestUpdateEvent { std::string questId; };
struct DSAUnlockEvent { std::string conceptName; };
struct CombatStartEvent { std::string enemyId; };
struct CombatEndEvent { bool playerWon; };
struct SceneChangeEvent { std::string sceneName; };

class EventBus {
public:
    using SubscriptionId = size_t;

    template<typename EventType>
    SubscriptionId subscribe(std::function<void(const EventType&)> callback) {
        std::type_index typeIdx(typeid(EventType));
        SubscriptionId id = nextId++;
        
        handlers[typeIdx].push_back({id, [callback](const std::any& ev) {
            callback(std::any_cast<const EventType&>(ev));
        }});
        
        return id;
    }

    void unsubscribe(SubscriptionId id);

    template<typename EventType>
    void publish(const EventType& event) {
        std::type_index typeIdx(typeid(EventType));
        auto it = handlers.find(typeIdx);
        if (it != handlers.end()) {
            for (const auto& handler : it->second) {
                handler.func(event);
            }
        }
    }

private:
    SubscriptionId nextId = 0;
    
    struct Handler {
        SubscriptionId id;
        std::function<void(const std::any&)> func;
    };
    
    std::unordered_map<std::type_index, std::vector<Handler>> handlers;
};
