#include "House.h"

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"

House::House(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) : IGameObject(position, size, rotation, layer), m_pCurrentSprite(ResourceManager::getSprite("House"))
{

}

void House::render() const
{
	m_pCurrentSprite->render(m_position, m_size, m_rotation, m_layer);
}

void House::update(const uint64_t delta)
{
}