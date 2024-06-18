#include "Level.h"

#include "../GameObject/BrickRoad.h"
#include "../GameObject/House.h"
#include "../GameObject/HouseLeftOne.h"
#include "../GameObject/HouseLeftTwo.h"
#include "../GameObject/HouseRightOne.h"
#include "../GameObject/HouseRightTwo.h"
#include "../GameObject/Water.h"
#include "../GameObject/Border.h"
#include "../GameObject/Hero.h"
#include "../Game.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <algorithm>
#include <cmath>

std::shared_ptr<IGameObject> createGameObjectFromDescription(const char description, const glm::vec2& position, const glm::vec2& size, const float rotation)
{
    switch (description)
    {
    case '0':
        return std::make_shared<BrickRoad>(position, size, rotation, 0.f);
        break;
    case '1':
        return std::make_shared<House>(position, size, rotation, 1.f);
    case '3':
        return std::make_shared<HouseLeftOne>(position, size, rotation, 0.f);
        break;
    case '4':
        return std::make_shared<HouseRightOne>(position, size, rotation, 0.f);
        break;
    case '6':
        return std::make_shared<HouseLeftTwo>(position, size, rotation, 0.f);
        break;
    case '7':
        return std::make_shared<HouseRightTwo>(position, size, rotation, 0.f);
        break;
    case '2':
        return std::make_shared<Water>(position, size, rotation, 0.f);
        break;
    case '5':
        return nullptr;
        break;
    default:
        std::cerr << "Unknown GameObject description" << description << std::endl;
        break;
    }
    return nullptr;
}

Level::Level(const std::vector<std::string>& levelDescription, Game* pGame, bool enemyLeftLive, bool enemyMiddleLive, bool enemyRightLive)
    : m_pGame(pGame)
    , m_enemyLeftIsActive(enemyLeftLive)
    , m_enemyMiddleIsActive(enemyMiddleLive)
    , m_enemyRightIsActive(enemyRightLive)
    , m_leftEnemy_RightTopPosition(0)
    , m_leftEnemy_LeftBottomPosition(0)
{
    if (levelDescription.empty())
    {
        std::cerr << "Empty level description" << std::endl;
    }

    m_widthBlocks = levelDescription[0].length();
    m_heightBlocks = levelDescription.size();
    m_widhtPixel = static_cast<unsigned int>(m_widthBlocks * BLOCK_SIZE);
    m_heightPixel = static_cast<unsigned int>(m_heightBlocks * BLOCK_SIZE);

    m_playerRespawn = { 4 * BLOCK_SIZE, 6 * BLOCK_SIZE + BLOCK_SIZE / 2 }; // 64, 104
    m_enemyRespawn_1 = { 4 * BLOCK_SIZE, 2 * BLOCK_SIZE + BLOCK_SIZE / 2 }; // 64, 40
    m_enemyRespawn_2 = { 3 * BLOCK_SIZE, 3 * BLOCK_SIZE + BLOCK_SIZE / 2 }; // 48, 56
    m_enemyRespawn_3 = { 5 * BLOCK_SIZE, 3 * BLOCK_SIZE + BLOCK_SIZE / 2 }; // 80, 56

    m_mapObject.reserve(m_widthBlocks * m_heightBlocks + 4);
    unsigned int currentBottomOffSet = static_cast<unsigned int>(BLOCK_SIZE * (m_heightBlocks - 1) + BLOCK_SIZE / 2.f);

    for (const std::string& currentRow : levelDescription)
    {
        unsigned int currentLeftOffSet = BLOCK_SIZE;
        for (const char currentElement : currentRow)
        {
            m_mapObject.emplace_back(createGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffSet, currentBottomOffSet), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f));
            currentLeftOffSet += BLOCK_SIZE;
        }
        currentBottomOffSet -= BLOCK_SIZE;
    }

    // bottom border
    m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(m_widthBlocks * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

    // top border
    m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_heightBlocks * BLOCK_SIZE + BLOCK_SIZE / 2), glm::vec2(m_widthBlocks * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

    // left border
    m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, (m_heightBlocks + 1) * BLOCK_SIZE), 0.f, 0.f));

    // right border
    m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2((m_widthBlocks + 1) * BLOCK_SIZE, 0.f), glm::vec2(BLOCK_SIZE * 2.f, (m_heightBlocks + 1) * BLOCK_SIZE), 0.f, 0.f));

}

