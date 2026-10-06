#include "Game.h"
#include "Config.h"
#include "ui/MainMenu.h"
#include <raylib.h>
#include "../utils/Logger.h"

Game* Game::instance = nullptr;

Game::Game() {
    instance = this;
}

Game::~Game() {
    if (instance == this) {
        instance = nullptr;
    }
}

void Game::init() {
    LOG_INFO("Initializing Game...");
    Config& config = Config::getInstance();
    config.load("data/config.json");

    if (config.isFullscreen()) {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
    }

    InitWindow(config.getWindowWidth(), config.getWindowHeight(), "RUNTIME: The Last Algorithm");
    SetTargetFPS(config.getTargetFPS());
    
    InitAudioDevice();
    SetMasterVolume(config.getMasterVolume());

    LOG_INFO("Pushing MainMenu scene.");
    sceneManager.changeScene(std::make_unique<MainMenu>());
}

void Game::run() {
    while (!WindowShouldClose()) {
        timeManager.update();
        float dt = timeManager.getDeltaTime();
        
        sceneManager.update(dt);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        sceneManager.draw();
        EndDrawing();
    }
}

void Game::shutdown() {
    LOG_INFO("Shutting down game...");
    CloseAudioDevice();
    CloseWindow();
}

SceneManager& Game::getSceneManager() { return instance->sceneManager; }
EventBus& Game::getEventBus() { return instance->eventBus; }
TimeManager& Game::getTimeManager() { return instance->timeManager; }
