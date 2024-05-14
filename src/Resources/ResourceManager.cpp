#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::string ResourceManager::m_path;
ResourceManager::ShaderProgramsMap ResourceManager::m_shaderProgram;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animatedSprites;
std::vector<std::vector<std::string>> ResourceManager::m_levels;


void ResourceManager::setExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

void ResourceManager::unloadAllResources()
{
	m_path.clear();
	m_shaderProgram.clear();
	m_textures.clear();
	m_sprites.clear();
	m_animatedSprites.clear();
}


std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

// Shaders

std::shared_ptr <Renderer::ShaderProgram>  ResourceManager::loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexSrting = getFileString(vertexPath);
	if (vertexSrting.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentSrting = getFileString(fragmentPath);
	if (fragmentSrting.empty())
	{
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr <Renderer::ShaderProgram>& newShader = m_shaderProgram.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexSrting, fragmentSrting)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Can't load shader program:\n" << "Vertex: " << vertexPath << "\n" << "Fragment: " << fragmentPath << std::endl;

	return nullptr;
}

std::shared_ptr <Renderer::ShaderProgram> ResourceManager::getShader(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderProgram.find(shaderName);
	if (it != m_shaderProgram.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}

// Texture

std::shared_ptr <Renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channel = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channel, 0);

	if (!pixels)
	{
		std::cerr << "Can't load texture: " << textureName << std::endl;
		return nullptr;
	}

	std::shared_ptr <Renderer::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<Renderer::Texture2D>(width, height, pixels, channel, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return newTexture;
}

std::shared_ptr <Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the texture: " << textureName << std::endl;
	return nullptr;
}

// Sprite

std::shared_ptr <Renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;
	}

	auto pShader = getShader(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader program: " << shaderName << "for the sprite: " << spriteName << std::endl;
	}

	std::shared_ptr <Renderer::Sprite> newSprite = m_sprites.emplace(spriteName, std::make_shared<Renderer::Sprite>(pTexture, subTextureName, pShader)).first->second;
	
	return newSprite;
}

std::shared_ptr <Renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the sprite: " << spriteName << std::endl;
	return nullptr;
}

// SubTexture :(

std::shared_ptr <Renderer::Texture2D> ResourceManager::loadTextureAtlas(std::string texturename, std::string texturePath, std::vector<std::string> subTextures, const unsigned int SubTextureWidth, const unsigned int SubTextureHeight)
{
	auto pTexture = loadTexture(std::move(texturename), std::move(texturePath));
	if (pTexture)
	{
		const unsigned int textureWidth = pTexture->width();
		const unsigned int textureHeight = pTexture->height();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;
		for (auto& currentSubTextureName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth, static_cast<float>(currentTextureOffsetY - SubTextureHeight) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + SubTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);
			pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);

			currentTextureOffsetX += SubTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= SubTextureHeight;
			}
		}
	}

	return pTexture;
}

// AnimatedSprite

std::shared_ptr <Renderer::AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;
	}

	auto pShader = getShader(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader program: " << shaderName << "for the sprite: " << spriteName << std::endl;
	}

	std::shared_ptr <Renderer::AnimatedSprite> newSprite = m_animatedSprites.emplace(spriteName, std::make_shared<Renderer::AnimatedSprite>(pTexture, subTextureName, pShader)).first->second;

	return newSprite;
}

