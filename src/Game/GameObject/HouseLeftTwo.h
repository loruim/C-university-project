#pragma once

#include "IGameObject.h"

#include <memory>

namespace RenderEngine
{
	class Sprite;
}

class HouseLeftTwo : public IGameObject
{
public:
	HouseLeftTwo(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

	virtual void render() const override;
	virtual bool collides(const EObjectType objectType) override;

private:
	std::shared_ptr<RenderEngine::Sprite> m_pCurrentSprite;
};