#include "ResourceManager.hpp"

ResourceManager g_resourceManager;

const std::string resourcePath = "resources/";

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}


sf::Texture* ResourceManager::getTexture(const std::string& _fileName)
{
	for (auto& texture : m_textures)
	{
		if (texture.name == _fileName) return texture.data.get();
	}

	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(resourcePath + _fileName))
	{
		return nullptr;
	}

	m_textures.emplace_back(_fileName, texture);

	return m_textures.back().data.get();
}

sf::Font* ResourceManager::getFont(const std::string& _fileName)
{
	for (auto& font : m_fonts)
	{
		if (font.name == _fileName) return font.data.get();
	}

	sf::Font* font = new sf::Font();
	if (!font->loadFromFile(resourcePath + "fonts/" + _fileName + ".ttf"))
	{
		return nullptr;
	}

	m_fonts.emplace_back(_fileName, font);

	return m_fonts.back().data.get();
}

sf::Shader* ResourceManager::getShader(const std::string& _fileName, bool _geometry)
{
	for (auto& shader : m_shaders)
	{
		if (shader.name == _fileName) return shader.data.get();
	}

	sf::Shader* shader = new sf::Shader();

	bool b = _geometry ? shader->loadFromFile("shader/" + _fileName + ".vs", "shader/" + _fileName + ".gs", "shader/" + _fileName + ".ps")
		: shader->loadFromFile("shader/" + _fileName + ".vs", "shader/" + _fileName + ".ps");
	if (!b)
	{
		return nullptr;
	}

	m_shaders.emplace_back(_fileName, shader);

	return m_shaders.back().data.get();
}