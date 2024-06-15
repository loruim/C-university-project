#include "HouseRightOne.h"

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"

HouseRightOne::HouseRightOne(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(IGameObject::EObjectType::RightHouseOne, position, size, rotation, layer)
	, m_pCurrentSprite(ResourceManager::getSprite("House"))
{
	m_colliders.emplace_back(glm::vec2(0), m_size);
}

void HouseRightOne::render() const
{
	m_pCurrentSprite->render(m_position, m_size, m_rotation, m_layer);
}

bool HouseRightOne::collides(const EObjectType objectType)
{
	if (!(objectType != IGameObject::EObjectType::Hero))
	{
		m_isActive = true;
	}
	return true;
}