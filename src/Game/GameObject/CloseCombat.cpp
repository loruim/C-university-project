#include "CloseCombat.h"

#include "IGameObject.h"
#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"
#include "Arrow.h"
#include "../../Physics/PhysicsEngine.h"	

const std::string& CloseCombat::getCloseCombatSpriteFromType(const ECloseCombatUnitType eType)
{
	return CloseCombatTypeString[static_cast<size_t>(eType)];
}

CloseCombat::CloseCombat(const CloseCombat::ECloseCombatUnitType eType,
						 const double maxVelocity,
						 const glm::vec2& position,
						 const glm::vec2& size,
						 const float layer)
	: IGameObject(IGameObject::EObjectType::Hero, position, size, 0.f, layer)
	, m_eOrientation(ECloseUnitOrientaition::Left)
	, m_eType(eType)
	, m_pSprite_top(ResourceManager::getSprite(getCloseCombatSpriteFromType(eType) + "_Left"))
	, m_pSprite_bottom(ResourceManager::getSprite(getCloseCombatSpriteFromType(eType) + "_Right"))
	, m_pSprite_left(ResourceManager::getSprite(getCloseCombatSpriteFromType(eType) + "_Left"))
	, m_pSprite_right(ResourceManager::getSprite(getCloseCombatSpriteFromType(eType) + "_Right"))
	, m_spriteAnimator_top(m_pSprite_top)
	, m_spriteAnimator_bottom(m_pSprite_bottom)
	, m_spriteAnimator_left(m_pSprite_left)
	, m_spriteAnimator_right(m_pSprite_right)
	, m_maxVelocity(maxVelocity)
	, m_leftBottomPossibleMove(0)
	, m_rightTopPossibleMove(0)
{
	switch (m_eType)
	{
	case CloseCombat::ECloseCombatUnitType::knight:
		m_closeSpeed = 2;
		break;
	case CloseCombat::ECloseCombatUnitType::barbarian:
		m_closeSpeed = 3;
		break;
	case CloseCombat::ECloseCombatUnitType::angel:
		m_closeSpeed = 5;
		break;
	}
}

void CloseCombat::render() const
{
	switch (m_eOrientation)
	{
	case CloseCombat::ECloseUnitOrientaition::Top:
		m_pSprite_top->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_top.getCurrentFrame());
		break;
	case CloseCombat::ECloseUnitOrientaition::Bottom:
		m_pSprite_bottom->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_bottom.getCurrentFrame());
		break;
	case CloseCombat::ECloseUnitOrientaition::Left:
		m_pSprite_left->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_left.getCurrentFrame());
		break;
	case CloseCombat::ECloseUnitOrientaition::Right:
		m_pSprite_right->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_right.getCurrentFrame());
		break;
	}
}

void CloseCombat::SetOrientation(const ECloseUnitOrientaition eOrientation)
{
	if (m_eOrientation == eOrientation)
	{
		return;
	}

	m_eOrientation = eOrientation;
	switch (m_eOrientation)
	{
	case CloseCombat::ECloseUnitOrientaition::Top:
		m_direction.x = 0;
		m_direction.y = 1;
		break;
	case CloseCombat::ECloseUnitOrientaition::Bottom:
		m_direction.x = 0;
		m_direction.y = -1;
		break;
	case CloseCombat::ECloseUnitOrientaition::Left:
		m_direction.x = -1;
		m_direction.y = 0;
		break;
	case CloseCombat::ECloseUnitOrientaition::Right:
		m_direction.x = 1;
		m_direction.y = 0;
		break;
	}
}

void CloseCombat::update(const double delta)
{
	if (m_velocity > 0)
	{
		switch (m_eOrientation)
		{
		case CloseCombat::ECloseUnitOrientaition::Top:
			m_spriteAnimator_top.update(delta);
			break;
		case CloseCombat::ECloseUnitOrientaition::Bottom:
			m_spriteAnimator_bottom.update(delta);
			break;
		case CloseCombat::ECloseUnitOrientaition::Left:
			m_spriteAnimator_left.update(delta);
			break;
		case CloseCombat::ECloseUnitOrientaition::Right:
			m_spriteAnimator_right.update(delta);
			break;
		}
	}
}