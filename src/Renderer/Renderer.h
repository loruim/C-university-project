#pragma once

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "IndexBuffer.h"

#include <string>

/**
* @file Renderer.h
* File responsible for rendering functions
*/

/**
* @namespace RenderEngine Covers all classes responsible for rendering resources
*/
namespace RenderEngine
{
	/**
	* @class Renderer
	* @brief render loading class
	*
	* The class loads all screen functions such as drawing, color filling, etc.
	*/
	class Renderer
	{
	public:

		/**
		* A static member. Draws a pair of triangles so that the result is a square
		* @param[in] vertexArray VertexArray&, reference to points in space whose connection forms a figure
		* @param[in] indexBuffer IndexBuffer&, link to the order of connecting the dots
		* @param[in] shader ShaderProgram&, link to shaders
		*/
		static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader);

		/**
		* A static member. Fills the screen with the selected color
		* @param[in] r float, red color
		* @param[in] g float, green color
		* @param[in] b float, blue color
		* @param[in] alpha float, transparency
		*/
		static void setClearColor(float r, float g, float b, float alpha);

		/**
		* A static member. Responsible for activating color depth
		* @param[in] enable bool
		*/
		static void setDepthTest(const bool enable);

		/**
		* A static member. Responsible for cleaning the screen
		*/
		static void clear();

		/**
		* A static member. Responsible for displaying the game depending on the screen size
		* @param[in] width unsigned int
		* @param[in] height unsigned int
		* @param[in] leftOffset unsigned int
		* @param[in] bottomOffset unsigned int
		*/
		static void setViewport(unsigned int width, unsigned int height, unsigned int leftOffset = 0, unsigned int bottomOffset = 0);
	};
}