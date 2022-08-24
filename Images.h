#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include<unordered_map>
using std::string;
using std::unordered_map;

//similar to texture manager file prof fox outlined

class Images {
public:
	static sf::Texture& GetTextures(string textureName);
	static void Clear();
private:
	static unordered_map<string, sf::Texture> textures;
};

