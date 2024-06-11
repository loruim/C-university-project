#pragma once

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"

#include <memory>

namespace RenderEngine
{
	class Sprite;
}

class Water : public IGameObject
{
public:
	Water(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

	virtual void render() const override;
	virtual void update(const double delta) override;

private:
	std::shared_ptr<RenderEngine::Sprite> m_pCurrentSprite;
	RenderEngine::SpriteAnimator m_spriteAnimator;
};