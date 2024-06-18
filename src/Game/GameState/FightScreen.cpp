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

FightScreen::FightScreen(const std::vector<std::string>& fightScreenDescription, Game* pGame, std::array<bool, 6> whatUnitHave)
	: m_pGame(pGame)
	, m_mousePosition(glm::vec2(0))
	, m_mousePositionNorm(glm::vec2(0))
	, m_whatUnitHave(std::move(whatUnitHave))
	, m_mouseReleased(true)
	, m_TopClosesPosition(0)
	, m_BottomClosesPosition(0)
	, m_LeftClosesPosition(0)
	, m_RightClosesPosition(0)
	, m_knightTurn(true)
	, m_angelTurn(true)
	, m_barbarianTurn(true)
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

	for (const auto& combatCurrent : m_pCloseCombat)
	{
		combatCurrent->render();
	}
	//m_pCloseCombat[0]->render();
	//m_pDistantCombat->render();
	for (const auto& distantCurrent : m_pDistantCombat)
	{
		distantCurrent->render();
	}
}

void FightScreen::update(const double delta)
{
	//m_pCloseCombat[0]->update(delta);
	for (const auto& combatCurrent : m_pCloseCombat)
	{
		combatCurrent->update(delta);
		PossibleMove(combatCurrent->getCloseSpeed(), combatCurrent->getLeftBottomPossibleMove(), combatCurrent->getRightTopPossibleMove(), combatCurrent->getCurrentPosition());
		
	}

	for (const auto& distantCurrent : m_pDistantCombat)
	{
		distantCurrent->update(delta);
		PossibleMove(distantCurrent->getDistantSpeed(), distantCurrent->getLeftBottomPossibleMove(), distantCurrent->getRightTopPossibleMove(), distantCurrent->getCurrentPosition());
	}
	//m_pDistantCombat->update(delta);
	//PossibleMove(m_distantSpeed);
}

