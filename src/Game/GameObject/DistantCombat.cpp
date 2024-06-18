#include "DistantCombat.h"

#include "IGameObject.h"
#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"
#include "Arrow.h"
#include "../../Physics/PhysicsEngine.h"	

uint8_t DistantCombat::m_distantSpeed;

std::string& DistantCombat::getDistantCombatSpriteFromType(const EDistantCombatUnitType eType)
{
	switch (eType)
	{
	case DistantCombat::EDistantCombatUnitType::archer:
		m_distantSpeed = 2;
		break;
	case DistantCombat::EDistantCombatUnitType::magican:
		m_distantSpeed = 3;
		break;
	case DistantCombat::EDistantCombatUnitType::titan:
		m_distantSpeed = 4;
		break;
	default:
		break;
	}
	return DistantCombatTypeString[static_cast<size_t>(eType)];
}

DistantCombat::DistantCombat(const DistantCombat::EDistantCombatUnitType eType,
							 const double maxVelocity,
							 const glm::vec2& position,
							 const glm::vec2& size,
							 const float layer)
	: IGameObject(IGameObject::EObjectType::Hero, position, size, 0.f, layer)
	, m_eOrientation(EDistantUnitOrientaition::Left)
	, m_eType(eType)
	, m_pSprite_top(ResourceManager::getSprite(getDistantCombatSpriteFromType(eType) + "_Left"))
	, m_pSprite_bottom(ResourceManager::getSprite(getDistantCombatSpriteFromType(eType) + "_Right"))
	, m_pSprite_left(ResourceManager::getSprite(getDistantCombatSpriteFromType(eType) + "_Left"))
	, m_pSprite_right(ResourceManager::getSprite(getDistantCombatSpriteFromType(eType) + "_Right"))
	, m_spriteAnimator_top(m_pSprite_top)
	, m_spriteAnimator_bottom(m_pSprite_bottom)
	, m_spriteAnimator_left(m_pSprite_left)
	, m_spriteAnimator_right(m_pSprite_right)
	, m_maxVelocity(maxVelocity)
	, m_leftBottomPossibleMove(0)
	, m_rightTopPossibleMove(0)
{
}

void DistantCombat::render() const
{
	switch (m_eOrientation)
	{
	case DistantCombat::EDistantUnitOrientaition::Top:
		m_pSprite_top->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_top.getCurrentFrame());
		break;
	case DistantCombat::EDistantUnitOrientaition::Bottom:
		m_pSprite_bottom->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_bottom.getCurrentFrame());
		break;
	case DistantCombat::EDistantUnitOrientaition::Left:
		m_pSprite_left->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_left.getCurrentFrame());
		break;
	case DistantCombat::EDistantUnitOrientaition::Right:
		m_pSprite_right->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_right.getCurrentFrame());
		break;
	}
}

void DistantCombat::SetOrientation(const EDistantUnitOrientaition eOrientation)
{
	if (m_eOrientation == eOrientation)
	{
		return;
	}

	m_eOrientation = eOrientation;
	switch (m_eOrientation)
	{
	case DistantCombat::EDistantUnitOrientaition::Top:
		m_direction.x = 0;
		m_direction.y = 1;
		break;
	case DistantCombat::EDistantUnitOrientaition::Bottom:
		m_direction.x = 0;
		m_direction.y = -1;
		break;
	case DistantCombat::EDistantUnitOrientaition::Left:
		m_direction.x = -1;
		m_direction.y = 0;
		break;
	case DistantCombat::EDistantUnitOrientaition::Right:
		m_direction.x = 1;
		m_direction.y = 0;
		break;
	}
}

void DistantCombat::update(const double delta)
{
	if (m_velocity > 0)
	{
		switch (m_eOrientation)
		{
		case DistantCombat::EDistantUnitOrientaition::Top:
			m_spriteAnimator_top.update(delta);
			break;
		case DistantCombat::EDistantUnitOrientaition::Bottom:
			m_spriteAnimator_bottom.update(delta);
			break;
		case DistantCombat::EDistantUnitOrientaition::Left:
			m_spriteAnimator_left.update(delta);
			break;
		case DistantCombat::EDistantUnitOrientaition::Right:
			m_spriteAnimator_right.update(delta);
			break;
		}
	}
}