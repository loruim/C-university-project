#include "Water.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Water::Water(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) 
	: IGameObject(IGameObject::EObjectType::Water, position, size, rotation, layer)
	, m_pCurrentSprite(ResourceManager::getSprite("waterSprite"))
	, m_spriteAnimator(m_pCurrentSprite)
{
	m_colliders.emplace_back(glm::vec2(0), m_size);
}

void Water::render() const
{
	m_pCurrentSprite->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator.getCurrentFrame());
}

void Water::update(const double delta)
{
	m_spriteAnimator.update(delta);
}