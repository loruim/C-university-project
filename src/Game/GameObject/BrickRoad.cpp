#include "BrickRoad.h"

#include "../../Renderer/Sprite.h"

BrickRoad::BrickRoad(const std::shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2& position, const glm::vec2& size, const float rotation) : IGameObject(position, size, rotation), m_pCurrentSprite(std::move(pSprite))
{

}

void BrickRoad::render() const
{
	m_pCurrentSprite->render(m_position, m_size, m_rotation);
}

void BrickRoad::update(const uint64_t delta)
{
}