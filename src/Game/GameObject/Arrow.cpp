#include "Arrow.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Arrow::Arrow(const double velocity, const glm::vec2& position, const glm::vec2& size, const float layer)
	: IGameObject(IGameObject::EObjectType::Arrow, position, size, 0.f, layer)
	, m_pSprite_top(ResourceManager::getSprite("ArrowTop"))
	, m_pSprite_bottom(ResourceManager::getSprite("ArrowBottom"))
	, m_pSprite_left(ResourceManager::getSprite("ArrowLeft"))
	, m_pSprite_right(ResourceManager::getSprite("ArrowRight"))
	, m_eOrientation(EOrientation::Top)
	, m_maxVelocity(velocity)
	, m_isActive(false)
{
	setVelocity(velocity);
	m_colliders.emplace_back(glm::vec2(0), m_size);
}

void Arrow::render() const
{
	if (m_isActive)
	{
		switch (m_eOrientation)
		{
		case EOrientation::Top:
			m_pSprite_top->render(m_position, m_size, m_rotation, m_layer);
			break;
		case EOrientation::Bottom:
			m_pSprite_bottom->render(m_position, m_size, m_rotation, m_layer);
			break;
		case EOrientation::Left:
			m_pSprite_left->render(m_position, m_size, m_rotation, m_layer);
			break;
		case EOrientation::Right:
			m_pSprite_right->render(m_position, m_size, m_rotation, m_layer);
			break;
		}
	}
}

void Arrow::fire(const glm::vec2& position, const glm::vec2& direction)
{
	m_position = position;
	m_direction = direction;
	if (m_direction.x == 0.f)
	{
		m_eOrientation = (m_direction.y < 0) ? EOrientation::Bottom : EOrientation::Top;
	}
	else
	{
		m_eOrientation = (m_direction.x < 0) ? EOrientation::Left : EOrientation::Right;
	}
	m_isActive = true;
	setVelocity(m_maxVelocity);
}

void Arrow::onCollision()
{
	setVelocity(0);
	m_isActive = false;
}