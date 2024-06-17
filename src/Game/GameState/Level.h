#pragma once

#include <vector>
#include <array>
#include <string>
#include <memory>
#include <set>

#include <glm/vec2.hpp>

#include "IGameState.h"

class IGameObject;
class Hero;
class Game;

class Level : public IGameState
{
public:
	static constexpr unsigned int BLOCK_SIZE = 16;

	Level(const std::vector<std::string>& levelDescription, Game* pGame);
	virtual void render() const override;
	virtual void update(const double delta) override;
	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;
	virtual void processInputKey(std::array<bool, 349> keys) override;

	const glm::ivec2& getPlayerRespawn() const { return m_playerRespawn; }
	const glm::ivec2& getEnemyRespawn_1() const { return m_enemyRespawn_1; }
	const glm::ivec2& getEnemyRespawn_2() const { return m_enemyRespawn_2; }
	const glm::ivec2& getEnemyRespawn_3() const { return m_enemyRespawn_3; }

	std::vector<std::shared_ptr<IGameObject>> getObjectsInArea(const glm::vec2& bottomLeft, const glm::vec2& topRight);
	virtual void initPhysics() override;

	std::shared_ptr<Hero> GetHeroPointer() { return m_pHero; }

private:
	Game* m_pGame;
	size_t m_widthBlocks = 0;
	size_t m_heightBlocks = 0;
	unsigned int m_widhtPixel = 0;
	unsigned int m_heightPixel = 0;

	glm::ivec2 m_playerRespawn;
	glm::ivec2 m_enemyRespawn_1;
	glm::ivec2 m_enemyRespawn_2;
	glm::ivec2 m_enemyRespawn_3;
	std::vector<std::shared_ptr<IGameObject>> m_mapObject;
	std::shared_ptr<Hero> m_pHero;

	bool m_enemyLeftIsActive;
	std::shared_ptr<Hero> m_enemyLeft;

	bool m_enemyMiddleIsActive;
	std::shared_ptr<Hero> m_enemyMiddle;

	bool m_enemyRightIsActive;
	std::shared_ptr<Hero> m_enemyRight;
};