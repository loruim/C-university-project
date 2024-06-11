#pragma once

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "IndexBuffer.h"

#include <string>

namespace RenderEngine
{
	class Renderer
	{
	public:
		static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader);
		static void setClearColor(float r, float g, float b, float alpha);
		static void setDepthTest(const bool enable);
		static void clear();
		static void setViewport(unsigned int width, unsigned int height, unsigned int leftOffset = 0, unsigned int bottomOffset = 0);
	};
}