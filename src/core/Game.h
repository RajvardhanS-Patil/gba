#pragma once
#include "SceneManager.h"
#include "TimeManager.h"
#include "EventBus.h"

class Game {
public:
    Game();
    ~Game();

    void init();
    void run();
    void shutdown();

    static SceneManager& getSceneManager();
    static EventBus& getEventBus();
    static TimeManager& getTimeManager();

private:
    SceneManager sceneManager;
    TimeManager timeManager;
    EventBus eventBus;

    static Game* instance;
};
