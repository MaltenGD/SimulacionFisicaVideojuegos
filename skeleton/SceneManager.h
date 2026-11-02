#pragma once

#include "Scene.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>

class SceneManager {
public:
    static SceneManager& instance() {
        static SceneManager inst;
        return inst;
    }

    template <typename T>
    void registerScene(const std::string& name) {
        m_factories[name] = [name]() { return std::make_unique<T>(name); };
    }

    void changeScene(const std::string& name);
    void update(double dt);
    void keyPress(unsigned char key, Camera* camera);

    [[nodiscard]] Scene* getCurrentScene() const { return m_currentScene.get(); }

private:
    SceneManager() = default;

    std::unordered_map<std::string, std::function<std::unique_ptr<Scene>()>> m_factories;
    std::unique_ptr<Scene> m_currentScene{ nullptr };
    std::string m_pendingSceneName;
    bool m_hasPendingChange{ false };

    void applyPendingSceneChange();
};
