#pragma once

#include "IGameState.h"	
#include "../../Renderer/SpriteAnimator.h"

#include <vector>
#include <array>
#include <string>
#include <memory>
#include <glm/vec2.hpp>

namespace RenderEngine
{
	class Sprite;
}

class FightScreen : public IGameState
{
public:
	static constexpr unsigned int BLOCK_SIZE = 16;
	static constexpr unsigned int STARTSCREEN_WIDTH = 10 * BLOCK_SIZE;
	static constexpr unsigned int STARTSCREEN_HEIGHT = 12 * BLOCK_SIZE;

	FightScreen(const std::vector<std::string>& fightScreenDescription);
	virtual void render() const override;
	virtual void update(const double delta) override;
	virtual void processInput(std::array<bool, 349> keys) override;

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;

private:
	// Game* m_pGame;
	// int m_currentMenuSelection;
	// bool m_keyReleased;
	std::vector<std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2>> m_sprites;
	// std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_menuSprite;
	// std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_pointerSprite;
	// RenderEngine::SpriteAnimator m_pointerSpriteAnimator;
	// size_t m_sizeShopScreenDescription;
};