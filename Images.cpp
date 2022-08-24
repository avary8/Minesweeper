#include "Images.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include<unordered_map>
#include <iostream>
#include <string>
using std::string;
using std::unordered_map;
using std::cout;
using std::endl;
//static var
unordered_map<string, sf::Texture> Images::textures; 

sf::Texture& Images::GetTextures(string textureName) {
	if (textures.find(textureName) == textures.end()) {
		//a load file function as part of GetTextures, rather than another function
		string fileName = "images/" + textureName + ".png";
		textures[textureName].loadFromFile(fileName);
	}
	//cout << "texture size at x: " << textures["hiddenTile"].getSize().x << endl;
	//cout << "GetTextures() called" << endl;
	return textures[textureName];
}

void Images::Clear() {
	textures.clear();
}