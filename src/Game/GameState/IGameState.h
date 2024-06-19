#pragma once

#include <array>

/**
* @file IGameState.h
* File containing the main functions of the game state
*/

/**
* @class IGameState
* @brief game state class
* 
* Virtual class. All classes that describe the current state of the game are inherited from it, such as Level, ShopScreen, FightScreen
*/
class IGameState
{
public:

	/**
	* A pure virtual member. Responsible for rendering in the current frame
	* @see update()
	* @see processInputKey()
	* @see processInputMouse()
	* @see processSetMousePosition()
	* @see initPhysics()
	* @see getStateWidth()
	* @see getStateHeight()
	*/
	virtual void render() const = 0;

	/**
	* A pure virtual member. Responsible for updating in the current frame
	* @see render()
	* @see processInputKey()
	* @see processInputMouse()
	* @see processSetMousePosition()
	* @see initPhysics()
	* @see getStateWidth()
	* @see getStateHeight()
	* @param[in] delta update time.
	*/
	virtual void update(const double delta) = 0;

	/**
	* A basic virtual destructor.
	*/
	virtual ~IGameState() = default;

	/**
	* A virtual member.
	* @see render()
	* @see update()
	* @see processInputMouse()
	* @see processSetMousePosition()
	* @see initPhysics()
	* @see getStateWidth()
	* @see getStateHeight()
	* @param[in] keys array of key numbers.
	*/
	virtual void processInputKey(std::array<bool, 349> keys) {};

	/**
	* A virtual member.
	* @see render()
	* @see update()
	* @see processInputKey()
	* @see processSetMousePosition()
	* @see initPhysics()
	* @see getStateWidth()
	* @see getStateHeight()
	* @param[in] mouseButtons array of mouse button numbers.
	*/
	virtual void processInputMouse(std::array<bool, 8> mouseButtons) {};

	/**
	* A virtual member.
	* @see render()
	* @see update()
	* @see processInputKey()
	* @see processInputMouse()
	* @see initPhysics()
	* @see getStateWidth()
	* @see getStateHeight()
	* @param[in] mouseX mouse X coordinates.
	* @param[in] mouseY mouse Y coordinates.
	*/
	virtual void processSetMousePosition(double mouseX, double mouseY) {};

	/**
	* A virtual member.
	* @see render()
	* @see update()
	* @see processInputKey()
	* @see processInputMouse()
	* @see processSetMousePosition()
	* @see getStateWidth()
	* @see getStateHeight()
	*/
	virtual void initPhysics() {};
 
	/**
	* A pure virtual member.
	* @see render()
	* @see update()
	* @see processInputKey()
	* @see processInputMouse()
	* @see processSetMousePosition()
	* @see initPhysics()
	* @see getStateHeight()
	* @return Screen width
	*/
	virtual unsigned int getStateWidth() const = 0;

	/**
	* A pure virtual member.
	* @see render()
	* @see update()
	* @see processInputKey()
	* @see processInputMouse()
	* @see processSetMousePosition()
	* @see initPhysics()
	* @see getStateWidth()
	* @return Screen height
	*/
	virtual unsigned int getStateHeight() const = 0;
};