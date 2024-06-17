#pragma once

#include <array>

class IGameState
{
public:
	virtual void render() const = 0;
	virtual void update(const double delta) = 0;
	virtual ~IGameState() = default;
	virtual void processInputKey(std::array<bool, 349> keys) {};
	virtual void processInputMouse(std::array<bool, 8> mouseButtons) {};
	virtual void processSetMousePosition(double mouseX, double mouseY) {};
	virtual void initPhysics() {};
 
	virtual unsigned int getStateWidth() const = 0;
	virtual unsigned int getStateHeight() const = 0;
};