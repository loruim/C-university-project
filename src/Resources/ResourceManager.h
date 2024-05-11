#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Renderer
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}

class ResourceManager {
public:
	static void setExecutablePath(const std::string& executablePath);
	static void unloadAllResources();
	
	ResourceManager() = delete;
	~ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	// Shaders
	static std::shared_ptr <Renderer::ShaderProgram> loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr <Renderer::ShaderProgram> getShader(const std::string& shaderName);

	// Texture
	static std::shared_ptr <Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr <Renderer::Texture2D> getTexture(const std::string& textureName);

	// Sprite
	static std::shared_ptr <Renderer::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "default");
	static std::shared_ptr <Renderer::Sprite> getSprite(const std::string& spriteName);

	// SubTexture :(
	static std::shared_ptr <Renderer::Texture2D> loadTextureAtlas(std::string texturename, std::string texturePath, std::vector<std::string> subTextures, const unsigned int SubTextureWidth, const unsigned int SubTextureHeight);

	// AnimatedSprite
	static std::shared_ptr <Renderer::AnimatedSprite> loadAnimatedSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "default");
	static std::shared_ptr <Renderer::AnimatedSprite> getAnimatedSprite(const std::string& spriteName);

private:
	static std::string getFileString(const std::string& relativeFilePath);
	static std::string m_path;

	// Shaders
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderProgram;

	// Texture
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	static TexturesMap m_textures;

	// Sprite
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	static SpritesMap m_sprites;

	// AnimatedSprite
	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimatedSprite>> AnimatedSpritesMap;
	static AnimatedSpritesMap m_animatedSprites;
};