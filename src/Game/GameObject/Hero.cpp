#include "Hero.h"

#include "IGameObject.h"
#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"
#include "Arrow.h"
#include "../../Physics/PhysicsEngine.h"	

const std::string& Hero::getHeroSpriteFromType(const EHeroType eType)
{
	return HeroTypeString[static_cast<size_t>(eType)];
}

Hero::Hero(const Hero::EHeroType eType,
	       const double maxVelocity, 
		   const glm::vec2& position, 
		   const glm::vec2& size, 
		   const float layer)
	: IGameObject(IGameObject::EObjectType::Hero, position, size, 0.f, layer)
	, m_eOrientation(EOrientaition::Left)
	, m_pCurrentArrow(std::make_shared<Arrow>(0.1, m_position + m_size / 4.f, m_size / 2.f, layer))
	, m_pSprite_top(ResourceManager::getSprite(getHeroSpriteFromType(eType) + "_Left"))
	, m_pSprite_bottom(ResourceManager::getSprite(getHeroSpriteFromType(eType) + "_Right"))
	, m_pSprite_left(ResourceManager::getSprite(getHeroSpriteFromType(eType) + "_Left"))
	, m_pSprite_right(ResourceManager::getSprite(getHeroSpriteFromType(eType) + "_Right"))
	, m_spriteAnimator_top(m_pSprite_top)
	, m_spriteAnimator_bottom(m_pSprite_bottom)
	, m_spriteAnimator_left(m_pSprite_left)
	, m_spriteAnimator_right(m_pSprite_right)
	, m_maxVelocity(maxVelocity)
	, m_isHaveAngel(true)
	, m_isHaveArcher(false)
	, m_isHaveBarbarian(false)
	, m_isHaveKnight(false)
	, m_isHaveMagican(true)
	, m_isHaveTitan(false)

{
	m_colliders.emplace_back(glm::vec2(0), m_size);
	Physics::PhysicsEngine::addDynamicGameObject(m_pCurrentArrow);
}

void Hero::render() const
{
	switch (m_eOrientation)
	{
	case Hero::EOrientaition::Top:
		m_pSprite_top->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_top.getCurrentFrame());
		break;
	case Hero::EOrientaition::Bottom:
		m_pSprite_bottom->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_bottom.getCurrentFrame());
		break;
	case Hero::EOrientaition::Left:
		m_pSprite_left->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_left.getCurrentFrame());
		break;
	case Hero::EOrientaition::Right:
		m_pSprite_right->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_right.getCurrentFrame());
		break;
	}

	if (m_pCurrentArrow->isActive())
	{
		m_pCurrentArrow->render();
	}
}

void Hero::SetOrientation(const EOrientaition eOrientation)
{
	if (m_eOrientation == eOrientation)
	{
		return;
	}

	m_eOrientation = eOrientation;
	switch (m_eOrientation)
	{
	case Hero::EOrientaition::Top:
		m_direction.x = 0;
		m_direction.y = 1;
		break;
	case Hero::EOrientaition::Bottom:
		m_direction.x = 0;
		m_direction.y = -1;
		break;
	case Hero::EOrientaition::Left:
		m_direction.x = -1;
		m_direction.y = 0;
		break;
	case Hero::EOrientaition::Right:
		m_direction.x = 1;
		m_direction.y = 0;
		break;
	}
}

void Hero::update(const double delta)
{
	if (m_velocity > 0)
	{
		switch (m_eOrientation)
		{
		case Hero::EOrientaition::Top:
			m_spriteAnimator_top.update(delta);
			break;
		case Hero::EOrientaition::Bottom:
			m_spriteAnimator_bottom.update(delta);
			break;
		case Hero::EOrientaition::Left:
			m_spriteAnimator_left.update(delta);
			break;
		case Hero::EOrientaition::Right:
			m_spriteAnimator_right.update(delta);
			break;
		}
	}
}

void Hero::fire()
{
	if (!m_pCurrentArrow->isActive())
	{
		m_pCurrentArrow->fire(m_position + m_size / 4.f, m_direction);
	}
}