std::shared_ptr <Renderer::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string& spriteName)
{
	AnimatedSpritesMap::const_iterator it = m_animatedSprites.find(spriteName);
	if (it != m_animatedSprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find animated sprite: " << spriteName << std::endl;
	return nullptr;
}

// Include JSON

bool ResourceManager::loadJSONResources(const std::string& JSONPath)
{
	// Парсинг JSON документа
	const std::string JSONString = getFileString(JSONPath);
	if (JSONString.empty())
	{
		std::cerr << "No JSON resources file!" << std::endl;
		return false;
	}

	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
	if (!parseResult)
	{
		std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "In JSON file: " << JSONPath << std::endl;
		return false;
	}

	// load Shader
	auto shaderIt = document.FindMember("shaders");
	if (shaderIt != document.MemberEnd())
	{
		for (const auto& currentShader : shaderIt->value.GetArray())
		{
			const std::string name = currentShader["name"].GetString();
			const std::string filePath_v = currentShader["filePath_v"].GetString();
			const std::string filePath_f = currentShader["filePath_f"].GetString();

			loadShaderProgram(name, filePath_v, filePath_f);
		}
	}

	// load texture
	auto textureAtlasesIt = document.FindMember("textureAtlases");
	if (textureAtlasesIt != document.MemberEnd())
	{
		for (const auto& currentTextureAtlas : textureAtlasesIt->value.GetArray())
		{
			const std::string name = currentTextureAtlas["name"].GetString();
			const std::string filePath = currentTextureAtlas["filePath"].GetString();
			const unsigned int subTextureWidth = currentTextureAtlas["subTextureWidth"].GetUint();
			const unsigned int subTextureHeight = currentTextureAtlas["subTextureHeight"].GetUint();

			const auto subTexturesArray = currentTextureAtlas["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTexturesArray.Size());
			for (const auto& currentSubTexture : subTexturesArray)
			{
				subTextures.emplace_back(currentSubTexture.GetString());
			}

			loadTextureAtlas(name, filePath, std::move(subTextures), subTextureWidth, subTextureHeight);
		}
	}

	// load Sprite
	auto spriteIt = document.FindMember("sprite");
	if (spriteIt != document.MemberEnd())
	{
		for (const auto& currentSprite : spriteIt->value.GetArray())
		{
			const std::string name = currentSprite["name"].GetString();
			const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
			const std::string shader = currentSprite["shader"].GetString();
			const std::string subTextureName = currentSprite["subTextureName"].GetString();

			auto pSprite = ResourceManager::loadSprite(name, textureAtlas, shader, subTextureName);
			if (!pSprite)
			{
				continue;
			}
		}
	}

	// load Animeted Sprite
	auto animatedSpriteIt = document.FindMember("animatedSprites");
	if (animatedSpriteIt != document.MemberEnd())
	{
		for (const auto& currentAnimatedSprite : animatedSpriteIt->value.GetArray())
		{
			const std::string name = currentAnimatedSprite["name"].GetString();
			const std::string textureAtlas = currentAnimatedSprite["textureAtlas"].GetString();
			const std::string shader = currentAnimatedSprite["shader"].GetString();
			const std::string initialSubTexture = currentAnimatedSprite["initialSubTexture"].GetString();

			auto pAnimatedSprite = ResourceManager::loadAnimatedSprite(name, textureAtlas, shader, initialSubTexture);
			if (!pAnimatedSprite)
			{
				continue;
			}

			const auto statesArray = currentAnimatedSprite["states"].GetArray();
			for (const auto& currentState : statesArray)
			{
				const std::string stateName = currentState["stateName"].GetString();
				std::vector<std::pair<std::string, uint64_t>> frames;
				const auto frameArray = currentState["frames"].GetArray();
				frames.reserve(frameArray.Size());
				for (const auto& currentFrame : frameArray)
				{
					const std::string subTexture = currentFrame["subTexture"].GetString();
					const uint64_t duration = currentFrame["duration"].GetUint64();
					frames.emplace_back(std::pair<std::string, uint64_t>(subTexture, duration));
				}
				pAnimatedSprite->insertState(stateName, std::move(frames));
			}
		}
	}

	// load level
	auto levelIt = document.FindMember("levels");
	if (levelIt != document.MemberEnd())
	{
		for (const auto& currentLevels : levelIt->value.GetArray())
		{
			const auto description = currentLevels["description"].GetArray();
			std::vector<std::string> levelRow;
			levelRow.reserve(description.Size());
			size_t maxLeght = 0;
			for (const auto& currentRow : description)
			{
				levelRow.emplace_back(currentRow.GetString());
				if (maxLeght < levelRow.back().length())
				{
					maxLeght = levelRow.back().length();
				}
			}

			for (auto& currentRow : levelRow)
			{
				while (currentRow.length() < maxLeght)
				{
					currentRow.append("5"); // 5 == void
				}
			}

			m_levels.emplace_back(std::move(levelRow));
		}
	}
	return true;
}