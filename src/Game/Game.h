#pragma once

#include <glm/vec2.hpp>
#include <array>
#include <memory>
#include <vector>

/**
* @file Game.h
* A file that changes the state of the game depending on conditions
*/

class IGameState;

namespace RenderEngine
{
	class ShaderProgram;
}

class Hero;

/**
* @class Game
* @brief game class
*
* A class that calls the main functions of game states and changes these states depending on conditions
*/
class Game
{
public:

	/**
	* A constructor.
	* @param[in] windowSize glm::uvec2&
	*/
	Game(const glm::uvec2& windowSize);
	~Game();

	/**
	* A member. Responsible for rendering in the current frame
	*/
	void render();

	/**
	* A member. Responsible for updating in the current frame
	* @see getCurrentWidth()
	* @see getCurrentHeight()
	* @param[in] delta Double update time.
	*/
	void update(const double delta);

	/**
	* A member.
	* @param key Int, shows the key number
	* @param action Int, maps a key number to a key
	*/
	void setKey(const int key, const int action);

	/**
	* A member.
	* @param button Int, shows the button number
	* @param action Int, maps a button number to a button
	*/
	void setButton(const int button, const int action);

	/**
	* A member.
	* @param[in] xpos Double, mouse X coordinates
	* @param[in] ypos Double, mouse Y coordinates
	*/
	void setMousePosition(const double xpos, const double ypos);

	/**
	* A member.
	* @return true if the game started successfully, else if there was an error
	*/
	bool init();

	/**
	* A const member.
	* @return screen Width
	*/
	unsigned int getCurrentWidth() const;

	/**
	* A const member.
	* @return screen Height
	*/
	unsigned int getCurrentHeight() const;

	/**
	* A member. Launches global map
	*/
	void startGlobalMap();

	/**
	* A member. Launches fight map
	* @param[in] enemy Ptr, Indicates the data of the enemy with whom the battle took place
	*/
	void startFightMap(std::shared_ptr<Hero> enemy);

	/**
	* A member. Launches shop map
	* @param[in] shopNumber Size_t, Indicates the store number
	*/
	void startShopScreen(const size_t shopNumber);

	/**
	* A member. Adjusts the map to screen sizes
	*/
	void updateViewport();

	/**
	* A member. Sets screen dimensions
	* @param[in] windowSize glm::uvec2, Contains new screen sizes
	*/
	void setWindowSize(const glm::uvec2& windowSize);

	/**
	* A member. Shows the presence of the left enemy on the map
	* @param[in] isLive bool, shows the state of the left opponent
	*/
	void setEnemyLeftLive(bool isLive) { m_enemyLeftLive = isLive; }

	/**
	* A member. Shows the presence of the right enemy on the map
	* @param[in] isLive bool, shows the state of the right opponent
	*/
	void setEnemyRightLive(bool isLive) { m_enemyRightLive = isLive; }

	/**
	* A member. Shows the presence of the middle enemy on the map
	* @param[in] isLive bool, shows the state of the middle opponent
	*/
	void setEnemyMiddleLive(bool isLive) { m_enemyMiddleLive = isLive; }

private:

	/**
	* An enum.
	* Contains the name of the game states
	*/
	enum class EGameState {
		FightScreen, ///< enum value FightScreen.
		GlobalMap, ///< enum value GlobalMap.
		ShopScreen, ///< enum value ShopScreen.
	};

	std::array<bool, 349> m_keys;
	std::array<bool, 8> m_mouseButtons;
	double m_mouseX;
	double m_mouseY;

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState; ///< enum variable. Contains information about the game state
	std::shared_ptr<IGameState> m_pCurrentGameState; ///< Pointer to the game state, allows you to access methods of the IGameState class
	std::shared_ptr<RenderEngine::ShaderProgram> m_pSpriteShaderProgram; ///< Pointer on shader

	std::array<bool, 6> m_availableUnits; ///< Player unit array

	bool m_enemyLeftLive;
	bool m_enemyRightLive;
	bool m_enemyMiddleLive;
};