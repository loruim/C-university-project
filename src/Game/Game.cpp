#include "Game.h"

#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include "GameObject/Archer.h"

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
    //ResourceManager::getAnimatedSprite("DefaultAnimetadSprite")->render();

    if (m_pLevel)
    {
        m_pLevel->render();
    }

    if (m_pArcher)
    {
        m_pArcher->render();
    }

    
}

void Game::update(const uint64_t delta)
{
    //ResourceManager::getAnimatedSprite("DefaultAnimetadSprite")->update(delta);

    if (m_pLevel)
    {
        m_pLevel->update(delta);
    }

    if (m_pArcher)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pArcher->SetOrientation(Archer::EOrientaition::Top);
            m_pArcher->move(true);
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_pArcher->SetOrientation(Archer::EOrientaition::Left);
            m_pArcher->move(true);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_pArcher->SetOrientation(Archer::EOrientaition::Right);
            m_pArcher->move(true);
        }
        else if (m_keys[GLFW_KEY_S])
        {
            m_pArcher->SetOrientation(Archer::EOrientaition::Bottom);
            m_pArcher->move(true);
        }
        else
        {
            m_pArcher->move(false);
        }

        m_pArcher->update(delta);
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

    auto pArchersTexturesAtlas = ResourceManager::getTexture("archerTextureAtlas");
    if (!pArchersTexturesAtlas)
    {
        std::cerr << "Can't find texture atlas: " << "archerTextureAtlas" << std::endl;
        return false;
    }

    /*auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("DefaultAnimetadSprite", "mapTextureAtlas", "spriteShader", 100, 100, "FullRoad");

    pAnimatedSprite->setPosition(glm::vec2(300, 300));
    
    std::vector<std::pair<std::string, uint16_t>> waterState;
    waterState.emplace_back(std::make_pair<std::string, uint16_t>("Water1", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint16_t>("Water2", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint16_t>("Water3", 1000000000));
    pAnimatedSprite->insertState("waterState", std::move(waterState));
    pAnimatedSprite->setState("waterState");*/

    /* Преоброзование координат */
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
    /* ------------------------ */

    auto pArchersAnimatedSprite = ResourceManager::getAnimatedSprite("archerAnimatedSprite");
    if (!pArchersAnimatedSprite)
    {
        std::cerr << "Can't find animated sprite: " << "archerAnimatedSprite" << std::endl;
        return false;
    }

    m_pArcher = std::make_unique<Archer>(pArchersAnimatedSprite, 0.000001f, glm::vec2(3*16.f, 6*16.f), glm::vec2(16.f, 16.f));

    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[0]);

    return true;
}