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

	static constexpr glm::vec2 LEFT_BOTTOM_FIELD = { 2 * BLOCK_SIZE, BLOCK_SIZE };
	/*static constexpr unsigned int LEFT_BOTTOM_FIELD_HEIGHT = BLOCK_SIZE;*/
	static constexpr  glm::vec2 RIGHT_BOTTOM_FIELD = { 6 * BLOCK_SIZE, 10 * BLOCK_SIZE };
	//static constexpr unsigned int RIGHT_BOTTOM_FIELD_HEIGHT = 10 * BLOCK_SIZE;

	FightScreen(const std::vector<std::string>& fightScreenDescription, Game* pGame, std::vector<bool> whatUnitHave);
	virtual void render() const override;
	virtual void update(const double delta) override;
	virtual void processInputKey(std::array<bool, 349> keys) override;
	virtual void processInputMouse(std::array<bool, 8> mouseButtons) override;
	virtual void processSetMousePosition(double mouseX, double mouseY) override;
	virtual void initPhysics() override;

	const glm::ivec2& getAlliesRespawn_1() const { return m_alliesRespawn_1; }
	const glm::ivec2& getAlliesRespawn_2() const { return m_alliesRespawn_2; }
	const glm::ivec2& getAlliesRespawn_3() const { return m_alliesRespawn_3; }
	const glm::ivec2& getEnemyRespawn_1() const { return m_enemyRespawn_1; }
	const glm::ivec2& getEnemyRespawn_2() const { return m_enemyRespawn_2; }
	const glm::ivec2& getEnemyRespawn_3() const { return m_enemyRespawn_3; }

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;

private:
	Game* m_pGame;
	std::vector<std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2>> m_sprites;
	std::vector<std::shared_ptr<CloseCombat>>   m_pCloseCombat;
	std::shared_ptr<DistantCombat> m_pDistantCombat;

	glm::ivec2 m_alliesRespawn_1;
	glm::ivec2 m_alliesRespawn_2;
	glm::ivec2 m_alliesRespawn_3;
	glm::ivec2 m_enemyRespawn_1;
	glm::ivec2 m_enemyRespawn_2;
	glm::ivec2 m_enemyRespawn_3;

	glm::vec2 m_mousePosition;
	
	std::vector<bool> m_whatUnitHave;
};