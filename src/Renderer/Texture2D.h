#pragma once

#include <glad/glad.h>
#include <string>
#include <map>

#include <glm/vec2.hpp>

/**
* @file Texture2D.h
* File loads textures
*/

/**
* @namespace RenderEngine Covers all classes responsible for rendering resources
*/
namespace RenderEngine 
{
	/**
	* @class Texture2D
	* @brief load texture
	*/
	class Texture2D
	{
	public:

		/**
		* @struct SubTexture2D
		* @brief gets subtexture data
		*/
		struct SubTexture2D
		{
			glm::vec2 leftBottomUV; ///< lower left corner
			glm::vec2 rightTopUV; ///< top right corner

			/**
			* A constructor. First overload
			* @param[in] _leftBottomUV glm::vec2 lower left corner
			* @param[in] _rightTopUV glm::vec2 top right corner
			*/
			SubTexture2D(const glm::vec2 _leftBottomUV, const glm::vec2 _rightTopUV) : leftBottomUV(_leftBottomUV), rightTopUV(_rightTopUV) {}

			/**
			* A constructor.
			*/
			SubTexture2D() : leftBottomUV(0.f), rightTopUV(1.f) {}
		};

		/**
		* A constructor.
		* @param[in] width GLuint width
		* @param[in] height GLuint height
		* @param[in] data unsigned char*
		* @param[in] channels unsigned int color channel
		* @param[in] filter GLenum
		* @param[in] wrapMode GLenum
		*/
		Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels = 4, const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);

		/**
		* A distructor. Clear Texture
		*/
		~Texture2D();

		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		/**
		* move operator
		* @param texture2D Texture2D&&
		* @return Texture2D&
		*/
		Texture2D& operator=(Texture2D&& texture2D);

		/**
		* move initialization
		* @param texture2D Texture2D&&
		* @return Texture2D&
		*/
		Texture2D(Texture2D&& texture2D);

		/**
		* A const member
		*/
		void bind() const;

		/**
		* A const member
		* @return width
		*/
		unsigned int width() const { return m_width; }

		/**
		* A const member
		* @return height
		*/
		unsigned int height() const { return m_height; }

		// Функции для SubStruct

		/**
		* A member
		* @param name std::string
		* @param leftBottomUV glm::vec2& lower left corner
		* @param rightTopUV glm::vec2& top right corner
		*/
		void addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);

		/**
		* A const member
		* @param name std::string
		* @return SubTexture
		*/
		const SubTexture2D& getSubTexture(const std::string& name) const;

	private:
		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width;
		unsigned int m_height;

		std::map<std::string, SubTexture2D> m_subTextures;
	};
}