#include "BrickRoad.h"

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"

BrickRoad::BrickRoad(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) : IGameObject(position, size, rotation, layer), m_pCurrentSprite(ResourceManager::getSprite("FullRoad"))
{

}

void BrickRoad::render() const
{
	m_pCurrentSprite->render(m_position, m_size, m_rotation, m_layer);
}