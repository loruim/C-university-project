#pragma once

#include <glm/vec2.hpp>
#include <array>
#include <memory>

class Hero;
class GameField;
class Level;

class Game
{
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void render();
	void update(const double delta);
	void setKey(const int key, const int action);
	bool init();
	size_t getCurrentLevelWidth() const;
	size_t getCurrentLevelHeight() const;

private:
	std::array<bool, 349> m_keys;

	enum EGameState {
		Active,
		Pause
	};

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
	std::unique_ptr<Hero> m_pHero;
	std::unique_ptr<Level> m_pLevel;
};