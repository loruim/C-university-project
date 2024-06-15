#include "Game.h"

#include "../Resources/ResourceManager.h"
#include "../Physics/PhysicsEngine.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

#include "GameObject/Hero.h"

#include "GameState/Level.h"
#include "GameState/ShopScreen.h"

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

Game::~Game()
{
}

void Game::render()
{
    switch (m_eCurrentGameState)
    {
    case Game::EGameState::FightScreen:
        break;
    case Game::EGameState::GlobalMap:
        if (m_pLevel)
        {
            m_pLevel->render();
        }

        if (m_pHero)
        {
            m_pHero->render();
        }
        break;
    case Game::EGameState::LoseScreen:
        break;
    case Game::EGameState::Pause:
        break;
    case Game::EGameState::ShopScreen:
        m_pShopsScreen->render();
        break;
    case Game::EGameState::WinnerScreen:
        break;
    default:
        break;
    }
}

void Game::update(const double delta)
{
    switch (m_eCurrentGameState)
    {
    case EGameState::FightScreen:
        break;
    case EGameState::GlobalMap:
        if (m_pLevel)
        {
            m_pLevel->update(delta);
        }

        if (m_pHero)
        {
            if (m_keys[GLFW_KEY_W])
            {
                m_pHero->SetOrientation(Hero::EOrientaition::Top);
                m_pHero->setVelocity(m_pHero->getMaxVelocity());
            }
            else if (m_keys[GLFW_KEY_A])
            {
                m_pHero->SetOrientation(Hero::EOrientaition::Left);
                m_pHero->setVelocity(m_pHero->getMaxVelocity());
            }
            else if (m_keys[GLFW_KEY_D])
            {
                m_pHero->SetOrientation(Hero::EOrientaition::Right);
                m_pHero->setVelocity(m_pHero->getMaxVelocity());
            }
            else if (m_keys[GLFW_KEY_S])
            {
                m_pHero->SetOrientation(Hero::EOrientaition::Bottom);
                m_pHero->setVelocity(m_pHero->getMaxVelocity());
            }
            else
            {
                m_pHero->setVelocity(0);
            }

            if (m_pHero && m_keys[GLFW_KEY_SPACE])
            {
                m_pHero->fire();
            }

            m_pHero->update(delta);
        }

        if (m_keys[GLFW_KEY_ENTER])
        {
            m_eCurrentGameState = EGameState::ShopScreen;
        }
        break;
    case EGameState::LoseScreen:
        break;
    case EGameState::Pause:
        break;
    case EGameState::ShopScreen:
        if (m_keys[GLFW_KEY_Q])
        {
            m_eCurrentGameState = EGameState::GlobalMap;
        }
        break;
    case EGameState::WinnerScreen:
        break;
    }
}

void Game::setKey(const int key, const int action)
{
    m_keys[key] = action;
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShader("spriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't create shader: " << "spriteShader" << std::endl;
        return false;
    }

    m_pShopsScreen = std::make_shared<ShopScreen>(ResourceManager::getShopsScreen()[0]);
    m_pLevel = std::make_shared<Level>(ResourceManager::getLevels()[0]);
    m_windowSize.x = static_cast<int>(m_pLevel->getStateWidth());
    m_windowSize.y = static_cast<int>(m_pLevel->getStateHeight());
    Physics::PhysicsEngine::setCurrentLevel(m_pLevel);

    /* Преоброзование координат */
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(getCurrentWidth()), 0.f, static_cast<float>(getCurrentHeight()), -100.f, 100.f);
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
    /* ------------------------ */
    
    m_pHero = std::make_shared<Hero>(0.05, m_pLevel->getPlayerRespawn(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 1.f);
    Physics::PhysicsEngine::addDynamicGameObject(m_pHero);

    return true;
}

unsigned int Game::getCurrentWidth() const
{
    switch (m_eCurrentGameState)
    {
    case Game::EGameState::FightScreen:
        break;
    case Game::EGameState::GlobalMap:
        return m_pLevel->getStateWidth();
        break;
    case Game::EGameState::LoseScreen:
        break;
    case Game::EGameState::Pause:
        break;
    case Game::EGameState::ShopScreen:
        return m_pShopsScreen->getStateWidth();
        break;
    case Game::EGameState::WinnerScreen:
        break;
    }
}

unsigned int Game::getCurrentHeight() const
{
    switch (m_eCurrentGameState)
    {
    case Game::EGameState::FightScreen:
        break;
    case Game::EGameState::GlobalMap:
        return m_pLevel->getStateHeight();
        break;
    case Game::EGameState::LoseScreen:
        break;
    case Game::EGameState::Pause:
        break;
    case Game::EGameState::ShopScreen:
        return m_pShopsScreen->getStateHeight();
        break;
    case Game::EGameState::WinnerScreen:
        break;
    default:
        break;
    }
}