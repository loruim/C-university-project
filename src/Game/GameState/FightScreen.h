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

class Game;
class CloseCombat;
class DistantCombat;

class FightScreen : public IGameState
{
public:
	static constexpr unsigned int BLOCK_SIZE = 16;
	static constexpr unsigned int STARTSCREEN_WIDTH = 10 * BLOCK_SIZE;
	static constexpr unsigned int STARTSCREEN_HEIGHT = 12 * BLOCK_SIZE;

	FightScreen(const std::vector<std::string>& fightScreenDescription, Game* pGame);
	virtual void render() const override;
	virtual void update(const double delta) override;
	virtual void processInputKey(std::array<bool, 349> keys) override;
	virtual void processInputMouse(std::array<bool, 8> mouseButtons) override;
	virtual void initPhysics() override;

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;

private:
	Game* m_pGame;
	std::vector<std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2>> m_sprites;
	std::shared_ptr<CloseCombat>   m_pCloseCombat;
	std::shared_ptr<DistantCombat> m_pDistantCombat;
};