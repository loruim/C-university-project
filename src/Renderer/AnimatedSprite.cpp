#include "AnimatedSprite.h"
#include "Texture2D.h"

#include <iostream>

namespace RenderEngine
{
    AnimatedSprite::AnimatedSprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram) : Sprite(std::move(pTexture), std::move(initialSubTexture), std::move(pShaderProgram))
    {
        m_pCurrentAnimationDurations = m_stateMap.end();
    }

    void AnimatedSprite::insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTextureDuration)
    {
        m_stateMap.emplace(std::move(state), std::move(subTextureDuration));
    }

    void AnimatedSprite::update(const uint64_t delta)
    {
        if (m_pCurrentAnimationDurations != m_stateMap.end())
        {
            m_currentAnimationTime += delta;
            while (m_currentAnimationTime >= m_pCurrentAnimationDurations->second[m_currentFrame].second)
            {
                m_currentAnimationTime -= m_pCurrentAnimationDurations->second[m_currentFrame].second;
                ++m_currentFrame;
                m_dirty = true;

                if (m_currentFrame == m_pCurrentAnimationDurations->second.size())
                {
                    m_currentFrame = 0;
                }
            }
        }
    }

    void AnimatedSprite::setState(const std::string& newState)
    {
        auto it = m_stateMap.find(newState);
        if (it == m_stateMap.end())
        {
            std::cout << "Can't found state: " << newState << std::endl;
            return;
        }

        if (it != m_pCurrentAnimationDurations)
        {
            m_currentAnimationTime = 0;
            m_currentFrame = 0;
            m_pCurrentAnimationDurations = it;
            m_dirty = true;
        }
    }

    void AnimatedSprite::render(const glm::vec2& position, const glm::vec2& size, const float rotation) const
    {
        if (m_dirty)
        {
            auto subTexture = m_pTexture->getSubTexture(m_pCurrentAnimationDurations->second[m_currentFrame].first);

            const GLfloat textureCoords[] = {
                // U V
                subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
                subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
                subTexture.rightTopUV.x, subTexture.rightTopUV.y,
                subTexture.rightTopUV.x, subTexture.leftBottomUV.y
            };

            m_textureCoordBuffer.update(textureCoords, 2 * 4 * sizeof(GLfloat));
            m_dirty = false;
        }
        Sprite::render(position, size, rotation);
    }
}