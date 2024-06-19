#pragma once

#include <memory>

/**
* @file SpriteAnimator.h
* The file is responsible for sprite animation
*/

/**
* @namespace RenderEngine Covers all classes responsible for rendering resources
*/
namespace RenderEngine
{
	class Sprite;

	/**
	* @class SpriteAnimator
	* @brief sprite animation loading class
	*/
	class SpriteAnimator
	{
	public:

		/**
		* A constructor
		* @param[in] pSprite std::shared_ptr<Sprite>, link to the sprite that needs to be animated\
		*/
		SpriteAnimator(std::shared_ptr<Sprite> pSprite);

		/**
		* A member.
		* @return Current Frame
		*/
		size_t getCurrentFrame() const { return m_currentFrame; }

		/**
		* A member. Responsible for updating in the current frame
		* @param[in] delta Double update time.
		*/
		void update(const double delta);

		/**
		* A member. Responsible for updating in the current frame
		* @return total Durations.
		*/
		double getTotalDuration() const { return m_totalDuration; }

		/**
		* A member. Resets animation
		*/
		void reset();
		
	private:
		std::shared_ptr<Sprite> m_pSprite;
		size_t m_currentFrame;
		double m_currentFrameDurration;
		double m_currentAnimationTime;
		double m_totalDuration;
	};
}