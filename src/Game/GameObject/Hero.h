#pragma once

#include <memory>
#include <string>
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

	enum class EHeroType : uint8_t
	{
		Archer,
		Titan,
		Barbarian,
		Angel,
		Knight,
		Magican
	};

	inline static std::string HeroTypeString[] = {
		"archerState",
		"titanState",
		"barbarianState",
		"angelState",
		"knightState",
		"magicanState"
	};

	enum class EOrientaition {
		Top,
		Bottom,
		Left,
		Right
	};

	Hero(const Hero::EHeroType eType,
		 const double maxVelocity, 
		 const glm::vec2& position, 
		 const glm::vec2& size, 
		 const float layer);

	virtual void render() const override;
	virtual void update(const double delta) override;
	void SetOrientation(const EOrientaition eOrientation);
	double getMaxVelocity() const { return m_maxVelocity; }
	void fire();

	bool isHaveAngel() { return m_isHaveAngel; }
	bool isHaveArcher() { return m_isHaveArcher; }
	bool isHaveBarbarian() { return m_isHaveBarbarian; }
	bool isHaveKnight() { return m_isHaveKnight; }
	bool isHaveMagican() { return m_isHaveMagican; }
	bool isHaveTitan() { return m_isHaveTitan; }

	void SetIsHaveAngel(bool isActive) { m_isHaveAngel = isActive; }
	void SetIsHaveArcher(bool isActive) { m_isHaveArcher = isActive; }
	void SetIsHaveBarbarian(bool isActive) { m_isHaveBarbarian = isActive;; }
	void SetIsHaveKnight(bool isActive) { m_isHaveKnight = isActive;; }
	void SetIsHaveMagican(bool isActive) { m_isHaveMagican = isActive;; }
	void SetIsHaveTitan(bool isActive) { m_isHaveTitan = isActive;; }

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
	static const std::string& getHeroSpriteFromType(const EHeroType eType);

	bool m_isHaveArcher;
	bool m_isHaveMagican;
	bool m_isHaveTitan;
	bool m_isHaveBarbarian;
	bool m_isHaveKnight;
	bool m_isHaveAngel;
};