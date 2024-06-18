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

FightScreen::FightScreen(const std::vector<std::string>& fightScreenDescription, Game* pGame, std::array<bool, 6> availableUnits, std::shared_ptr<Hero> enemy)
	: m_pGame(pGame)
	, m_mousePosition(glm::vec2(0))
	, m_mousePositionNorm(glm::vec2(0))
	, m_availableUnits(std::move(availableUnits))
	, m_enemy(std::move(enemy))
	, m_mouseLeftReleased(true)
	, m_mouseRightReleased(true)
	, m_TopClosesPosition(0)
	, m_BottomClosesPosition(0)
	, m_LeftClosesPosition(0)
	, m_RightClosesPosition(0)
	, m_knightTurn(false)
	, m_angelTurn(false)
	, m_barbarianTurn(false)
	, m_archerTurn(false)
	, m_magicanTurn(false)
	, m_titanTurn(false)
	, m_enemyKnightTurn(false)
	, m_enemyAngelTurn(false)
	, m_enemyBarbarianTurn(false)
	, m_enemyArcherTurn(false)
	, m_enemyMagicanTurn(false)
	, m_enemyTitanTurn(false)
	, m_playerTurn(true)
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

	for (const auto& distantCurrent : m_pDistantCombat)
	{
		distantCurrent->render();
	}

	if (m_pEnemyCloseCombat != nullptr)
	{
		m_pEnemyCloseCombat->render();
	}

	if (m_pEnemyDistantCombat != nullptr)
	{
		m_pEnemyDistantCombat->render();
	}
}

void FightScreen::update(const double delta)
{
	for (const auto& combatCurrent : m_pCloseCombat)
	{
		combatCurrent->update(delta);
		
	}

	for (const auto& distantCurrent : m_pDistantCombat)
	{
		distantCurrent->update(delta);
	}

	if (m_pEnemyCloseCombat != nullptr)
	{
		m_pEnemyCloseCombat->update(delta);
	}
	
	if (m_pEnemyDistantCombat != nullptr)
	{
		m_pEnemyDistantCombat->update(delta);
	}

	if (m_pEnemyCloseCombat == nullptr && m_pEnemyDistantCombat == nullptr)
	{
		if (m_enemy->getCurrentPosition() == glm::vec2(3 * BLOCK_SIZE, 3 * BLOCK_SIZE + BLOCK_SIZE / 2))
		{
			m_pGame->setEnemyLeftLive(false);
			m_pGame->startGlobalMap();
		}
		
		if (m_enemy->getCurrentPosition() == glm::vec2(4 * BLOCK_SIZE, 2 * BLOCK_SIZE + BLOCK_SIZE / 2))
		{
			m_pGame->setEnemyMiddleLive(false);
			m_pGame->startGlobalMap();
		}

		if (m_enemy->getCurrentPosition() == glm::vec2(5 * BLOCK_SIZE, 3 * BLOCK_SIZE + BLOCK_SIZE / 2))
		{
			m_pGame->setEnemyRightLive(false);
			m_pGame->startGlobalMap();
		}
	}
}

