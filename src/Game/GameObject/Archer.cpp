#include "Archer.h"

#include "../../Renderer/AnimatedSprite.h"
#include "IGameObject.h"

Archer::Archer(std::shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position, const glm::vec2& size) : IGameObject(position, size, 0.f), m_eOrientation(EOrientaition::Left), m_pSprite(std::move(pSprite)), m_move(false), m_velocity(velocity), m_moveOffSet(glm::vec2(1.f, 0.f))
{
}

void Archer::render() const
{
	m_pSprite->render(m_position, m_size, m_rotation);
}

void Archer::SetOrientation(const EOrientaition eOrientation)
{
	if (m_eOrientation == eOrientation)
	{
		return;
	}

	m_eOrientation = eOrientation;
	switch (m_eOrientation)
	{
	case Archer::EOrientaition::Left:
		m_pSprite->setState("ArcherLeftState");
		m_moveOffSet.x = -1;
		m_moveOffSet.y = 0;
		break;
	case Archer::EOrientaition::Right:
		m_pSprite->setState("ArcherRightState");
		m_moveOffSet.x = 1;
		m_moveOffSet.y = 0;
		break;
	case Archer::EOrientaition::Top:
		m_pSprite->setState("ArcherLeftState");
		m_moveOffSet.x = 0;
		m_moveOffSet.y = 1;
		break;
	case Archer::EOrientaition::Bottom:
		m_pSprite->setState("ArcherLeftState");
		m_moveOffSet.x = 0;
		m_moveOffSet.y = -1;
		break;
	default:
		break;
	}
}

void Archer::move(const bool move)
{
	m_move = move;
}

void Archer::update(const uint64_t delta)
{
	if (m_move)
	{
		m_position += delta * m_velocity * m_moveOffSet;
		//m_pSprite->setPosition(m_position);
		m_pSprite->update(delta);
	}
}