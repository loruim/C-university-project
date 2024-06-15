#include "ShopScreen.h"

#include <iostream>

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"
#include "../Game.h"

#include <GLFW/glfw3.h>

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

ShopScreen::ShopScreen(const std::vector<std::string>& shopScreenDescription, Game* pGame)
	: m_pGame(pGame)
	, m_keyReleased(true)
	, m_currentMenuSelection(0)
	, m_menuSprite(std::make_pair(ResourceManager::getSprite("castleShopsSprite"), glm::vec2(6 * BLOCK_SIZE, STARTSCREEN_HEIGHT - shopScreenDescription.size() * BLOCK_SIZE - MENU_HEIGHT - 9 * BLOCK_SIZE)))
	, m_pointerSprite(std::make_pair(ResourceManager::getSprite("pointerAnimation"), glm::vec2(6 * BLOCK_SIZE + MENU_WIDTH * 2, m_menuSprite.second.y + MENU_HEIGHT + 2 * BLOCK_SIZE - m_currentMenuSelection * MENU_HEIGHT * 2)))
	, m_pointerSpriteAnimator(m_pointerSprite.first)
{
	if (shopScreenDescription.empty())
	{
		std::cerr << "Empty shop screen description" << std::endl;
	}

	auto leftOffsetPixel = BLOCK_SIZE * 3;
	auto bottomOffset = STARTSCREEN_HEIGHT - BLOCK_SIZE * 2;

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

unsigned int ShopScreen::getStateWidth() const
{
	return STARTSCREEN_WIDTH;
}

unsigned int ShopScreen::getStateHeight() const
{
	return STARTSCREEN_HEIGHT;
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
	m_menuSprite.first->render(m_menuSprite.second, glm::vec2(MENU_WIDTH * 2, MENU_HEIGHT* 2), 0.f);
	m_pointerSprite.first->render(glm::vec2(m_pointerSprite.second.x, m_pointerSprite.second.y - m_currentMenuSelection * MENU_HEIGHT), glm::vec2(POINTER_SIZE * 2), 0.f, 0.f, m_pointerSpriteAnimator.getCurrentFrame());
}

void ShopScreen::update(const double delta)
{
	m_pointerSpriteAnimator.update(delta);
}

void ShopScreen::processInput(std::array<bool, 349> keys)
{
	if (!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S])
	{
		m_keyReleased = true;
	}

	if (m_keyReleased)
	{
		if (keys[GLFW_KEY_W])
		{
			m_keyReleased = false;
			--m_currentMenuSelection;
			if (m_currentMenuSelection < 0)
			{
				m_currentMenuSelection = 1;
			}
		}
		else if (keys[GLFW_KEY_S])
		{
			m_keyReleased = false;
			++m_currentMenuSelection;
			if (m_currentMenuSelection > 1)
			{
				m_currentMenuSelection = 0;
			}
		}
	}

	if (keys[GLFW_KEY_ENTER]) // Сделаьб обработку покупок
	{
		switch (m_currentMenuSelection)
		{
		case 0:
			m_pGame->startGlobalMap();
			break;
		default:
			break;
		}
	}

	if (keys[GLFW_KEY_Q])
	{
		m_pGame->startGlobalMap();
	}
}