#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <vector>
#include <memory>

enum struct DefaultShader
{
	Light
};

template< typename _T>
struct Resource
{
	Resource(const std::string& _name, _T* _data) : name(_name), data(_data){}
	Resource(Resource&& _oth) :
		name(std::move(_oth.name)), data(std::move(_oth.data)){}
	std::string name;
	std::unique_ptr< _T > data;
};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	sf::Texture* getTexture(const std::string& _fileName);
	sf::Font* getFont(const std::string& _fileName);
	sf::Music* getTrack(const std::string& _fileName);
	sf::SoundBuffer* getSound(const std::string& _fileName);
	sf::Shader* getShader(const std::string& _fileName, bool _geometry = false);

	sf::Shader* getShader(DefaultShader _shader) { return m_shaders[static_cast<int>(_shader)].data.get(); };
private:
	std::vector < Resource< sf::Texture > > m_textures;
	std::vector < Resource< sf::Font > > m_fonts;
	std::vector < Resource< sf::Shader > > m_shaders;
	std::vector < Resource< sf::Music > > m_tracks;
	std::vector < Resource< sf::SoundBuffer > > m_soundBuffers;
};


extern std::unique_ptr<ResourceManager> g_resourceManager;

