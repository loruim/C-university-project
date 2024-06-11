#include "Level.h"

#include "../Resources/ResourceManager.h"
#include "GameObject/BrickRoad.h"
#include "GameObject/House.h"
#include "GameObject/Water.h"

#include <iostream>

const unsigned int BLOCK_SIZE = 16;

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

	m_mapObject.reserve(m_width * m_height);
	unsigned int currentBottomOffSet = static_cast<unsigned int>(BLOCK_SIZE * (m_height-1));

	for (const std::string& currentRow : levelDescription)
	{
		unsigned int currentLeftOffSet = 0;
		for (const char currentElement : currentRow)
		{
			m_mapObject.emplace_back(createGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffSet, currentBottomOffSet), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f));

			currentLeftOffSet += BLOCK_SIZE;
		}
		currentBottomOffSet -= BLOCK_SIZE;
	}
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

void Level::update(const uint64_t delta)
{
	for (const auto& currentMapObject : m_mapObject)
	{
		if (currentMapObject)
		{
			currentMapObject->update(delta);
		}
	}
}