void FightScreen::initPhysics()
{
	//if (Hero::isHaveAngel())
	if (m_whatUnitHave[0])
	{
		m_pCloseCombat.push_back(std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::angel, 0.05, getEnemyRespawn_3(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
	}

	if (m_whatUnitHave[5])
	{
		m_pDistantCombat.push_back(std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::titan, 0.05, getEnemyRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
	}

	if (m_whatUnitHave[4])
	{
		m_pDistantCombat.push_back(std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::magican, 0.05, getEnemyRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
	}

	if (m_whatUnitHave[2])
	{
		m_pCloseCombat.push_back(std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::barbarian, 0.05, getAlliesRespawn_1(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
	}

	if (m_whatUnitHave[3])
	{
		m_pCloseCombat.push_back(std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::knight, 0.05, getAlliesRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
	}

	if (m_whatUnitHave[1])
	{
		m_pDistantCombat.push_back(std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::archer, 0.05, getAlliesRespawn_3(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
	}


	for (const auto& combatCurrent : m_pCloseCombat)
	{
		Physics::PhysicsEngine::addDynamicGameObject(combatCurrent);
	}

	for (const auto& distantCurrent : m_pDistantCombat)
	{
		Physics::PhysicsEngine::addDynamicGameObject(distantCurrent);
	}
}

void FightScreen::processInputKey(std::array<bool, 349> keys)
{
	if (keys[GLFW_KEY_ENTER])
	{
		m_pGame->startGlobalMap();
	}
}
void FightScreen::processInputMouse(std::array<bool, 8> mouseButtons)
{
	if (!mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
	{
		m_mouseReleased = true;
	}

	
	if (m_mouseReleased)
	{
		for (const auto closeCurrent : m_pCloseCombat)
		{
			if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
			{
				m_mouseReleased = false;
				if (closeCurrent->getEType() == CloseCombat::ECloseCombatUnitType::angel)
				{
					if (m_angelTurn)
					{
						PossibleMove(closeCurrent->getCloseSpeed(), closeCurrent->getLeftBottomPossibleMove(), closeCurrent->getRightTopPossibleMove(), closeCurrent->getCurrentPosition());
						closeCurrent->SetOrientation(CloseCombat::ECloseUnitOrientaition::Top);
						m_LeftClosesPosition = closeCurrent->getLeftBottomPossibleMove();
						m_RightClosesPosition = closeCurrent->getRightTopPossibleMove();
						if (m_mousePosition.x > m_LeftClosesPosition.x && m_mousePosition.x < m_RightClosesPosition.x
							&& m_mousePosition.y > m_LeftClosesPosition.y && m_mousePosition.y < m_RightClosesPosition.y)
						{
							m_angelTurn = false;
							m_mousePositionNorm = m_mousePosition;
							while (static_cast<unsigned int>(m_mousePositionNorm.x) % BLOCK_SIZE != 0)
							{
								--m_mousePositionNorm.x;
							}
							while (static_cast<unsigned int>(m_mousePositionNorm.y) % BLOCK_SIZE != 0)
							{
								++m_mousePositionNorm.y;
							}
							closeCurrent->SetPosition(m_mousePositionNorm);
							break;
						}
					}
				}
				else if (closeCurrent->getEType() == CloseCombat::ECloseCombatUnitType::knight)
				{
					if (m_knightTurn)
					{

						closeCurrent->SetOrientation(CloseCombat::ECloseUnitOrientaition::Top);
						m_LeftClosesPosition = closeCurrent->getLeftBottomPossibleMove();
						m_RightClosesPosition = closeCurrent->getRightTopPossibleMove();
						if (m_mousePosition.x > m_LeftClosesPosition.x && m_mousePosition.x < m_RightClosesPosition.x
							&& m_mousePosition.y > m_LeftClosesPosition.y && m_mousePosition.y < m_RightClosesPosition.y)
						{
							m_mousePositionNorm = m_mousePosition;
							m_knightTurn = false;
							while (static_cast<unsigned int>(m_mousePositionNorm.x) % BLOCK_SIZE != 0)
							{
								--m_mousePositionNorm.x;
							}
							while (static_cast<unsigned int>(m_mousePositionNorm.y) % BLOCK_SIZE != 0)
							{
								++m_mousePositionNorm.y;
							}
							closeCurrent->SetPosition(m_mousePositionNorm);
							break;
						}
					}
				}
			}
		}
	}
	

	if (!m_angelTurn && !m_knightTurn)
	{
		m_angelTurn = true;
		m_knightTurn = true;
	}
	/*for (const auto distantCurrent : m_pDistantCombat)
	{
		if (m_mouseReleased)
		{
			if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
			{
				m_mouseReleased = false;
				distantCurrent->SetOrientation(DistantCombat::EDistantUnitOrientaition::Bottom);
				m_LeftClosesPosition = distantCurrent->getLeftBottomPossibleMove();
				m_RightClosesPosition = distantCurrent->getRightTopPossibleMove();
				if (m_mousePosition.x > m_LeftClosesPosition.x && m_mousePosition.x < m_RightClosesPosition.x
					&& m_mousePosition.y > m_LeftClosesPosition.y && m_mousePosition.y < m_RightClosesPosition.y)
				{
					m_mousePositionNorm = m_mousePosition;
					while (static_cast<unsigned int>(m_mousePositionNorm.x) % BLOCK_SIZE != 0)
					{
						--m_mousePositionNorm.x;
					}
					while (static_cast<unsigned int>(m_mousePositionNorm.y) % BLOCK_SIZE != 0)
					{
						++m_mousePositionNorm.y;
					}
					distantCurrent->SetPosition(m_mousePositionNorm);
				}
			}
		}
	}*/
}

void FightScreen::processSetMousePosition(double mouseX, double mouseY)
{
	m_mousePosition = { mouseX, mouseY }; //639, 767, // 0, 764 // 0, 0 --> слева сверху находится 0
	/*m_mousePosition.x = ceil(m_mousePosition.x / 4 - BLOCK_SIZE / 2);
	m_mousePosition.y = ceil(m_mousePosition.y / 4 - BLOCK_SIZE / 2);*/
	m_mousePosition.x = ceil(m_mousePosition.x / 4);
	m_mousePosition.y = ceil(m_mousePosition.y / 4);

	m_mousePosition.y = static_cast<double>(STARTSCREEN_HEIGHT) - m_mousePosition.y - BLOCK_SIZE;
}

void FightScreen::PossibleMove(const uint8_t speed, glm::vec2& leftBottom, glm::vec2& rightTop, const glm::vec2& currentPosition)
{
	rightTop.y = currentPosition.y + speed * BLOCK_SIZE;
	if (rightTop.y > RIGHT_BOTTOM_FIELD.y)
	{
		rightTop.y = RIGHT_BOTTOM_FIELD.y;
	}

	rightTop.x = currentPosition.x + speed * BLOCK_SIZE + BLOCK_SIZE / 2;
	if (rightTop.x > RIGHT_BOTTOM_FIELD.x)
	{
		rightTop.x = RIGHT_BOTTOM_FIELD.x;
	}

	leftBottom.y = currentPosition.y - speed * BLOCK_SIZE - BLOCK_SIZE / 2;
	if (leftBottom.y < LEFT_BOTTOM_FIELD.y)
	{
		leftBottom.y = LEFT_BOTTOM_FIELD.y;
	}

	leftBottom.x = currentPosition.x - speed * BLOCK_SIZE;
	if (leftBottom.x < LEFT_BOTTOM_FIELD.x)
	{
		leftBottom.x = LEFT_BOTTOM_FIELD.x;
	}
}