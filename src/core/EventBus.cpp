#include "EventBus.h"
#include <algorithm>

void EventBus::unsubscribe(SubscriptionId id) {
    for (auto& pair : handlers) {
        auto& vec = pair.second;
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [id](const Handler& h) { return h.id == id; }), vec.end());
    }
}
