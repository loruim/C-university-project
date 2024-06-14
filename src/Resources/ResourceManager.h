#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace RenderEngine
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
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
	static std::shared_ptr <RenderEngine::ShaderProgram> loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr <RenderEngine::ShaderProgram> getShader(const std::string& shaderName);

	// Texture
	static std::shared_ptr <RenderEngine::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr <RenderEngine::Texture2D> getTexture(const std::string& textureName);

	// Sprite
	static std::shared_ptr <RenderEngine::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName = "default");
	static std::shared_ptr <RenderEngine::Sprite> getSprite(const std::string& spriteName);

	// TextureAtlas
	static std::shared_ptr <RenderEngine::Texture2D> loadTextureAtlas(std::string texturename, std::string texturePath, std::vector<std::string> subTextures, const unsigned int SubTextureWidth, const unsigned int SubTextureHeight);

	// Include JSON
	static bool loadJSONResources(const std::string& JSONPath);

	// Levels
	static const std::vector<std::vector<std::string>>& getLevels() { return m_levels; }

	// Shops
	static const  std::vector<std::vector<std::string>>& getShopsScreen() { return m_shopsScreen; }

private:
	static std::string getFileString(const std::string& relativeFilePath);
	static std::string m_path;

	// Shaders
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderProgram;

	// Texture
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture2D>> TexturesMap;
	static TexturesMap m_textures;

	// Sprite
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Sprite>> SpritesMap;
	static SpritesMap m_sprites;

	// Levels
	static std::vector<std::vector<std::string>> m_levels;

	// Shops
	static std::vector<std::vector<std::string>> m_shopsScreen;
};