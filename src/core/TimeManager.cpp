#include "TimeManager.h"
#include <raylib.h>

TimeManager::TimeManager() : deltaTime(0.0f), totalTime(0.0f) {}

void TimeManager::update() {
    deltaTime = GetFrameTime();
    totalTime += deltaTime;

    for (auto& pair : timers) {
        if (!pair.second.done) {
            pair.second.elapsed += deltaTime;
            if (pair.second.elapsed >= pair.second.duration) {
                pair.second.done = true;
            }
        }
    }
}

float TimeManager::getDeltaTime() const {
    return deltaTime;
}

float TimeManager::getTotalTime() const {
    return totalTime;
}

void TimeManager::createTimer(const std::string& id, float duration) {
    timers[id] = {duration, 0.0f, false};
}

bool TimeManager::isTimerDone(const std::string& id) const {
    auto it = timers.find(id);
    if (it != timers.end()) {
        return it->second.done;
    }
    return false;
}

void TimeManager::resetTimer(const std::string& id) {
    auto it = timers.find(id);
    if (it != timers.end()) {
        it->second.elapsed = 0.0f;
        it->second.done = false;
    }
}

void TimeManager::removeTimer(const std::string& id) {
    timers.erase(id);
}
