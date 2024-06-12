#include "Hero.h"

#include "IGameObject.h"
#include "../../Renderer/Sprite.h"

Hero::Hero(std::shared_ptr<RenderEngine::Sprite> pSprite_top, 
		   std::shared_ptr<RenderEngine::Sprite> pSprite_bottom, 
		   std::shared_ptr<RenderEngine::Sprite> pSprite_left, 
		   std::shared_ptr<RenderEngine::Sprite> pSprite_right, 
		   const double maxVelocity, 
		   const glm::vec2& position, 
		   const glm::vec2& size, 
		   const float layer)
	: IGameObject(IGameObject::EObjectType::Hero, position, size, 0.f, layer)
	, m_eOrientation(EOrientaition::Left)
	, m_pSprite_top(std::move(pSprite_top))
	, m_pSprite_bottom(std::move(pSprite_bottom))
	, m_pSprite_left(std::move(pSprite_left))
	, m_pSprite_right(std::move(pSprite_right))
	, m_spriteAnimator_top(m_pSprite_top)
	, m_spriteAnimator_bottom(m_pSprite_bottom)
	, m_spriteAnimator_left(m_pSprite_left)
	, m_spriteAnimator_right(m_pSprite_right)
	, m_maxVelocity(maxVelocity)
{
	m_colliders.emplace_back(glm::vec2(0), m_size);
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