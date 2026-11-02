#include "SceneManager.h"
#include <iostream>

void SceneManager::changeScene(const std::string& name) {
    if (m_factories.find(name) == m_factories.end()) {
        std::cerr << "[SceneManager Error] Escena no registrada: " << name << "\n";
        return;
    }
    m_pendingSceneName = name;
    m_hasPendingChange = true;
}

void SceneManager::applyPendingSceneChange() {
    if (!m_hasPendingChange) return;

    if (m_currentScene) {
        m_currentScene->cleanup();
    }

    m_currentScene = m_factories[m_pendingSceneName]();
    if (m_currentScene) {
        m_currentScene->init();
        std::cout << "[SceneManager] Escena cargada: " << m_pendingSceneName << "\n";
    }

    m_hasPendingChange = false;
    m_pendingSceneName.clear();
}

void SceneManager::update(double dt) {
    if (m_hasPendingChange) {
        applyPendingSceneChange();
    }

    if (m_currentScene) {
        m_currentScene->update(dt);
    }
}

void SceneManager::keyPress(unsigned char key, Camera* camera) {
    if (m_currentScene != nullptr) {
        m_currentScene->keyPress(key, camera);
    }
}
