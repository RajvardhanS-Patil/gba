#include "Game.h"
#include "Config.h"
#include "GBAEngine.h"
#include "ui/GBAMainMenu.h"
#include "audio/AudioManager.h"
#include "audio/MusicManager.h"
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
    LOG_INFO("Initializing GBA Handheld Game Engine...");
    Config& config = Config::getInstance();
    config.load("data/config.json");

    int scale = 4; // 240x160 * 4 = 960x640 window
    InitWindow(GBA_SCREEN_WIDTH * scale, GBA_SCREEN_HEIGHT * scale, "RUNTIME: The Last Algorithm [GBA]");
    SetExitKey(KEY_NULL); // Prevent ESC from closing the entire game window
    SetTargetFPS(60);
    
    AudioManager::instance().init();
    SetMasterVolume(config.getMasterVolume());

    GBAEngine::instance().init(scale);

    LOG_INFO("Launching GBA Main Menu...");
    sceneManager.changeScene(std::make_unique<GBAMainMenu>());
}

void Game::run() {
    while (!WindowShouldClose()) {
        timeManager.update();
        float dt = timeManager.getDeltaTime();
        
        MusicManager::instance().update(dt);
        sceneManager.update(dt);
        
        // Render scene to native 240x160 GBA target
        GBAEngine::instance().beginFrame();
        sceneManager.draw();
        
        // Upscale with nearest-neighbor point filter to window, then draw high-res overlay
        GBAEngine::instance().endFrame([this]() {
            sceneManager.drawScreenOverlay();
        });
    }
}

void Game::shutdown() {
    LOG_INFO("Shutting down GBA game...");
    GBAEngine::instance().shutdown();
    AudioManager::instance().shutdown();
    CloseWindow();
}

SceneManager& Game::getSceneManager() { return instance->sceneManager; }
EventBus& Game::getEventBus() { return instance->eventBus; }
TimeManager& Game::getTimeManager() { return instance->timeManager; }
