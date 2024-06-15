#pragma once

#include "IGameState.h"	
#include "../../Renderer/SpriteAnimator.h"

#include <vector>
#include <string>
#include <memory>
#include <glm/vec2.hpp>

namespace RenderEngine
{
	class Sprite;
}

class Game;

class ShopScreen : public IGameState
{
public:

	enum class EShopsType : uint8_t
	{
		CastleShopScreen,
		LeftShopScreen_1,
		LeftShopScreen_2,
		RightShopScreen_1,
		RightShopScreen_2
	};

	static constexpr unsigned int BLOCK_SIZE = 8;
	static constexpr unsigned int STARTSCREEN_WIDTH = 10 * BLOCK_SIZE * 2;
	static constexpr unsigned int STARTSCREEN_HEIGHT = 12 * BLOCK_SIZE * 2;
	static constexpr unsigned int POINTER_SIZE = BLOCK_SIZE * 2;

	ShopScreen(const std::vector<std::string>& shopScreenDescription, Game* pGame);
	virtual void render() const override;
	virtual void update(const double delta) override;
	virtual void processInput(std::array<bool, 349> keys) override;

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;

	void setShopsType(const size_t shopNumber);
	void setMenuSize(const unsigned int newMenuWidth, const unsigned int newMenuHeight);

private:
	unsigned int MENU_WIDTH = 32;
	unsigned int MENU_HEIGHT = 64;
	Game* m_pGame;
	int m_currentMenuSelection;
	bool m_keyReleased;
	std::vector<std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2>> m_sprites;
	std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_menuSprite;
	std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_pointerSprite;
	RenderEngine::SpriteAnimator m_pointerSpriteAnimator;
	EShopsType m_eShopsType;
	size_t m_sizeShopScreenDescription;
};