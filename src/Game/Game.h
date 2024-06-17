#pragma once

#include <glm/vec2.hpp>
#include <array>
#include <memory>

class IGameState;

namespace RenderEngine
{
	class ShaderProgram;
}

class Game
{
public:
	Game(const glm::uvec2& windowSize);
	~Game();

	void render();
	void update(const double delta);
	void setKey(const int key, const int action);
	void setButton(const int button, const int action);
	void setMousePosition(const double xpos, const double ypos);
	bool init();
	unsigned int getCurrentWidth() const;
	unsigned int getCurrentHeight() const;
	void startGlobalMap();
	void startFightMap();
	void startShopScreen(const size_t shopNumber);
	void updateViewport();
	void setWindowSize(const glm::uvec2& windowSize);

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
	std::array<bool, 8> m_mouseButtons;
	double m_mouseX;
	double m_mouseY;

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
	std::shared_ptr<IGameState> m_pCurrentGameState;
	std::shared_ptr<RenderEngine::ShaderProgram> m_pSpriteShaderProgram;
};