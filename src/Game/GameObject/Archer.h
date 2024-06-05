#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include "IGameObject.h"

namespace RenderEngine
{
	class AnimatedSprite;
}

class Archer : public IGameObject
{
public:
	enum class EOrientaition {
		Top,
		Bottom,
		Left,
		Right
	};

	Archer(std::shared_ptr<RenderEngine::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position, const glm::vec2& size);

	void render() const override;
	void SetOrientation(const EOrientaition eOrientation);
	void move(const bool move);
	void update(const uint64_t delta) override;

private:
	EOrientaition m_eOrientation;
	std::shared_ptr<RenderEngine::AnimatedSprite> m_pSprite;
	bool m_move;
	float m_velocity;
	glm::vec2 m_moveOffSet;
};