void Level::initPhysics()
{
    m_pHero = std::make_shared<Hero>(Hero::EHeroType::Archer, 0.05, getPlayerRespawn(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 1.f);
    Physics::PhysicsEngine::addDynamicGameObject(m_pHero);

    //m_enemyLeftIsActive = true;
    if (m_enemyLeftIsActive)
    {
        m_enemyLeft = std::make_shared<Hero>(Hero::EHeroType::Knight, 0.05, getEnemyRespawn_2(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 1.f);
        m_enemyLeft->SetIsHaveAngel(false);
        m_enemyLeft->SetIsHaveArcher(true);
        m_enemyLeft->SetIsHaveBarbarian(false);
        m_enemyLeft->SetIsHaveKnight(true);
        m_enemyLeft->SetIsHaveMagican(false);
        m_enemyLeft->SetIsHaveTitan(false);
        Physics::PhysicsEngine::addDynamicGameObject(m_enemyLeft);
    }
    
    if (m_enemyMiddleIsActive)
    {
        m_enemyMiddle = std::make_shared<Hero>(Hero::EHeroType::Angel, 0.05, getEnemyRespawn_1(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 1.f);
        m_enemyMiddle->SetIsHaveAngel(true);
        m_enemyMiddle->SetIsHaveArcher(false);
        m_enemyMiddle->SetIsHaveBarbarian(false);
        m_enemyMiddle->SetIsHaveKnight(false);
        m_enemyMiddle->SetIsHaveMagican(true);
        m_enemyMiddle->SetIsHaveTitan(false);
        Physics::PhysicsEngine::addDynamicGameObject(m_enemyMiddle);
    }
    
    if (m_enemyRightIsActive)
    {
        m_enemyRight = std::make_shared<Hero>(Hero::EHeroType::Barbarian, 0.05, getEnemyRespawn_3(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 1.f);
        m_enemyRight->SetIsHaveAngel(false);
        m_enemyRight->SetIsHaveArcher(false);
        m_enemyRight->SetIsHaveBarbarian(true);
        m_enemyRight->SetIsHaveKnight(false);
        m_enemyRight->SetIsHaveMagican(false);
        m_enemyRight->SetIsHaveTitan(true);
        Physics::PhysicsEngine::addDynamicGameObject(m_enemyRight);
    }
}

void Level::render() const
{
    for (const auto& currentMapObject : m_mapObject)
    {
        if (currentMapObject)
        {
            currentMapObject->render();
        }
    }
    m_pHero->render();

    if (m_enemyLeftIsActive)
    {
        m_enemyLeft->render();
    }

    if (m_enemyMiddleIsActive)
    {
        m_enemyMiddle->render();
    }

    if (m_enemyRightIsActive)
    {
        m_enemyRight->render();
    }
}

void Level::update(const double delta)
{
    for (const auto& currentMapObject : m_mapObject)
    {
        if (currentMapObject)
        {
            currentMapObject->update(delta);
        }
        if (currentMapObject->isActive())
        {
            auto getType = currentMapObject->getObjectType();
            switch (getType)
            {
            case IGameObject::EObjectType::House:
                m_pGame->startShopScreen(0);
                break;
            case IGameObject::EObjectType::LeftHouseOne:
                m_pGame->startShopScreen(1);
                break;
            case IGameObject::EObjectType::LeftHouseTwo:
                m_pGame->startShopScreen(2);
                break;
            case IGameObject::EObjectType::RightHouseOne:
                m_pGame->startShopScreen(3);
                break;
            case IGameObject::EObjectType::RightHouseTwo:
                m_pGame->startShopScreen(4);
                break;
            }
        }
    }
    m_pHero->update(delta);

    if (m_enemyLeftIsActive)
    {
        m_enemyLeft->update(delta);
        m_leftEnemy_LeftBottomPosition = m_enemyLeft->getCurrentPosition();
        m_leftEnemy_RightTopPosition = glm::vec2(m_leftEnemy_LeftBottomPosition.x + 16, m_leftEnemy_LeftBottomPosition.y + 16);
        if (m_pHero->getCurrentPosition().x > m_leftEnemy_LeftBottomPosition.x && m_pHero->getCurrentPosition().x < m_leftEnemy_RightTopPosition.x
            && m_pHero->getCurrentPosition().y > m_leftEnemy_LeftBottomPosition.y && m_pHero->getCurrentPosition().y < m_leftEnemy_RightTopPosition.y)
        {
            m_pGame->startFightMap(m_enemyLeft);
        }
    }
    if (m_enemyMiddleIsActive)
    {
        m_enemyMiddle->update(delta);
        m_leftEnemy_LeftBottomPosition = glm::vec2(m_enemyMiddle->getCurrentPosition().x - 3, m_enemyMiddle->getCurrentPosition().y);
        m_leftEnemy_RightTopPosition = glm::vec2(m_enemyMiddle->getCurrentPosition().x + 8, m_leftEnemy_LeftBottomPosition.y + 16);
        if (m_pHero->getCurrentPosition().x > m_leftEnemy_LeftBottomPosition.x && m_pHero->getCurrentPosition().x < m_leftEnemy_RightTopPosition.x
            && m_pHero->getCurrentPosition().y > m_leftEnemy_LeftBottomPosition.y && m_pHero->getCurrentPosition().y < m_leftEnemy_RightTopPosition.y)
        {
            m_pGame->startFightMap(m_enemyMiddle);
        }
    }
    if (m_enemyRightIsActive)
    {
        m_enemyRight->update(delta);
        m_leftEnemy_LeftBottomPosition = glm::vec2(m_enemyRight->getCurrentPosition().x - 3, m_enemyRight->getCurrentPosition().y);
        m_leftEnemy_RightTopPosition = glm::vec2(m_enemyRight->getCurrentPosition().x + 16, m_leftEnemy_LeftBottomPosition.y + 16);
        if (m_pHero->getCurrentPosition().x > m_leftEnemy_LeftBottomPosition.x && m_pHero->getCurrentPosition().x < m_leftEnemy_RightTopPosition.x
            && m_pHero->getCurrentPosition().y > m_leftEnemy_LeftBottomPosition.y && m_pHero->getCurrentPosition().y < m_leftEnemy_RightTopPosition.y) // 82, 55
        {
            m_pGame->startFightMap(m_enemyRight);
        }
    }
}

void Level::processInputKey(std::array<bool, 349> keys)
{
    if (keys[GLFW_KEY_W])
    {
        m_pHero->SetOrientation(Hero::EOrientaition::Top);
        m_pHero->setVelocity(m_pHero->getMaxVelocity());
    }
    else if (keys[GLFW_KEY_A])
    {
        m_pHero->SetOrientation(Hero::EOrientaition::Left);
        m_pHero->setVelocity(m_pHero->getMaxVelocity());
    }
    else if (keys[GLFW_KEY_D])
    {
        m_pHero->SetOrientation(Hero::EOrientaition::Right);
        m_pHero->setVelocity(m_pHero->getMaxVelocity());
    }
    else if (keys[GLFW_KEY_S])
    {
        m_pHero->SetOrientation(Hero::EOrientaition::Bottom);
        m_pHero->setVelocity(m_pHero->getMaxVelocity());
    }
    else
    {
        m_pHero->setVelocity(0);
    }

    if (m_pHero && keys[GLFW_KEY_SPACE])
    {
        m_pHero->fire();
    }
}

unsigned int Level::getStateWidth() const
{
    return static_cast<unsigned int>((m_widthBlocks + 3) * BLOCK_SIZE);
}

unsigned int Level::getStateHeight() const
{
    return static_cast<unsigned int>((m_heightBlocks + 1) * BLOCK_SIZE);
}

std::vector<std::shared_ptr<IGameObject>> Level::getObjectsInArea(const glm::vec2& bottomLeft, const glm::vec2& topRight)
{
    std::vector<std::shared_ptr<IGameObject>> output;
    output.reserve(9);

    glm::vec2 bottomLeft_converted(std::clamp(bottomLeft.x - BLOCK_SIZE, 0.f, static_cast<float>(m_widhtPixel)),
                                   std::clamp(m_heightPixel - bottomLeft.y + BLOCK_SIZE / 2, 0.f, static_cast<float>(m_heightPixel)));
    glm::vec2 topRight_converted(std::clamp(topRight.x - BLOCK_SIZE, 0.f, static_cast<float>(m_widhtPixel)),
                                 std::clamp(m_heightPixel - topRight.y + BLOCK_SIZE / 2, 0.f, static_cast<float>(m_heightPixel)));

    size_t startX = static_cast<size_t>(floor(bottomLeft_converted.x / BLOCK_SIZE));
    size_t endX = static_cast<size_t>(ceil(topRight_converted.x / BLOCK_SIZE));

    size_t startY = static_cast<size_t>(floor(topRight_converted.y / BLOCK_SIZE));
    size_t endY = static_cast<size_t>(ceil(bottomLeft_converted.y / BLOCK_SIZE));

    for (size_t currentColumn = startX; currentColumn < endX; ++currentColumn)
    {
        for (size_t currentRow = startY; currentRow < endY; ++currentRow)
        {
            auto& currentObject = m_mapObject[currentRow * m_widthBlocks + currentColumn];
            if (currentObject)
            {
                output.push_back(currentObject);
            }
        }
    }

    if (endX >= m_widthBlocks)
    {
        output.push_back(m_mapObject[m_mapObject.size() - 1]);
    }
    if (startX <= 1)
    {
        output.push_back(m_mapObject[m_mapObject.size() - 2]);
    }
    if (startY <= 1)
    {
        output.push_back(m_mapObject[m_mapObject.size() - 3]);
    }
    if (endY >= m_heightBlocks)
    {
        output.push_back(m_mapObject[m_mapObject.size() - 4]);
    }

    return output;
}