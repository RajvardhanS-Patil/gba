#pragma once
#include <string>
#include <unordered_map>

class TimeManager {
public:
    TimeManager();
    ~TimeManager() = default;

    void update();
    
    float getDeltaTime() const;
    float getTotalTime() const;

    void createTimer(const std::string& id, float duration);
    bool isTimerDone(const std::string& id) const;
    void resetTimer(const std::string& id);
    void removeTimer(const std::string& id);

private:
    float deltaTime;
    float totalTime;
    
    struct Timer {
        float duration;
        float elapsed;
        bool done;
    };
    std::unordered_map<std::string, Timer> timers;
};
