#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"

namespace RenderEngine
{
	class Sprite;
}

class Arrow;

class Hero : public IGameObject
{
public:
	enum class EOrientaition {
		Top,
		Bottom,
		Left,
		Right
	};

	Hero(const double maxVelocity, 
		 const glm::vec2& position, 
		 const glm::vec2& size, 
		 const float layer);

	void render() const override;
	void SetOrientation(const EOrientaition eOrientation);
	void update(const double delta) override;
	double getMaxVelocity() const { return m_maxVelocity; }
	void fire();

private:
	EOrientaition m_eOrientation;
	std::shared_ptr<Arrow> m_pCurrentArrow;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_top;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_bottom;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_left;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_right;
	RenderEngine::SpriteAnimator m_spriteAnimator_top;
	RenderEngine::SpriteAnimator m_spriteAnimator_bottom;
	RenderEngine::SpriteAnimator m_spriteAnimator_left;
	RenderEngine::SpriteAnimator m_spriteAnimator_right;
	double m_maxVelocity;
};