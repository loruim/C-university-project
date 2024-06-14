#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

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
std::vector<std::vector<std::string>> ResourceManager::m_levels;
std::vector<std::vector<std::string>> ResourceManager::m_shopsScreen;


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

std::shared_ptr <RenderEngine::ShaderProgram>  ResourceManager::loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
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

	std::shared_ptr <RenderEngine::ShaderProgram>& newShader = m_shaderProgram.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexSrting, fragmentSrting)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Can't load shader program:\n" << "Vertex: " << vertexPath << "\n" << "Fragment: " << fragmentPath << std::endl;

	return nullptr;
}

std::shared_ptr <RenderEngine::ShaderProgram> ResourceManager::getShader(const std::string& shaderName)
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

std::shared_ptr <RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
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

	std::shared_ptr <RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<RenderEngine::Texture2D>(width, height, pixels, channel, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return newTexture;
}

std::shared_ptr <RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
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

std::shared_ptr <RenderEngine::Sprite> ResourceManager::loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName)
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

	std::shared_ptr <RenderEngine::Sprite> newSprite = m_sprites.emplace(spriteName, std::make_shared<RenderEngine::Sprite>(pTexture, subTextureName, pShader)).first->second;
	
	return newSprite;
}

std::shared_ptr <RenderEngine::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the sprite: " << spriteName << std::endl;
	return nullptr;
}

// TextureAtlas

std::shared_ptr <RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(std::string texturename, std::string texturePath, std::vector<std::string> subTextures, const unsigned int SubTextureWidth, const unsigned int SubTextureHeight)
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
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - SubTextureHeight + 0.01f) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + SubTextureWidth - 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);
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

// Include JSON

bool ResourceManager::loadJSONResources(const std::string& JSONPath)
{
	// ������� JSON ���������
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

	// load texture atlas
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
			const std::string subTextureName = currentSprite["initialSubTexture"].GetString();

			auto pSprite = ResourceManager::loadSprite(name, textureAtlas, shader, subTextureName);
			if (!pSprite)
			{
				continue;
			}

			auto framesIt = currentSprite.FindMember("frames");
			if (framesIt != currentSprite.MemberEnd())
			{
				const auto framesArray = framesIt->value.GetArray();
				std::vector<RenderEngine::Sprite::FrameDescription> framesDescriptions;
				framesDescriptions.reserve(framesArray.Size());
				for (const auto& currentFrame : framesArray)
				{
					const std::string subTextureStr = currentFrame["subTexture"].GetString();
					const double duration = currentFrame["duration"].GetDouble();
					const auto pTextureAtlas = getTexture(textureAtlas);
					const auto pSubTexture = pTextureAtlas->getSubTexture(subTextureStr);
					framesDescriptions.emplace_back(pSubTexture.leftBottomUV, pSubTexture.rightTopUV, duration);
				}
				pSprite->insertFrames(std::move(framesDescriptions));
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

	// load Shops Screen
	auto shopsScreenIt = document.FindMember("shopsScreen");
	if (shopsScreenIt != document.MemberEnd())
	{
		const auto descriptionArray = shopsScreenIt->value.GetArray();
		m_shopsScreen.reserve(descriptionArray.Size());
		std::vector<std::string> ShopsRow;
		ShopsRow.reserve(descriptionArray.Size());
		size_t maxLenght = 0;
		for (const auto& currentRow : descriptionArray)
		{
			ShopsRow.emplace_back(currentRow.GetString());
			if (maxLenght < ShopsRow.back().length())
			{
				maxLenght = ShopsRow.back().length();
			}
		}

		for (auto& currentRow : ShopsRow)
		{
			while (currentRow.length() < maxLenght)
			{
				currentRow.append("5"); // 5 == void
			}
		}
		m_shopsScreen.emplace_back(std::move(ShopsRow));
	}
	return true;
}