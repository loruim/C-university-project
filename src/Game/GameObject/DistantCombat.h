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

class DistantCombat : public IGameObject
{
public:

    enum class EDistantCombatUnitType : uint8_t
    {
        archer,
        magican,
        titan
    };

    inline static std::string DistantCombatTypeString[] = {
        "archerState",
        "magicanState",
        "titanState"
    };

    enum class EDistantUnitOrientaition {
        Top,
        Bottom,
        Left,
        Right
    };

    DistantCombat(const DistantCombat::EDistantCombatUnitType eType,
                  const double maxVelocity,
                  const glm::vec2& position,
                  const glm::vec2& size,
                  const float layer);

    virtual void render() const override;
    virtual void update(const double delta) override;
    double getMaxVelocity() const { return m_maxVelocity; }
    void SetOrientation(const EDistantUnitOrientaition eUnitOrientation);
    glm::vec2& SetPosition(glm::vec2 position) { return m_position = position; }
    uint8_t getDistantSpeed() { return m_distantSpeed; }
    glm::vec2& getLeftBottomPossibleMove() { return m_leftBottomPossibleMove; }
    glm::vec2& getRightTopPossibleMove() { return m_rightTopPossibleMove; }
    /*glm::vec2& setPossibleMove(glm::vec2 PossibleMove) { return m_possibleMove = PossibleMove; }
    glm::vec2& getPossibleMove() { return m_possibleMove; }*/

private:
    EDistantUnitOrientaition m_eOrientation;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_top;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_bottom;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_left;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_right;
    RenderEngine::SpriteAnimator m_spriteAnimator_top;
    RenderEngine::SpriteAnimator m_spriteAnimator_bottom;
    RenderEngine::SpriteAnimator m_spriteAnimator_left;
    RenderEngine::SpriteAnimator m_spriteAnimator_right;
    double m_maxVelocity;

    glm::vec2 m_leftBottomPossibleMove;
    glm::vec2 m_rightTopPossibleMove;
    /*glm::vec2 m_possibleMove;*/

    static uint8_t m_distantSpeed;
    static std::string& getDistantCombatSpriteFromType(const EDistantCombatUnitType eType);
};