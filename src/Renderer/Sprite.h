#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>
#include <string>

namespace RenderEngine
{
	class Texture2D;
	class ShaderProgram;

	class Sprite
	{
	public:

		struct FrameDescription
		{
			FrameDescription(const glm::vec2 leftBottomUV, const glm::vec2 rightTopUV, const uint64_t duration) : s_leftBottomUV(leftBottomUV), s_rightTopUV(rightTopUV), s_duration(duration)
			{
			}
				
			glm::vec2 s_leftBottomUV;
			glm::vec2 s_rightTopUV;
			uint64_t s_duration;
		};

		Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void render(const glm::vec2& position, const glm::vec2& size, const float rotation, const size_t frameID = 0) const;
		uint64_t getFrameDuration(const size_t frameID) const;
		size_t getFramesCount() const;	
		void insertFrames(std::vector<FrameDescription> framesDescriptions);

	protected:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordBuffer;
		VertexBuffer m_textureCoordBuffer;
		IndexBuffer m_IndexBuffer;

		std::vector<FrameDescription> m_framesDescriptions;
		mutable size_t m_lastFrameID;
	};
}