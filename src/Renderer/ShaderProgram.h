#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/mat4x4.hpp>

/**
* @file ShaderProgram.h
* The file contains game shaders
*/

/**
* @namespace RenderEngine Covers all classes responsible for rendering resources
*/
namespace RenderEngine
{
	/**
	* @class ShaderProgram
	* @brief shader loading class
	*
	* Ñlass that loads vertex and fragment shaders
	*/
	class ShaderProgram {
	public:

		/**
		* A constructor.
		* @param[in] vertexShader std::string
		* @param[in] fragmentShader std::string
		*/
		ShaderProgram(const std::string vertexShader, const std::string fragmentShader);

		/**
		* A destructor. Clears shader slots to free up memory
		*/
		~ShaderProgram();

		/**
		* A const member.
		* @return Compilation status
		*/
		bool isCompiled() const { return m_isCompiled; }

		/**
		* A const member. Uses current shaders
		*/
		void use() const;

		/**
		* A member.
		* @param[in] name std::string&
		* @param[in] value GLint
		*/
		void setInt(const std::string& name, const GLint value);

		/**
		* A member.
		* @param[in] name std::string&
		* @param[in] value GLfloat
		*/
		void setFloat(const std::string& name, const GLfloat value);

		/**
		* A member.
		* @param[in] name std::string&
		* @param[in] matrix glm::mat4&
		*/
		void setMatrix4(const std::string& name, const glm::mat4& matrix);

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		/**
		* Move assignment
		* @param[in] shaderProgram ShaderProgram&&
		* @return ShaderProgram& 
		*/
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;

		/**
		* Move add
		* @param[in] shaderProgram ShaderProgram&&
		* @return ShaderProgram&
		*/
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

	private:

		/**
		* A member.
		* @param[in] source std::string& contents of the shader file
		* @param[in] shaderType GLenum
		* @param[in] shaderID GLuint&
		*/
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}