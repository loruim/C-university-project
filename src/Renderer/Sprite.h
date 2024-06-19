#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>
#include <string>

/**
* @file Sprite.h
* The file is responsible for sprites
*/

/**
* @namespace RenderEngine Covers all classes responsible for rendering resources
*/
namespace RenderEngine
{
	class Texture2D;
	class ShaderProgram;

	/**
	* @class Sprite
	* @brief sprite loading class
	*/
	class Sprite
	{
	public:

		/**
		* @struct FrameDescription
		* @brief Frame Description
		*
		* describes the position and duration
		*/
		struct FrameDescription
		{

			/**
			* A constructor
			* @param[in] leftBottomUV glm::vec2&, coordinates of the lower left point
			* @param[in] rightTopUV glm::vec2&, coordinates of the upper right point
			* @param[in] duration double
			*/
			FrameDescription(const glm::vec2 leftBottomUV, const glm::vec2 rightTopUV, const double duration) : s_leftBottomUV(leftBottomUV), s_rightTopUV(rightTopUV), s_duration(duration)
			{
			}
				
			glm::vec2 s_leftBottomUV; ///< coordinates of the lower left point
			glm::vec2 s_rightTopUV; ///<  coordinates of the upper right point
			double s_duration;
		};

		/**
		* A constructor
		* @param[in] pTexture std::shared_ptr<Texture2D>, texture pointer
		* @param[in] initialSubTexture std::string, name subTexture
		* @param[in] pShaderProgram std::shared_ptr<ShaderProgram>, pointer on shader
		*/
		Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram);

		/**
		* A disconstructor
		*/
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		/**
		* A member.
		* @param[in] position glm::vec2&
		* @param[in] size glm::vec2&
		* @param[in] rotation float
		* @param[in] layer float
		* @param[in] frameID size_t, the last frame
		*/
		void render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer = 0.f, const size_t frameID = 0) const;

		/**
		* A const member.
		* @param[in] frameID size_t
		* @return frame Duration
		*/
		double getFrameDuration(const size_t frameID) const;

		/**
		* A const member.
		* @return frame count
		*/
		size_t getFramesCount() const;

		/**
		* A const member.
		* @param[in] framesDescriptions std::vector<FrameDescription>
		*/
		void insertFrames(std::vector<FrameDescription> framesDescriptions);

	protected:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordBuffer;
		VertexBuffer m_textureCoordBuffer;
		IndexBuffer m_IndexBuffer;

		std::vector<FrameDescription> m_framesDescriptions; ///< vector structure elements
		mutable size_t m_lastFrameID;
	};
}