#include "SceneManager.h"
#include <raylib.h>
#include "../utils/Logger.h"

SceneManager::SceneManager() : isFading(false), fadeTimer(0.0f), fadeDuration(0.5f), fadingOut(false) {}

void SceneManager::changeScene(std::unique_ptr<IScene> scene) {
    isFading = true;
    fadingOut = true;
    fadeTimer = 0.0f;
    nextScene = std::move(scene);
    LOG_INFO("Changing scene to " + nextScene->getName());
}

void SceneManager::pushScene(std::unique_ptr<IScene> scene) {
    if (!scenes.empty()) {
        scenes.back()->onExit();
    }
    LOG_INFO("Pushing scene " + scene->getName());
    scene->onEnter();
    scenes.push_back(std::move(scene));
}

void SceneManager::popScene() {
    if (!scenes.empty()) {
        LOG_INFO("Popping scene " + scenes.back()->getName());
        scenes.back()->onExit();
        scenes.pop_back();
        if (!scenes.empty()) {
            scenes.back()->onEnter();
        }
    }
}

void SceneManager::update(float dt) {
    if (isFading) {
        fadeTimer += dt;
        if (fadeTimer >= fadeDuration) {
            if (fadingOut) {
                if (!scenes.empty()) {
                    scenes.back()->onExit();
                    scenes.pop_back();
                }
                if (nextScene) {
                    nextScene->onEnter();
                    scenes.push_back(std::move(nextScene));
                }
                fadingOut = false;
                fadeTimer = 0.0f;
            } else {
                isFading = false;
            }
        }
        return;
    }
    
    if (!scenes.empty()) {
        scenes.back()->update(dt);
    }
}

void SceneManager::draw() {
    if (!scenes.empty()) {
        scenes.back()->draw();
    }
    
    if (isFading) {
        float alpha = fadingOut ? (fadeTimer / fadeDuration) : (1.0f - (fadeTimer / fadeDuration));
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));
    }
}

void SceneManager::drawScreenOverlay() {
    if (!scenes.empty()) {
        scenes.back()->drawScreenOverlay();
    }
}