void FightScreen::initPhysics()
{
	if (m_availableUnits[0])
	{
		m_pCloseCombat.push_back(std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::angel, 0.05, getAlliesRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
		m_angelTurn = true;
	}

	if (m_availableUnits[5])
	{
		m_pDistantCombat.push_back(std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::titan, 0.05, getAlliesRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
		m_titanTurn = true;
	}

	if (m_availableUnits[4])
	{
		m_pDistantCombat.push_back(std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::magican, 0.05, getAlliesRespawn_3(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
		m_magicanTurn = true;
	}

	if (m_availableUnits[2])
	{
		m_pCloseCombat.push_back(std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::barbarian, 0.05, getAlliesRespawn_1(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
		m_barbarianTurn = true;
	}

	if (m_availableUnits[3])
	{
		m_pCloseCombat.push_back(std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::knight, 0.05, getAlliesRespawn_3(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
		m_knightTurn = true;
	}

	if (m_availableUnits[1])
	{
		m_pDistantCombat.push_back(std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::archer, 0.05, getAlliesRespawn_1(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f));
		m_archerTurn = true;
	}


	for (const auto& combatCurrent : m_pCloseCombat)
	{
		Physics::PhysicsEngine::addDynamicGameObject(combatCurrent);
	}

	for (const auto& distantCurrent : m_pDistantCombat)
	{
		Physics::PhysicsEngine::addDynamicGameObject(distantCurrent);
	}

	EnemyUnits();
}

void FightScreen::EnemyUnits()
{
	if (m_enemy->isHaveAngel())
	{
		m_pEnemyCloseCombat = std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::angel, 0.05, getEnemyRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f);
		m_enemyAngelTurn = true;
	}

	if (m_enemy->isHaveTitan())
	{
		m_pEnemyDistantCombat = std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::titan, 0.05, getEnemyRespawn_2(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f);
		m_enemyTitanTurn = true;
	}

	if (m_enemy->isHaveMagican())
	{
		m_pEnemyDistantCombat = std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::magican, 0.05, getEnemyRespawn_3(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f);
		m_enemyMagicanTurn = true;
	}

	if (m_enemy->isHaveBarbarian())
	{
		m_pEnemyCloseCombat = std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::barbarian, 0.05, getEnemyRespawn_1(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f);
		m_enemyBarbarianTurn = true;
	}

	if (m_enemy->isHaveKnight())
	{
		m_pEnemyCloseCombat = std::make_shared<CloseCombat>(CloseCombat::ECloseCombatUnitType::knight, 0.05, getEnemyRespawn_3(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f);
		m_enemyKnightTurn = true;
	}

	if (m_enemy->isHaveArcher())
	{
		m_pEnemyDistantCombat = std::make_shared<DistantCombat>(DistantCombat::EDistantCombatUnitType::archer, 0.05, getEnemyRespawn_1(), glm::vec2(FightScreen::BLOCK_SIZE, FightScreen::BLOCK_SIZE), 1.f);
		m_enemyArcherTurn = true;
	}

	Physics::PhysicsEngine::addDynamicGameObject(m_pEnemyCloseCombat);
	Physics::PhysicsEngine::addDynamicGameObject(m_pEnemyDistantCombat);

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
		m_mouseLeftReleased = true;
	}

	if (!mouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
	{
		m_mouseRightReleased = true;
	}

	if (m_playerTurn)
	{
		if (m_mouseLeftReleased)
		{
			for (const auto& closeCurrent : m_pCloseCombat)
			{
				if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
				{
					m_mouseLeftReleased = false;
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
								m_playerTurn = false;
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
								if (m_pEnemyCloseCombat != nullptr)
								{
									if (closeCurrent->getCurrentPosition() == m_pEnemyCloseCombat->getCurrentPosition())
									{
										m_pEnemyCloseCombat = nullptr;
									}
								}

								if (m_pEnemyDistantCombat != nullptr)
								{
									if (closeCurrent->getCurrentPosition() == m_pEnemyDistantCombat->getCurrentPosition())
									{
										m_pEnemyDistantCombat = nullptr;
									}
								}
								break;
							}
						}
					}
					else if (closeCurrent->getEType() == CloseCombat::ECloseCombatUnitType::knight)
					{
						if (m_knightTurn)
						{
							PossibleMove(closeCurrent->getCloseSpeed(), closeCurrent->getLeftBottomPossibleMove(), closeCurrent->getRightTopPossibleMove(), closeCurrent->getCurrentPosition());
							closeCurrent->SetOrientation(CloseCombat::ECloseUnitOrientaition::Top);
							m_LeftClosesPosition = closeCurrent->getLeftBottomPossibleMove();
							m_RightClosesPosition = closeCurrent->getRightTopPossibleMove();
							if (m_mousePosition.x > m_LeftClosesPosition.x && m_mousePosition.x < m_RightClosesPosition.x
								&& m_mousePosition.y > m_LeftClosesPosition.y && m_mousePosition.y < m_RightClosesPosition.y)
							{
								m_mousePositionNorm = m_mousePosition;
								m_knightTurn = false;
								m_playerTurn = false;
								while (static_cast<unsigned int>(m_mousePositionNorm.x) % BLOCK_SIZE != 0)
								{
									--m_mousePositionNorm.x;
								}
								while (static_cast<unsigned int>(m_mousePositionNorm.y) % BLOCK_SIZE != 0)
								{
									++m_mousePositionNorm.y;
								}
								closeCurrent->SetPosition(m_mousePositionNorm);
								if (m_pEnemyCloseCombat != nullptr)
								{
									if (closeCurrent->getCurrentPosition() == m_pEnemyCloseCombat->getCurrentPosition())
									{
										m_pEnemyCloseCombat = nullptr;
									}
								}

								if (m_pEnemyDistantCombat != nullptr)
								{
									if (closeCurrent->getCurrentPosition() == m_pEnemyDistantCombat->getCurrentPosition())
									{
										m_pEnemyDistantCombat = nullptr;
									}
								}
								break;
							}
						}
					}
					else if (closeCurrent->getEType() == CloseCombat::ECloseCombatUnitType::barbarian)
					{
						if (m_barbarianTurn)
						{
							PossibleMove(closeCurrent->getCloseSpeed(), closeCurrent->getLeftBottomPossibleMove(), closeCurrent->getRightTopPossibleMove(), closeCurrent->getCurrentPosition());
							closeCurrent->SetOrientation(CloseCombat::ECloseUnitOrientaition::Top);
							m_LeftClosesPosition = closeCurrent->getLeftBottomPossibleMove();
							m_RightClosesPosition = closeCurrent->getRightTopPossibleMove();
							if (m_mousePosition.x > m_LeftClosesPosition.x && m_mousePosition.x < m_RightClosesPosition.x
								&& m_mousePosition.y > m_LeftClosesPosition.y && m_mousePosition.y < m_RightClosesPosition.y)
							{
								m_mousePositionNorm = m_mousePosition;
								m_barbarianTurn = false;
								m_playerTurn = false;
								while (static_cast<unsigned int>(m_mousePositionNorm.x) % BLOCK_SIZE != 0)
								{
									--m_mousePositionNorm.x;
								}
								while (static_cast<unsigned int>(m_mousePositionNorm.y) % BLOCK_SIZE != 0)
								{
									++m_mousePositionNorm.y;
								}
								closeCurrent->SetPosition(m_mousePositionNorm);
								if (m_pEnemyCloseCombat != nullptr)
								{
									if (closeCurrent->getCurrentPosition() == m_pEnemyCloseCombat->getCurrentPosition())
									{
										m_pEnemyCloseCombat = nullptr;
									}
								}

								if (m_pEnemyDistantCombat != nullptr)
								{
									if (closeCurrent->getCurrentPosition() == m_pEnemyDistantCombat->getCurrentPosition())
									{
										m_pEnemyDistantCombat = nullptr;
									}
								}
								break;
							}
						}
					}
				}
			}
		}


		if (!m_angelTurn && !m_knightTurn && !m_barbarianTurn)
		{
			if (m_availableUnits[0])
			{
				m_angelTurn = true;
			}

			if (m_availableUnits[2])
			{
				m_barbarianTurn = true;
			}

			if (m_availableUnits[3])
			{
				m_knightTurn = true;
			}
		}

		if (m_mouseRightReleased)
		{
			for (const auto& distantCurrent : m_pDistantCombat)
			{
				if (mouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
				{
					m_mouseRightReleased = false;
					if (distantCurrent->getEType() == DistantCombat::EDistantCombatUnitType::magican)
					{
						if (m_magicanTurn)
						{
							PossibleMove(distantCurrent->getDistantSpeed(), distantCurrent->getLeftBottomPossibleMove(), distantCurrent->getRightTopPossibleMove(), distantCurrent->getCurrentPosition());
							distantCurrent->SetOrientation(DistantCombat::EDistantUnitOrientaition::Top);
							m_LeftClosesPosition = distantCurrent->getLeftBottomPossibleMove();
							m_RightClosesPosition = distantCurrent->getRightTopPossibleMove();
							if (m_mousePosition.x > m_LeftClosesPosition.x && m_mousePosition.x < m_RightClosesPosition.x
								&& m_mousePosition.y > m_LeftClosesPosition.y && m_mousePosition.y < m_RightClosesPosition.y)
							{
								m_mousePositionNorm = m_mousePosition;
								m_magicanTurn = false;
								m_playerTurn = false;
								while (static_cast<unsigned int>(m_mousePositionNorm.x) % BLOCK_SIZE != 0)
								{
									--m_mousePositionNorm.x;
								}
								while (static_cast<unsigned int>(m_mousePositionNorm.y) % BLOCK_SIZE != 0)
								{
									++m_mousePositionNorm.y;
								}
								distantCurrent->SetPosition(m_mousePositionNorm);
								break;
							}
						}
					}
					else if (distantCurrent->getEType() == DistantCombat::EDistantCombatUnitType::archer)
					{
						if (m_archerTurn)
						{
							PossibleMove(distantCurrent->getDistantSpeed(), distantCurrent->getLeftBottomPossibleMove(), distantCurrent->getRightTopPossibleMove(), distantCurrent->getCurrentPosition());
							distantCurrent->SetOrientation(DistantCombat::EDistantUnitOrientaition::Top);
							m_LeftClosesPosition = distantCurrent->getLeftBottomPossibleMove();
							m_RightClosesPosition = distantCurrent->getRightTopPossibleMove();
							if (m_mousePosition.x > m_LeftClosesPosition.x && m_mousePosition.x < m_RightClosesPosition.x
								&& m_mousePosition.y > m_LeftClosesPosition.y && m_mousePosition.y < m_RightClosesPosition.y)
							{
								m_mousePositionNorm = m_mousePosition;
								m_archerTurn = false;
								m_playerTurn = false;
								while (static_cast<unsigned int>(m_mousePositionNorm.x) % BLOCK_SIZE != 0)
								{
									--m_mousePositionNorm.x;
								}
								while (static_cast<unsigned int>(m_mousePositionNorm.y) % BLOCK_SIZE != 0)
								{
									++m_mousePositionNorm.y;
								}
								distantCurrent->SetPosition(m_mousePositionNorm);
								break;
							}
						}
					}
					else if (distantCurrent->getEType() == DistantCombat::EDistantCombatUnitType::titan)
					{
						if (m_titanTurn)
						{
							PossibleMove(distantCurrent->getDistantSpeed(), distantCurrent->getLeftBottomPossibleMove(), distantCurrent->getRightTopPossibleMove(), distantCurrent->getCurrentPosition());
							distantCurrent->SetOrientation(DistantCombat::EDistantUnitOrientaition::Top);
							m_LeftClosesPosition = distantCurrent->getLeftBottomPossibleMove();
							m_RightClosesPosition = distantCurrent->getRightTopPossibleMove();
							if (m_mousePosition.x > m_LeftClosesPosition.x && m_mousePosition.x < m_RightClosesPosition.x
								&& m_mousePosition.y > m_LeftClosesPosition.y && m_mousePosition.y < m_RightClosesPosition.y)
							{
								m_mousePositionNorm = m_mousePosition;
								m_titanTurn = false;
								m_playerTurn = false;
								while (static_cast<unsigned int>(m_mousePositionNorm.x) % BLOCK_SIZE != 0)
								{
									--m_mousePositionNorm.x;
								}
								while (static_cast<unsigned int>(m_mousePositionNorm.y) % BLOCK_SIZE != 0)
								{
									++m_mousePositionNorm.y;
								}
								distantCurrent->SetPosition(m_mousePositionNorm);
								break;
							}
						}
					}
				}
			}
		}

		if (!m_archerTurn && !m_magicanTurn && !m_titanTurn)
		{
			if (m_availableUnits[1])
			{
				m_archerTurn = true;
			}

			if (m_availableUnits[5])
			{
				m_titanTurn = true;
			}

			if (m_availableUnits[4])
			{
				m_magicanTurn = true;
			}
		}
	}
	else
	{
		if (m_pEnemyCloseCombat != nullptr)
		{
			if (m_pEnemyCloseCombat->getEType() == CloseCombat::ECloseCombatUnitType::angel)
			{
				if (m_enemyAngelTurn)
				{
					PossibleMove(m_pEnemyCloseCombat->getCloseSpeed(), m_pEnemyCloseCombat->getLeftBottomPossibleMove(), m_pEnemyCloseCombat->getRightTopPossibleMove(), m_pEnemyCloseCombat->getCurrentPosition());
					m_pEnemyCloseCombat->SetOrientation(CloseCombat::ECloseUnitOrientaition::Top);
					m_LeftClosesPosition = m_pEnemyCloseCombat->getLeftBottomPossibleMove();
					while (m_LeftClosesPosition.x < BLOCK_SIZE * 2)
					{
						++m_LeftClosesPosition.x;
					}
					while (m_LeftClosesPosition.y < BLOCK_SIZE)
					{
						++m_LeftClosesPosition.y;
					}
					m_RightClosesPosition = m_pEnemyCloseCombat->getRightTopPossibleMove();
					while (m_RightClosesPosition.x > BLOCK_SIZE * 6)
					{
						--m_RightClosesPosition.x;
					}
					m_enemyAngelTurn = false;
					if (static_cast<unsigned int>(m_LeftClosesPosition.y) % BLOCK_SIZE != 0)
					{
						m_LeftClosesPosition.y = m_LeftClosesPosition.y + BLOCK_SIZE / 2;
					}
					m_pEnemyCloseCombat->SetPosition(glm::vec2(m_RightClosesPosition.x - BLOCK_SIZE  * 2, m_LeftClosesPosition.y));
					m_playerTurn = true;
				}
			}
			/*else if (m_pEnemyCloseCombat->getEType() == CloseCombat::ECloseCombatUnitType::knight)
			{
				if (m_enemyKnightTurn)
				{
					PossibleMove(m_pEnemyCloseCombat->getCloseSpeed(), m_pEnemyCloseCombat->getLeftBottomPossibleMove(), m_pEnemyCloseCombat->getRightTopPossibleMove(), m_pEnemyCloseCombat->getCurrentPosition());
					m_pEnemyCloseCombat->SetOrientation(CloseCombat::ECloseUnitOrientaition::Top);
					m_LeftClosesPosition = m_pEnemyCloseCombat->getLeftBottomPossibleMove();
					while (m_LeftClosesPosition.x < BLOCK_SIZE * 2)
					{
						++m_LeftClosesPosition.x;
					}
					while (m_LeftClosesPosition.y < BLOCK_SIZE)
					{
						++m_LeftClosesPosition.y;
					}
					m_RightClosesPosition = m_pEnemyCloseCombat->getRightTopPossibleMove();
					while (m_RightClosesPosition.x > BLOCK_SIZE * 6)
					{
						--m_RightClosesPosition.x;
					}
					m_enemyAngelTurn = false;
					m_pEnemyCloseCombat->SetPosition(glm::vec2(m_RightClosesPosition.x - BLOCK_SIZE * 2, m_LeftClosesPosition.y + BLOCK_SIZE / 2));
					m_playerTurn = true;
				}
			}*/
		}

		if (!m_enemyAngelTurn && !m_enemyKnightTurn && !m_enemyBarbarianTurn)
		{
			if (m_enemy->isHaveAngel())
			{
				m_enemyAngelTurn = true;
			}

			if (m_enemy->isHaveBarbarian())
			{
				m_enemyKnightTurn = true;
			}

			if (m_enemy->isHaveKnight())
			{
				m_enemyBarbarianTurn = true;
			}
		}
	}
}

void FightScreen::processSetMousePosition(double mouseX, double mouseY)
{
	m_mousePosition = { mouseX, mouseY };
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

	rightTop.x = currentPosition.x + speed * BLOCK_SIZE + static_cast<float>(BLOCK_SIZE / 2);
	if (rightTop.x > RIGHT_BOTTOM_FIELD.x)
	{
		rightTop.x = RIGHT_BOTTOM_FIELD.x;
	}

	leftBottom.y = currentPosition.y - speed * BLOCK_SIZE - static_cast<float>(BLOCK_SIZE / 2);
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