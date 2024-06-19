#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

/**
* @file ResourceManager.h
* One of the main files downloaded by all game resources
*/

namespace RenderEngine
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
}

/**
* @class ResourceManager
* @brief resource loading class
*
* The class that loads all resources, such as textures, shaders, animation times, etc.
*/
class ResourceManager {
public:

	/**
	* A static member. Gets the path to the files
	* @param[in] executablePath std::string&, path to files
	*/
	static void setExecutablePath(const std::string& executablePath);

	/**
	* A static member. Unloads all resources
	*/
	static void unloadAllResources();
	
	ResourceManager() = delete;
	~ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	// Shaders

	/**
	* A static member.
	* @see setExecutablePath()
	* @param[in] shaderName std::string, shader name
	* @param[in] vertexPath std::string, path to the file with vertex shaders
	* @param[in] fragmentPath std::string, path to the file with fragment shaders
	* @return Pointer to shaders program
	*/
	static std::shared_ptr <RenderEngine::ShaderProgram> loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);

	/**
	* A static member.
	* @param[in] shaderName std::string, shader name
	* @return Pointer to shaders programm
	*/
	static std::shared_ptr <RenderEngine::ShaderProgram> getShader(const std::string& shaderName);

	// Texture

	/**
	* A static member.
	* @param[in] textureName std::string, texture name
	* @param[in] texturePath std::string, path to the file with texture
	* @return Pointer to texture
	*/
	static std::shared_ptr <RenderEngine::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);

	/**
	* A static member.
	* @param[in] textureName std::string, texture name
	* @return Pointer to texture
	*/
	static std::shared_ptr <RenderEngine::Texture2D> getTexture(const std::string& textureName);

	// Sprite

	/**
	* A static member.
	* @see getTexture()
	* @see getShader()
	* @param[in] spriteName std::string, sprite name
	* @param[in] textureName std::string, texutre name
	* @param[in] shaderName std::string, shaderName name
	* @param[in] subTextureName std::string, subtexture name
	* @return Pointer to sprite
	*/
	static std::shared_ptr <RenderEngine::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName = "default");

	/**
	* A static member.
	* @param[in] spriteName std::string, sprite name
	* @return Pointer to sprite
	*/
	static std::shared_ptr <RenderEngine::Sprite> getSprite(const std::string& spriteName);

	// TextureAtlas

	/**
	* A static member.
	* @see loadTexture()
	* @param[in] textureName std::string, texutre name
	* @param[in] subTextures std::string, subtexture name
	* @param[in] SubTextureWidth unsigned int, subtexture width
	* @param[in] SubTextureHeight unsigned int, subtexture height
	* @return Pointer to texture atlas
	*/
	static std::shared_ptr <RenderEngine::Texture2D> loadTextureAtlas(std::string texturename, std::string texturePath, std::vector<std::string> subTextures, const unsigned int SubTextureWidth, const unsigned int SubTextureHeight);

	// Include JSON

	/**
	* A static member.
	* @see loadShaderProgram()
	* @see loadTextureAtlas()
	* @param[in] JSONPath std::string, path to JSON file
	* @return results of parsing a JSON file and loading data from it
	*/
	static bool loadJSONResources(const std::string& JSONPath);

	// Levels

	/**
	* A static member.
	* @return A vector of values ​​representing the structure of the map
	*/
	static const std::vector<std::vector<std::string>>& getLevels() { return m_levels; }

	// Shops

	/**
	* A static member.
	* @return A vector of values ​​representing the structure of the shop
	*/
	static const  std::vector<std::vector<std::string>>& getShopsScreen() { return m_shopsScreen; }

	// FightScreen

	/**
	* A static member.
	* @return A vector of values ​​representing the structure of the fight map
	*/
	static const  std::vector<std::string>& getFightScreen() { return m_fightScreen; }

private:

	/**
	* A static member.
	* @param[in] relativeFilePath std::string, path to the resources folder
	* @return Full path to the resources folder
	*/
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

	// FightScreen
	static std::vector<std::string> m_fightScreen;
};