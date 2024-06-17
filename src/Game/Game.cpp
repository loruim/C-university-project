#include "Game.h"

#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

#include "GameObject/Hero.h"

#include "GameState/Level.h"
#include "GameState/ShopScreen.h"
#include "GameState/FightScreen.h"
#include "../Physics/PhysicsEngine.h"
#include "../Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Game::Game(const glm::uvec2& windowSize) 
    : m_windowSize(windowSize)
    , m_eCurrentGameState(EGameState::ShopScreen)
{
    m_keys.fill(false);
}

void Game::setWindowSize(const glm::uvec2& windowSize)
{
    m_windowSize = windowSize;
    updateViewport();
}

Game::~Game()
{
}

void Game::render()
{
    m_pCurrentGameState->render();
}

void Game::updateViewport()
{
    const float map_aspect_ratio = static_cast<float>(getCurrentWidth()) / getCurrentHeight();
    unsigned int viewPortWidth = m_windowSize.x;
    unsigned int viewPortHeight = m_windowSize.y;
    unsigned int viewPortLeftOffset = 0;
    unsigned int viewPortBottomOffset = 0;

    if (static_cast<float>(m_windowSize.x) / m_windowSize.y > map_aspect_ratio)
    {
        viewPortWidth = static_cast<unsigned int>(m_windowSize.y * map_aspect_ratio);
        viewPortLeftOffset = (m_windowSize.x - viewPortWidth) / 2;
    }
    else
    {
        viewPortHeight = static_cast<unsigned int>(m_windowSize.x / map_aspect_ratio);
        viewPortBottomOffset = (m_windowSize.y - viewPortHeight) / 2;
    }

    RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(getCurrentWidth()), 0.f, static_cast<float>(getCurrentHeight()), -100.f, 100.f);
    m_pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
}

void Game::startGlobalMap()
{
     auto pLevel= std::make_shared<Level>(ResourceManager::getLevels()[0], this);
     m_pCurrentGameState = pLevel;
     Physics::PhysicsEngine::setCurrentLevel(pLevel);
     updateViewport();
}

void Game::startShopScreen(const size_t shopNumber)
{
    auto pShops = std::make_shared<ShopScreen>(ResourceManager::getShopsScreen()[0], this);
    pShops->setShopsType(shopNumber);
    m_pCurrentGameState = pShops;
    updateViewport();
}

void Game::startFightMap()
{
    auto pFightScreen = std::make_shared<FightScreen>(ResourceManager::getFightScreen(), this);
    m_pCurrentGameState = pFightScreen;
    Physics::PhysicsEngine::setCurrentFight(pFightScreen);
    updateViewport();
}

void Game::update(const double delta)
{
    m_pCurrentGameState->processInput(m_keys);
    m_pCurrentGameState->update(delta);
}

void Game::setKey(const int key, const int action)
{
    m_keys[key] = action;
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    m_pSpriteShaderProgram = ResourceManager::getShader("spriteShader");
    if (!m_pSpriteShaderProgram)
    {
        std::cerr << "Can't create shader: " << "spriteShader" << std::endl;
        return false;
    }
    m_pSpriteShaderProgram->use();
    m_pSpriteShaderProgram->setInt("tex", 0);

    auto pLevel = std::make_shared<Level>(ResourceManager::getLevels()[0], this);
    m_pCurrentGameState = pLevel;
    Physics::PhysicsEngine::setCurrentLevel(pLevel);
    updateViewport();
    setWindowSize(m_windowSize);
    
    return true;
}

unsigned int Game::getCurrentWidth() const
{
    return m_pCurrentGameState->getStateWidth();
}

unsigned int Game::getCurrentHeight() const
{
    return m_pCurrentGameState->getStateHeight();
}