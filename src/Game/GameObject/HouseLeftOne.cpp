#include "HouseLeftOne.h"

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"

HouseLeftOne::HouseLeftOne(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(IGameObject::EObjectType::LeftHouseOne, position, size, rotation, layer)
	, m_pCurrentSprite(ResourceManager::getSprite("House"))
{
	m_colliders.emplace_back(glm::vec2(0), m_size);
}

void HouseLeftOne::render() const
{
	m_pCurrentSprite->render(m_position, m_size, m_rotation, m_layer);
}

bool HouseLeftOne::collides(const EObjectType objectType)
{
	if (!(objectType != IGameObject::EObjectType::Hero))
	{
		m_isActive = true;
	}
	return true;
}