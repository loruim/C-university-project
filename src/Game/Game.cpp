#include "Game.h"

#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

#include "GameObject/Hero.h"

#include "Level.h"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Game::Game(const glm::ivec2& windowSize) : m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize)
{
    m_keys.fill(false);
}

Game::~Game()
{

}

void Game::render()
{
    if (m_pLevel)
    {
        m_pLevel->render();
    }

    if (m_pHero)
    {
        m_pHero->render();
    }
}

void Game::update(const uint64_t delta)
{
    if (m_pLevel)
    {
        m_pLevel->update(delta);
    }

    if (m_pHero)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pHero->SetOrientation(Hero::EOrientaition::Top);
            m_pHero->move(true);
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_pHero->SetOrientation(Hero::EOrientaition::Left);
            m_pHero->move(true);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_pHero->SetOrientation(Hero::EOrientaition::Right);
            m_pHero->move(true);
        }
        else if (m_keys[GLFW_KEY_S])
        {
            m_pHero->SetOrientation(Hero::EOrientaition::Bottom);
            m_pHero->move(true);
        }
        else
        {
            m_pHero->move(false);
        }

        m_pHero->update(delta);
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
        std::cerr << "Can't create sprite: " << "spriteShader" << std::endl;
        return false;
    }

    auto pTextureAtlas = ResourceManager::getTexture("mapTextureAtlas");
    if (!pTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: " << "mapTextureAtlas" << std::endl;
        return false;
    }

    auto pHerosTexturesAtlas = ResourceManager::getTexture("archerTextureAtlas");
    if (!pHerosTexturesAtlas)
    {
        std::cerr << "Can't find texture atlas: " << "archerTextureAtlas" << std::endl;
        return false;
    }

    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[0]);
    m_windowSize.x = static_cast<int>(m_pLevel->getLevelWidth());
    m_windowSize.y = static_cast<int>(m_pLevel->getLevelHeight());

    /* Преоброзование координат */
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
    /* ------------------------ */

    m_pHero = std::make_unique<Hero>(ResourceManager::getSprite("archerTopState"), 
                                     ResourceManager::getSprite("archerBottomState"), 
                                     ResourceManager::getSprite("archerLeftState"), 
                                     ResourceManager::getSprite("archerRightState"), 
                                     0.0000001f, m_pLevel->getPlayerRespawn()/*glm::vec2(3*16.f, 6*16.f)*/, glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 1.f);
    return true;
}

size_t Game::getCurrentLevelWidth() const
{
    return m_pLevel->getLevelWidth();
}

size_t Game::getCurrentLevelHeight() const
{
    return m_pLevel->getLevelHeight();
}