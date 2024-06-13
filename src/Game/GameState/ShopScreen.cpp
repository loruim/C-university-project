#include "ShopScreen.h"

#include <iostream>

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"

std::shared_ptr<RenderEngine::Sprite> getSpriteForDescription(const char description)
{
	switch (description)
	{
	case '0':
		return ResourceManager::getSprite("FullRoad");
	case '5':
		return nullptr;
	default:
		std::cerr << "Unknown shops description" << description << std::endl;
	}
	return nullptr;
}

ShopScreen::ShopScreen(const std::vector<std::string>& shopScreenDescription)
{
	if (shopScreenDescription.empty())
	{
		std::cerr << "Empty shop screen description" << std::endl;
	}

	auto leftOffsetPixel = 2 * BLOCK_SIZE;
	auto bottomOffset = STARTSCREEN_HEIGHT - 2 * BLOCK_SIZE;

	unsigned int currentBottomOffset = bottomOffset;
	for (const std::string& currentRow : shopScreenDescription)
	{
		unsigned int currentLeftOffset = leftOffsetPixel;
		for (const char currentElement : currentRow)
		{
			m_sprites.emplace_back(std::make_pair(getSpriteForDescription(currentElement), glm::vec2(currentLeftOffset, currentBottomOffset)));
			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}
}

void ShopScreen::render() const
{
	for (const auto& current : m_sprites)
	{
		if (current.first)
		{
			current.first->render(current.second, glm::vec2(BLOCK_SIZE), 0.f);
		}
	}
}

void ShopScreen::update(const double delta)
{
}

unsigned int ShopScreen::getStateWidth() const
{
	return STARTSCREEN_WIDTH;
}

unsigned int ShopScreen::getStateHeight() const
{
	return STARTSCREEN_HEIGHT;
}