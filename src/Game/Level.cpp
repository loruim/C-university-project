#include "Level.h"

#include "../Resources/ResourceManager.h"
#include "GameObject/BrickRoad.h"
#include "GameObject/House.h"
#include "GameObject/Water.h"
#include "GameObject/Border.h"

#include <iostream>

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
	case '3':
		return nullptr;
		break;
	default:
		std::cerr << "Unknown GameObject description" << description << std::endl;
		break;
	}
	return nullptr;
}

Level::Level(const std::vector<std::string>& levelDescription)
{
	if (levelDescription.empty())
	{
		std::cerr << "Empty level description" << std::endl;
	}

	m_width = levelDescription[0].length();
	m_height = levelDescription.size();

	m_playerRespawn = { 4 * BLOCK_SIZE, 6 * BLOCK_SIZE + BLOCK_SIZE / 2 };
	m_enemyRespawn_1 = { 4 * BLOCK_SIZE, 2 * BLOCK_SIZE + BLOCK_SIZE / 2 };
	m_enemyRespawn_2 = { 3 * BLOCK_SIZE, 3 * BLOCK_SIZE + BLOCK_SIZE / 2 };
	m_enemyRespawn_3 = { 5 * BLOCK_SIZE, 3 * BLOCK_SIZE + BLOCK_SIZE / 2 };

	m_mapObject.reserve(m_width * m_height + 4);
	unsigned int currentBottomOffSet = static_cast<unsigned int>(BLOCK_SIZE * (m_height-1) + BLOCK_SIZE / 2.f);

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
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

	// top border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_height * BLOCK_SIZE + BLOCK_SIZE / 2), glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

	// left border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, (m_height + 1) * BLOCK_SIZE), 0.f, 0.f));

	// right border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2((m_width + 1) * BLOCK_SIZE, 0.f), glm::vec2(BLOCK_SIZE * 2.f, (m_height + 1) * BLOCK_SIZE), 0.f, 0.f));

}

void Level::render()
{
	for (const auto& currentMapObject : m_mapObject)
	{
		if (currentMapObject)
		{
			currentMapObject->render();
		}
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
	}
}

size_t Level::getLevelWidth() const
{
	return (m_width + 3) * BLOCK_SIZE;
}

size_t Level::getLevelHeight() const
{
	return (m_height + 1) * BLOCK_SIZE;
}