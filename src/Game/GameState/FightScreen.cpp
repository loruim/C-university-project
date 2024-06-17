#include "FightScreen.h"

#include <iostream>

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"
#include "../GameObject/CloseCombat.h"
#include "../GameObject/DistantCombat.h"
#include "../GameObject/Hero.h"
#include "../GameState/Level.h"
#include "../Game.h"

#include <GLFW/glfw3.h>

std::shared_ptr<RenderEngine::Sprite> getFightObjectForDescription(const char description)
{
	switch (description)
	{
	case '8':
		return ResourceManager::getSprite("inaccessibleField");
	case '5':
		return nullptr;
	default:
		std::cerr << "Unknown shops description" << description << std::endl;
	}
	return nullptr;
}

FightScreen::FightScreen(const std::vector<std::string>& fightScreenDescription, Game* pGame, std::vector<bool> whatUnitHave) : m_pGame(pGame), m_mousePosition(glm::vec2(0)), m_whatUnitHave(std::move(whatUnitHave))
{
	if (fightScreenDescription.empty())
	{
		std::cerr << "Empty fight screen description" << std::endl;
	}

	auto leftOffsetPixel = BLOCK_SIZE * 2;
	auto bottomOffset = STARTSCREEN_HEIGHT - BLOCK_SIZE * 2;

	m_alliesRespawn_1 = { 2 * BLOCK_SIZE, BLOCK_SIZE };
	m_alliesRespawn_2 = { 4 * BLOCK_SIZE, BLOCK_SIZE };
	m_alliesRespawn_3 = { 6 * BLOCK_SIZE, BLOCK_SIZE };
	m_enemyRespawn_1 = { 2 * BLOCK_SIZE, 10 * BLOCK_SIZE };
	m_enemyRespawn_2 = { 4 * BLOCK_SIZE, 10 * BLOCK_SIZE };
	m_enemyRespawn_3 = { 6 * BLOCK_SIZE, 10 * BLOCK_SIZE };

	unsigned int currentBottomOffset = bottomOffset;
	for (const std::string& currentRow : fightScreenDescription)
	{
		unsigned int currentLeftOffset = leftOffsetPixel;
		for (const char currentElement : currentRow)
		{
			m_sprites.emplace_back(std::make_pair(getFightObjectForDescription(currentElement), glm::vec2(currentLeftOffset, currentBottomOffset)));
			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}
}

unsigned int FightScreen::getStateWidth() const
{
	return STARTSCREEN_WIDTH;
}

unsigned int FightScreen::getStateHeight() const
{
	return STARTSCREEN_HEIGHT;
}

void FightScreen::render() const
{
	for (const auto& current : m_sprites)
	{
		if (current.first)
		{
			current.first->render(current.second, glm::vec2(BLOCK_SIZE), 0.f);
		}
	}
	m_pCloseCombat[0]->render();
	m_pDistantCombat->render();
}

void FightScreen::update(const double delta)
{
	m_pCloseCombat[0]->update(delta);
	m_pDistantCombat->update(delta);
}

void FightScreen::initPhysics()
{
	//if (Hero::isHaveAngel())
	if (m_whatUnitHave[0])
	{
		m_pCloseCombat.push_back(std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::angel, 0.05, getEnemyRespawn_1(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
		Physics::PhysicsEngine::addDynamicGameObject(m_pCloseCombat[0]);
	}

	if (m_whatUnitHave[1])
	{
		m_pDistantCombat = std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::archer, 0.05, getEnemyRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f);
		Physics::PhysicsEngine::addDynamicGameObject(m_pDistantCombat);
	}

	if (m_whatUnitHave[2])
	{
		m_pCloseCombat.push_back(std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::angel, 0.05, getEnemyRespawn_1(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
		Physics::PhysicsEngine::addDynamicGameObject(m_pCloseCombat[0]);
	}

	if (m_whatUnitHave[3])
	{
		m_pCloseCombat.push_back(std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::angel, 0.05, getEnemyRespawn_1(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
		Physics::PhysicsEngine::addDynamicGameObject(m_pCloseCombat[0]);
	}

	if (m_whatUnitHave[4])
	{
		m_pDistantCombat = std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::magican, 0.05, getEnemyRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f);
		Physics::PhysicsEngine::addDynamicGameObject(m_pDistantCombat);
	}

	if (m_whatUnitHave[5])
	{
		m_pDistantCombat = std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::titan, 0.05, getEnemyRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f);
		Physics::PhysicsEngine::addDynamicGameObject(m_pDistantCombat);
	}
}

void FightScreen::processInputKey(std::array<bool, 349> keys)
{
	if (keys[GLFW_KEY_W])
	{
		m_pCloseCombat[0]->SetOrientation(CloseCombat::ECloseUnitOrientaition::Top);
		m_pCloseCombat[0]->setVelocity(m_pCloseCombat[0]->getMaxVelocity());

		m_pDistantCombat->SetOrientation(DistantCombat::EDistantUnitOrientaition::Bottom);
		m_pDistantCombat->setVelocity(m_pDistantCombat->getMaxVelocity());
	}
	else if (keys[GLFW_KEY_A])
	{
		m_pCloseCombat[0]->SetOrientation(CloseCombat::ECloseUnitOrientaition::Left);
		m_pCloseCombat[0]->setVelocity(m_pCloseCombat[0]->getMaxVelocity());

		m_pDistantCombat->SetOrientation(DistantCombat::EDistantUnitOrientaition::Right);
		m_pDistantCombat->setVelocity(m_pDistantCombat->getMaxVelocity());
	}
	else if (keys[GLFW_KEY_D])
	{
		m_pCloseCombat[0]->SetOrientation(CloseCombat::ECloseUnitOrientaition::Right);
		m_pCloseCombat[0]->setVelocity(m_pCloseCombat[0]->getMaxVelocity());

		m_pDistantCombat->SetOrientation(DistantCombat::EDistantUnitOrientaition::Left);
		m_pDistantCombat->setVelocity(m_pDistantCombat->getMaxVelocity());
	}
	else if (keys[GLFW_KEY_S])
	{
		m_pCloseCombat[0]->SetOrientation(CloseCombat::ECloseUnitOrientaition::Bottom);
		m_pCloseCombat[0]->setVelocity(m_pCloseCombat[0]->getMaxVelocity());

		m_pDistantCombat->SetOrientation(DistantCombat::EDistantUnitOrientaition::Top);
		m_pDistantCombat->setVelocity(m_pDistantCombat->getMaxVelocity());
	}
	else
	{
		m_pCloseCombat[0]->setVelocity(0);
		m_pDistantCombat->setVelocity(0);
	}

	if (keys[GLFW_KEY_ENTER])
	{
		m_pGame->startGlobalMap();
	}
}
void FightScreen::processInputMouse(std::array<bool, 8> mouseButtons)
{
	if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
	{
		m_pCloseCombat[0]->SetOrientation(CloseCombat::ECloseUnitOrientaition::Top);
		// m_pCloseCombat->setVelocity(m_pCloseCombat->getMaxVelocity());
		m_pCloseCombat[0]->SetPosition(glm::vec2(30.f, 30.f));

		m_pDistantCombat->SetOrientation(DistantCombat::EDistantUnitOrientaition::Bottom);
		//auto pDistantCombat = m_pDistantCombat;
		if (m_mousePosition.x > LEFT_BOTTOM_FIELD.x && m_mousePosition.x < RIGHT_BOTTOM_FIELD.x
			&& m_mousePosition.y > LEFT_BOTTOM_FIELD.y && m_mousePosition.y < RIGHT_BOTTOM_FIELD.y)
		{
			m_pDistantCombat->SetPosition(m_mousePosition);
		}
		
		//m_pDistantCombat->setVelocity(m_pDistantCombat->getMaxVelocity());
	}
}

void FightScreen::processSetMousePosition(double mouseX, double mouseY)
{
	m_mousePosition = { mouseX, mouseY }; //639, 767, // 0, 764 // 0, 0 --> ����� ������ ��������� 0
	m_mousePosition.x = ceil(m_mousePosition.x / 4 - BLOCK_SIZE / 2);
	m_mousePosition.y = ceil(m_mousePosition.y / 4 - BLOCK_SIZE / 2);

	m_mousePosition.y = static_cast<double>(STARTSCREEN_HEIGHT) - m_mousePosition.y - BLOCK_SIZE;
}