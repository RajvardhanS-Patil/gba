#pragma once
#include <string>
#include <memory>
#include <vector>

class IScene {
public:
    virtual ~IScene() = default;
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual std::string getName() const = 0;
};

class SceneManager {
public:
    SceneManager();
    ~SceneManager() = default;

    void changeScene(std::unique_ptr<IScene> scene);
    void pushScene(std::unique_ptr<IScene> scene);
    void popScene();

    void update(float dt);
    void draw();

private:
    std::vector<std::unique_ptr<IScene>> scenes;
    
    bool isFading;
    float fadeTimer;
    float fadeDuration;
    bool fadingOut;
    std::unique_ptr<IScene> nextScene;
};
