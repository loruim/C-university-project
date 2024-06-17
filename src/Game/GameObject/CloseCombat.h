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

class CloseCombat : public IGameObject
{
public:

    enum class ECloseCombatUnitType : uint8_t
    {
        knight,
        barbarian,
        angel
    };

    inline static std::string CloseCombatTypeString[] = {
        "knightState",
        "barbarianState",
        "angelState"
    };

    enum class ECloseUnitOrientaition {
        Top,
        Bottom,
        Left,
        Right
    };

    CloseCombat(const CloseCombat::ECloseCombatUnitType eType,
                const double maxVelocity,
                const glm::vec2& position,
                const glm::vec2& size,
                const float layer);

    virtual void render() const override;
    virtual void update(const double delta) override;
    double getMaxVelocity() const { return m_maxVelocity; }
    void SetOrientation(const ECloseUnitOrientaition eUnitOrientation);
    glm::vec2& SetPosition(glm::vec2 position) { return m_position = position; }

private:
    ECloseUnitOrientaition m_eOrientation;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_top;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_bottom;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_left;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_right;
    RenderEngine::SpriteAnimator m_spriteAnimator_top;
    RenderEngine::SpriteAnimator m_spriteAnimator_bottom;
    RenderEngine::SpriteAnimator m_spriteAnimator_left;
    RenderEngine::SpriteAnimator m_spriteAnimator_right;
    double m_maxVelocity;
    static const std::string& getCloseCombatSpriteFromType(const ECloseCombatUnitType eType);
};