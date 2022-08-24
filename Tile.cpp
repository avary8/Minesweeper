#include "Tile.h"
#include "Board.h"
#include "Images.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include<vector>
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::getline;
using std::ifstream;
using std::shared_ptr;

//initializing static member vars
int Tile::totalRevealed = 0;
int Tile::totalFlags = 0;
bool Tile::debug = 0;
vector<sf::Sprite> Tile::otherTiles;

Tile::Tile(int index, int x, int y) {
	hiddenSprite.setTexture(Images::GetTextures("tile_hidden"));
	this->index = index;
	this->x = x;
	this->y = y;
	hiddenSprite.setPosition(x, y);
}

void Tile::SetMine(bool set) { 
	mine = set;
}

void Tile::SetDebug() {
	debug = !debug;
}

void Tile::setTotalFlags() {
	totalFlags = Board::getMineCount();
}

void Tile::SetFlagged() { 
	flagged = !flagged;
	//cout << "index: " << index  << " " << flagged << endl;
	if (flagged) {
		totalFlags++;
	}
	else {
		totalFlags--;
	}
}

void Tile::setRevealedSprite(vector<shared_ptr<Tile>>& tileSprites) {
	if (!neighborsFound && !mine) {
		findNeighbors(tileSprites);
	}
	if (!isRevealed()) {
		//not setting revealed sprite as mine because debug shows mines, but this function is only called when a tile is revealed (clicked). the nature of whether the tile is revealed is based on this function and total revealed... could make a setRevealed function and manually toggle it after debug... did it this way instead. mines are generated at the tile's location using GetX() and GetY()
		
		/*if (isAMine()) {
			revealedSprite.setTexture(Images::GetTextures("mine"));
		}
		else*/
		if (!isAMine()) {
			if (neighbors == 0) {
				revealedSprite.setTexture(Images::GetTextures("tile_revealed"));
			}
			else {
				revealedSprite.setTexture(Images::GetTextures("number_" + std::to_string(neighbors)));
			}
		}
	}
	revealedSprite.setPosition(x, y);
	revealed = true;
	totalRevealed++;
}

void Tile::setOtherTiles() {
	sf::Sprite happyFaceTile(Images::GetTextures("face_happy"));
	sf::Sprite debugTile(Images::GetTextures("debug"));
	sf::Sprite test1Tile(Images::GetTextures("test_1"));
	sf::Sprite test2Tile(Images::GetTextures("test_2"));
	sf::Sprite test3Tile(Images::GetTextures("test_3"));

	happyFaceTile.setPosition((Board::getWidth() / 2) - 32, Board::getHeight() - 100);
	int tilePos = happyFaceTile.getPosition().x;
	debugTile.setPosition(tilePos + 64 * 2, Board::getHeight() - 100);
	test1Tile.setPosition(tilePos + 64 * 3, Board::getHeight() - 100);
	test2Tile.setPosition(tilePos + 64 * 4, Board::getHeight() - 100);
	test3Tile.setPosition(tilePos + 64 * 5, Board::getHeight() - 100);

	otherTiles.push_back(happyFaceTile);
	otherTiles.push_back(debugTile);
	otherTiles.push_back(test1Tile);
	otherTiles.push_back(test2Tile);
	otherTiles.push_back(test3Tile);
}

void Tile::restart(vector<shared_ptr<Tile>>& tileSprites) {
	totalFlags = 0;
	totalRevealed = 0;
	for (int i = 0; i < tileSprites.size(); i++) {
		tileSprites[i]->getNeighborTiles().clear();
	}
}

bool& Tile::isAMine() {  
	return mine;
}

bool& Tile::isRevealed() {
	return revealed;
}

bool& Tile::isFlagged() {
	return flagged;
}

bool& Tile::isDebug() {
	return debug;
}

vector<sf::Sprite>& Tile::getOtherTiles() {
	return otherTiles;
}

vector<shared_ptr<Tile>>& Tile::getNeighborTiles() {
	return neighborTiles;
}

sf::Sprite& Tile::getHiddenSprite() {
	return hiddenSprite;
}

sf::Sprite& Tile::getRevealedSprite() {
	return revealedSprite;
}

int& Tile::getTotalRevealed() {
	return totalRevealed;
}

int& Tile::getTotalFlags() {
	return totalFlags;
}

int& Tile::getNeighbors() {
	return neighbors;
}

int& Tile::getIndex() {
	return index;
}

int& Tile::getX() {
	return x;
}

int& Tile::getY() {
	return y;
}


void Tile::findNeighbors(vector<shared_ptr<Tile>>& tileSprites) { //kinda ugly but finds all neighbors by manipulating index and where it is in relation to board
	int col = Board::getCol();
	int row = Board::getRow();
	bool isTopRow = index < col;
	bool isFlushLeft = index % col == 0;
	bool isFlushRight = (index + 1) % col == 0;
	bool isBottomRow = index >= (col * row - col);

	//upper row neighbors
	if (!isTopRow) {
		if (!isFlushLeft) {
			neighborTiles.push_back(tileSprites[index - col - 1]); //upper left
		}

		neighborTiles.push_back(tileSprites[index - col]); //upper mid

		if (!isFlushRight) {
			neighborTiles.push_back(tileSprites[index - col + 1]); //upper right
		}
	}

	//same row neighbors
	if (!isFlushLeft) {
		neighborTiles.push_back(tileSprites[index - 1]); //left
	}
	if (!isFlushRight) {
		neighborTiles.push_back(tileSprites[index + 1]); //right

	}
	//lower row neighbors
	if (!isBottomRow) {
		if (!isFlushLeft) {
			neighborTiles.push_back(tileSprites[index + col - 1]); //lower left
		}

		neighborTiles.push_back(tileSprites[index + col]); //lower mid

		if (!isFlushRight) {
			neighborTiles.push_back(tileSprites[index + col + 1]); //lower right
		}
	}
	//counts which neighbors are mines. need to do it here because we need to know which tiles to have in vector if no neighbors are mines
	for (int i = 0; i < neighborTiles.size(); i++) {
		if (neighborTiles[i]->isAMine()) {
			neighbors++;
		}
	}

	if (neighbors != 0) {
		neighborTiles.clear();
	}
	neighborsFound = true;
}

