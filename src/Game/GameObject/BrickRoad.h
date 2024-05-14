#pragma once

#include "IGameObject.h"

#include <memory>

namespace Renderer
{
	class Sprite;
}

class BrickRoad : public IGameObject
{
public:
	BrickRoad(const std::shared_ptr<Renderer::Sprite> pSprite, const glm::vec2& position, const glm::vec2& size, const float rotation);

	virtual void render() const override;
	virtual void update(const uint64_t delta) override;

private:
	std::shared_ptr<Renderer::Sprite> m_pCurrentSprite;
};