#include "Game.h"

#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Game::Game(const glm::ivec2& windowSize) : m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize)
{
	m_keys.fill(false);
}

Game::~Game()
{

}

void Game::render()
{
	ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
}

void Game::update(const uint64_t delta)
{
	ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
}

void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}

bool Game::init()
{
	auto pDefaultShaderProgram = ResourceManager::loadShaderProgram("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
	if (!pDefaultShaderProgram)
	{
		std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
		return false;
	}

	auto pSpriteShaderProgram = ResourceManager::loadShaderProgram("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
	if (!pSpriteShaderProgram)
	{
		std::cerr << "Can't create sprite: " << "SpriteShader" << std::endl;
		return false;
	}

	auto tex = ResourceManager::loadTexture("DefaultTexture", "res/texture/battleCity.png");

	std::vector<std::string> subTexturesName = {
		"topYellowTankOne",
		"topYellowTankTwo",
		"leftYellowTankOne",
		"leftYellowTankTwo",
		"bottomYellowTankOne",
		"bottomYellowTankTwo",
		"RightYellowTankOne",
		"RightYellowTankTwo",
		"topWhiteTankOne",
		"topWhiteTankTwo",
		"leftWhiteTankOne",
		"leftWhiteTankTwo",
		"bottomWhiteTankOne",
		"bottomWhiteTankTwo",
		"RightWhiteTankOne",
		"RightWhiteTankTwo",
		"block",
		"rightBlock",
		"bottomBlock",
		"leftBlock",
		"topBlock" };

	auto pTexturesAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/texture/battleCity.png", std::move(subTexturesName), 16, 16);

	auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "leftWhiteTankOne");

	pAnimatedSprite->setPosition(glm::vec2(300, 300));

	std::vector<std::pair<std::string, uint16_t>> blockState;
	blockState.emplace_back(std::make_pair<std::string, uint16_t>("block", 1000000000));
	blockState.emplace_back(std::make_pair<std::string, uint16_t>("rightBlock", 1000000000));
	blockState.emplace_back(std::make_pair<std::string, uint16_t>("bottomBlock", 1000000000));

	std::vector<std::pair<std::string, uint16_t>> tankState;
	tankState.emplace_back(std::make_pair<std::string, uint16_t>("topYellowTankOne", 1000000000));
	tankState.emplace_back(std::make_pair<std::string, uint16_t>("topYellowTankTwo", 1000000000));

	pAnimatedSprite->insertState("blockState", std::move(blockState));
	pAnimatedSprite->insertState("tankState", std::move(tankState));

	pAnimatedSprite->setState("blockState");

	pDefaultShaderProgram->use();
	pDefaultShaderProgram->setInt("tex", 0);

	glm::mat4 modelMatrix_1 = glm::mat4(1.f);
	modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

	glm::mat4 modelMatrix_2 = glm::mat4(1.f);
	modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

	pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

	pSpriteShaderProgram->use();
	pSpriteShaderProgram->setInt("tex", 0);
	pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

	return true;
}