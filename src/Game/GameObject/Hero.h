#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"

namespace RenderEngine
{
	class Sprite;
}

class Hero : public IGameObject
{
public:
	enum class EOrientaition {
		Top,
		Bottom,
		Left,
		Right
	};

	Hero(std::shared_ptr<RenderEngine::Sprite> pSprite_top, 
		 std::shared_ptr<RenderEngine::Sprite> pSprite_bottom, 
		 std::shared_ptr<RenderEngine::Sprite> pSprite_left, 
		 std::shared_ptr<RenderEngine::Sprite> pSprite_right, 
		 const double velocity, 
		 const glm::vec2& position, 
		 const glm::vec2& size, 
		 const float layer);

	void render() const override;
	void SetOrientation(const EOrientaition eOrientation);
	void move(const bool move);
	void update(const double delta) override;

private:
	EOrientaition m_eOrientation;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_top;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_bottom;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_left;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_right;
	RenderEngine::SpriteAnimator m_spriteAnimator_top;
	RenderEngine::SpriteAnimator m_spriteAnimator_bottom;
	RenderEngine::SpriteAnimator m_spriteAnimator_left;
	RenderEngine::SpriteAnimator m_spriteAnimator_right;
	bool m_move;
	double m_velocity;
	glm::vec2 m_moveOffSet;
};