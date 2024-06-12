#include "SpriteAnimator.h"

#include "Sprite.h"

namespace RenderEngine
{
	SpriteAnimator::SpriteAnimator(std::shared_ptr<Sprite> pSprite) 
        : m_pSprite(std::move(pSprite))
        , m_currentFrame(0)
        , m_currentFrameDurration(m_pSprite->getFrameDuration(0))
        , m_currentAnimationTime(0)
        , m_totalDuration(0)
	{
        for (size_t currentFrameID = 0; currentFrameID < m_pSprite->getFramesCount(); ++currentFrameID)
        {
            m_totalDuration += m_pSprite->getFrameDuration(currentFrameID);
        }
	}

	void SpriteAnimator::update(const double delta)
	{
        m_currentAnimationTime += delta;
        while (m_currentAnimationTime >= m_currentFrameDurration)
        {
            m_currentAnimationTime -= m_currentFrameDurration;
            ++m_currentFrame;
            if (m_currentFrame == m_pSprite->getFramesCount())
            {
                m_currentFrame = 0;
            }
            m_currentFrameDurration = m_pSprite->getFrameDuration(m_currentFrame);
        }
	}

    void SpriteAnimator::reset()
    {
        m_currentFrame = 0;
        m_currentFrameDurration = m_pSprite->getFrameDuration(0);
        m_currentAnimationTime = 0;
    }
}