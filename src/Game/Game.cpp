#include "Game.h"

#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"
#include "Archer.h"

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
    // ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
    if (m_pArcher)
    {
        m_pArcher->render();
    }
}

void Game::update(const uint64_t delta)
{
    // ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
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
    auto pDefaultShaderProgram = ResourceManager::loadShaderProgram("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
        return false;
    }

    auto pSpriteShaderProgram = ResourceManager::loadShaderProgram("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't create sprite: " << "SpriteShader" << std::endl;
        return false;
    }

    // auto tex = ResourceManager::loadTexture("DefaultTexture", "res/texture/battleCity.png");

    std::vector<std::string> subTexturesArcher = {
        "ArcherStay1",
        "ArcherMove1",
        "ArcherStay2",
        "ArcherMove2",
        "ArcgerAttackRight1",
        "ArcgerAttackRight2",
        "ArcgerAttackLeft1",
        "ArcgerAttackLeft2",
        };

    auto pTexturesAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/texture/Archer.png", std::move(subTexturesArcher), 16, 18);

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "ArcherStay1");

    pAnimatedSprite->setPosition(glm::vec2(300, 300));

    std::vector<std::pair<std::string, uint16_t>> ArcherState;
    ArcherState.emplace_back(std::make_pair<std::string, uint16_t>("ArcgerAttackRight1", 1000000000));
    ArcherState.emplace_back(std::make_pair<std::string, uint16_t>("ArcgerAttackRight2", 1000000000));

    pAnimatedSprite->insertState("ArcherState", std::move(ArcherState));

    pAnimatedSprite->setState("ArcherState");

    pDefaultShaderProgram->use();
    pDefaultShaderProgram->setInt("tex", 0);

    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);




    std::vector<std::string> ArchersubTexturesNames = {
        "ArcherRight1",
        "ArcherRight2",
        "ArcherLeft1",
        "ArcherLeft2",
        "ArcgerAttackRight1",
        "ArcgerAttackRight2",
        "ArcgerAttackLeft1",
        "ArcgerAttackLeft2",
    };

    auto pArchersTexturesAtlas = ResourceManager::loadTextureAtlas("ArcherTextureAtlas", "res/texture/Archer.png", std::move(ArchersubTexturesNames), 16, 18);

    auto pArchersAnimatedSprite = ResourceManager::loadAnimatedSprite("ArcherAnimatedSprite", "ArcherTextureAtlas", "SpriteShader", 100, 100, "ArcherRight1");

    std::vector<std::pair<std::string, uint16_t>> ArcherRightState;
    ArcherRightState.emplace_back(std::make_pair<std::string, uint16_t>("ArcherRight1", 500000000));
    ArcherRightState.emplace_back(std::make_pair<std::string, uint16_t>("ArcherRight2", 500000000));

    std::vector<std::pair<std::string, uint16_t>> ArcherLeftState;
    ArcherLeftState.emplace_back(std::make_pair<std::string, uint16_t>("ArcherLeft1", 500000000));
    ArcherLeftState.emplace_back(std::make_pair<std::string, uint16_t>("ArcherLeft2", 500000000));

    pArchersAnimatedSprite->insertState("ArcherRightState", std::move(ArcherRightState));
    pArchersAnimatedSprite->insertState("ArcherLeftState", std::move(ArcherLeftState));

    pArchersAnimatedSprite->setState("ArcherRightState");

    

    m_pArcher = std::make_unique<Archer>(pArchersAnimatedSprite, 0.000001f, glm::vec2(100.f, 100.f));

    return true;
}