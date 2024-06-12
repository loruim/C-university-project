#include "Border.h"

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"

Border::Border(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) : IGameObject(position, size, rotation, layer), m_pCurrentSprite(ResourceManager::getSprite("Border"))
{
	m_colliders.emplace_back(glm::vec2(0), m_size);
}

void Border::render() const
{
	m_pCurrentSprite->render(m_position, m_size, m_rotation, m_layer);
}