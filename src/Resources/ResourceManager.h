#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
}

class ResourceManager {
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	// Shaders
	std::shared_ptr <Renderer::ShaderProgram> loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr <Renderer::ShaderProgram> getShader(const std::string& shaderName);

	// Texture
	std::shared_ptr <Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	std::shared_ptr <Renderer::Texture2D> getTexture(const std::string& textureName);

	// Sprite
	std::shared_ptr <Renderer::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight);
	std::shared_ptr <Renderer::Sprite> getSprite(const std::string& spriteName);

private:
	std::string getFileString(const std::string& relativeFilePath) const;

	// Shaders
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderProgram;

	// Texture
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	TexturesMap m_textures;

	// Sprite
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	SpritesMap m_sprites;

	std::string m_path;
};