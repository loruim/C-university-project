#include "Level.h"

#include "../GameObject/BrickRoad.h"
#include "../GameObject/House.h"
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

Level::Level(const std::vector<std::string>& levelDescription, Game* pGame) : m_pGame(pGame)
{
    if (levelDescription.empty())
    {
        std::cerr << "Empty level description" << std::endl;
    }

    m_widthBlocks = levelDescription[0].length();
    m_heightBlocks = levelDescription.size();
    m_widhtPixel = static_cast<unsigned int>(m_widthBlocks * BLOCK_SIZE);
    m_heightPixel = static_cast<unsigned int>(m_heightBlocks * BLOCK_SIZE);

    m_playerRespawn = { 4 * BLOCK_SIZE, 6 * BLOCK_SIZE + BLOCK_SIZE / 2 };
    m_enemyRespawn_1 = { 4 * BLOCK_SIZE, 2 * BLOCK_SIZE + BLOCK_SIZE / 2 };
    m_enemyRespawn_2 = { 3 * BLOCK_SIZE, 3 * BLOCK_SIZE + BLOCK_SIZE / 2 };
    m_enemyRespawn_3 = { 5 * BLOCK_SIZE, 3 * BLOCK_SIZE + BLOCK_SIZE / 2 };

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
    m_pHero = std::make_shared<Hero>(0.05, getPlayerRespawn(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 1.f);
    Physics::PhysicsEngine::addDynamicGameObject(m_pHero);
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
            m_pGame->startShopScreen(0);
        }
    }
    m_pHero->update(delta);
}

void Level::processInput(std::array<bool, 349> keys)
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