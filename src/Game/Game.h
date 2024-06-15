#pragma once

#include <glm/vec2.hpp>
#include <array>
#include <memory>

class Hero;
class Level;
class ShopScreen;

class Game
{
public:
	Game(const glm::uvec2& windowSize);
	~Game();

	void render();
	void update(const double delta);
	void setKey(const int key, const int action);
	bool init();
	unsigned int getCurrentWidth() const;
	unsigned int getCurrentHeight() const;

private:
	enum class EGameState {
		FightScreen,
		GlobalMap,
		LoseScreen,
		Pause,
		ShopScreen,
		WinnerScreen
	};

	std::array<bool, 349> m_keys;

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
	std::shared_ptr<Hero> m_pHero;
	std::shared_ptr<Level> m_pLevel;
	std::shared_ptr<ShopScreen> m_pShopsScreen;
};