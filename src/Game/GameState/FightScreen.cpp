#include "FightScreen.h"

#include <iostream>

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"
#include "../Game.h"

#include <GLFW/glfw3.h>

std::shared_ptr<RenderEngine::Sprite> getFightObjectForDescription(const char description)
{
	switch (description)
	{
	case '8':
		return ResourceManager::getSprite("inaccessibleField");
	case '5':
		return nullptr;
	default:
		std::cerr << "Unknown shops description" << description << std::endl;
	}
	return nullptr;
}

FightScreen::FightScreen(const std::vector<std::string>& fightScreenDescription)
{
	if (fightScreenDescription.empty())
	{
		std::cerr << "Empty fight screen description" << std::endl;
	}

	auto leftOffsetPixel = BLOCK_SIZE * 2;
	auto bottomOffset = STARTSCREEN_HEIGHT - BLOCK_SIZE * 2;

	unsigned int currentBottomOffset = bottomOffset;
	for (const std::string& currentRow : fightScreenDescription)
	{
		unsigned int currentLeftOffset = leftOffsetPixel;
		for (const char currentElement : currentRow)
		{
			m_sprites.emplace_back(std::make_pair(getFightObjectForDescription(currentElement), glm::vec2(currentLeftOffset, currentBottomOffset)));
			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}
}

unsigned int FightScreen::getStateWidth() const
{
	return STARTSCREEN_WIDTH;
}

unsigned int FightScreen::getStateHeight() const
{
	return STARTSCREEN_HEIGHT;
}

void FightScreen::render() const
{
	for (const auto& current : m_sprites)
	{
		if (current.first)
		{
			current.first->render(current.second, glm::vec2(BLOCK_SIZE), 0.f);
		}
	}
}

void FightScreen::update(const double delta)
{
}

void FightScreen::processInput(std::array<bool, 349> keys)
{
}