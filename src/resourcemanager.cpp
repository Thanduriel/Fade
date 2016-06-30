#include "resourcemanager.hpp"
#include "config.hpp"

ResourceManager g_resourceManager;

ResourceManager::ResourceManager()
{
	 getShader("light", true);
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

	texture->setSmooth(true);
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
	if (!font->loadFromFile(resourcePath + _fileName + ".ttf"))
	{
		return nullptr;
	}

	m_fonts.emplace_back(_fileName, font);

	return m_fonts.back().data.get();
}

sf::Music* ResourceManager::getTrack(const std::string& _fileName)
{
	for (auto& track : m_tracks)
	{
		if (track.name == _fileName) return track.data.get();
	}

	sf::Music* track = new sf::Music();
	if (!track->openFromFile(resourcePath + _fileName + ".ogg"))
	{
		return nullptr;
	}

	m_tracks.emplace_back(_fileName, track);

	return m_tracks.back().data.get();
}

sf::SoundBuffer* ResourceManager::getSound(const std::string& _fileName)
{
	for (auto& soundBuffer : m_soundBuffers)
	{
		if (soundBuffer.name == _fileName) return soundBuffer.data.get();
	}

	sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
	if (!soundBuffer->loadFromFile(resourcePath + _fileName + ".ogg"))
		return nullptr;

	m_soundBuffers.emplace_back(_fileName, soundBuffer);

	return m_soundBuffers.back().data.get();
}

sf::Shader* ResourceManager::getShader(const std::string& _fileName, bool _geometry)
{
	for (auto& shader : m_shaders)
	{
		if (shader.name == _fileName) return shader.data.get();
	}

	sf::Shader* shader = new sf::Shader();

	static std::string path = resourcePath + "shader/";

	bool b = _geometry ? shader->loadFromFile(resourcePath + "shader/" + _fileName + ".vs", 
		resourcePath + "shader/" + _fileName + ".gs", 
		resourcePath + "shader/" + _fileName + ".ps")
		: shader->loadFromFile(resourcePath + "shader/" + _fileName + ".vs",
		resourcePath+"shader/" + _fileName + ".ps");
	 if (!b)
	{
		return nullptr;
	}

	m_shaders.emplace_back(_fileName, shader);

	return m_shaders.back().data.get();
}