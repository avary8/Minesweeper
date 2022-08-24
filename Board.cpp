#include "Board.h"
#include "Images.h"
#include "Tile.h"
#include "Random.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <math.h>
#include <unordered_map>
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
int Board::width = 0;
int Board::height = 0;
int Board::col = 0;
int Board::row = 0;
int Board::mineCount = 0;
bool Board::playable = 1;
bool Board::winCond = 0;
bool Board::loseCond = 0;

Board::Board() {
	string configVal;
	ifstream config;
	config.open("boards/config.cfg");
	getline(config, configVal);
	col = stoi(configVal);
	getline(config, configVal);
	row = stoi(configVal);
	getline(config, configVal);

	width = col * 32;
	height = row * 32 + 100;
	mineCount = stoi(configVal);
	tileCount = row * col;
	config.close();
}

//target determines whether the board is randomly generated or from a testBoard and is the testBoard # 
void Board::InitBoard(vector<shared_ptr<Tile>>&tileSprites, sf::RenderWindow & window, int target) {
	Board board;	
	if (!playable) { //after a win/lose need to reset the win/lose conditions and set smiley face 
		restart(tileSprites);
	}
	int index = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			tileSprites.push_back(shared_ptr<Tile>(new Tile(index, 0 + (32 * j), 0 + (32 * i))));
			window.draw(tileSprites[index]->getHiddenSprite());
			/*cout << index << " : " << tileSprites[index].isAMine() << endl;  */
			index++;
		}
	}

	if (target == -1) {
		setRandomMines(tileSprites);
	} 
	else {
		setFileMines(target, tileSprites);
	}

	if (Tile::isDebug()) {
		revealMines(tileSprites, window, 0);
	}

	for (int i = 0; i < Tile::getOtherTiles().size(); i++) {
		window.draw(Tile::getOtherTiles()[i]);
	}
	drawCounter(window);
}

void Board::setFileMines(int target, vector<shared_ptr<Tile>>& tileSprites) {
	int index = 0;
	string fileName = "testboard" + std::to_string(target) + ".brd";
	string configVal, temp;
	ifstream config;
	config.open("boards/" + fileName);
	mineCount = 0;
	while (getline(config, configVal)) {
		for (int i = 0; i < configVal.size(); i++) {
			temp = configVal.at(i);
			tileSprites[index]->SetMine(stoi(temp));
			index++;
			//reassigning mine count derived from config file to actual mine count based on testboard
			if (stoi(temp) == 1) {
				mineCount++;
				//cout << "minecount " << mineCount << endl;
			}
		}
	}
}

void Board::setRandomMines(vector<shared_ptr<Tile>>& tileSprites) {
	for (int i = 0; i < mineCount; i++) {
		int index = Random::Int(0, tileSprites.size()-1);
		tileSprites[index]->SetMine(!tileSprites[index]->isAMine());
		//account for when our random number chooses a tile that is already a mine
		if (!tileSprites[index]->isAMine()) {
			i-=2;
		}
	}
}

void Board::updateBoard(vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window) {
	sf::Sprite flag(Images::GetTextures("flag"));
	for (int i = 0; i < tileCount; i++) {
		window.draw(tileSprites[i]->getHiddenSprite());
		if (tileSprites[i]->isFlagged()) {
			flag.setPosition(tileSprites[i]->getX(), tileSprites[i]->getY());
			window.draw(flag);
		}
		else if (tileSprites[i]->isRevealed()) {
			window.draw(tileSprites[i]->getRevealedSprite());
		}
	}

	if (Tile::isDebug()) {
		revealMines(tileSprites, window, 0);
	}
	if (!getPlayable()) {
		drawEndConditions(tileSprites, window);
		Tile::setTotalFlags(); // all mines are found..counter is 0 when you win
	}
	for (int i = 0; i < Tile::getOtherTiles().size(); i++) {
		window.draw(Tile::getOtherTiles()[i]);
	}
	drawCounter(window);
	window.display();
}

void Board::Reveal(sf::Vector2i& pos, vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window) {
	int xNumbered = pos.x / 32;
	int yNumbered = pos.y / 32;
	int position = (yNumbered * col) + xNumbered;
	if (!tileSprites[position]->isFlagged()) {
		if (!tileSprites[position]->isAMine()) {
			RecursiveReveal(position, tileSprites);
			fixMineCount(tileSprites); //recursive function breaks mineCount ...??
		}
		if (tileSprites[position]->isAMine()) {
			loseCond = 1;
		}
		else if (Tile::getTotalRevealed() == tileCount - mineCount && Tile::getTotalFlags() == 0) {
			winCond = 1;
		}
	}
	/*cout << "total revealed " << Tile::getTotalRevealed() << endl;
	cout << "tile Count " << tileCount << endl;
	cout << "mine Count " << mineCount << endl;*/
	updateBoard(tileSprites, window);
}

void Board::RecursiveReveal(int position, vector<shared_ptr<Tile>>& tileSprites) {
	//recursive reveal messes up mine count var
	if (!tileSprites[position]->isFlagged()) {
		tileSprites[position]->setRevealedSprite(tileSprites);
		vector<shared_ptr<Tile>> neighborTiles = tileSprites[position]->getNeighborTiles(); //shortens the call for it
		if (!tileSprites[position]->isAMine()) {
			if (tileSprites[position]->getNeighbors() == 0) {
				for (int i = 0; i < tileSprites[position]->getNeighborTiles().size(); i++) {
					if (!neighborTiles[i]->isAMine() && !neighborTiles[i]->isRevealed() && !neighborTiles[i]->isFlagged() && neighborTiles[i]->getNeighbors()== 0) {
						RecursiveReveal(neighborTiles[i]->getIndex(), tileSprites);
					}
				}
			}
		}
	}
}

void Board::addFlag(sf::Vector2i& pos, vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window) {
	int position = ((pos.y / 32) * col) + (pos.x / 32);
	if (!tileSprites[position]->isRevealed()) {
		tileSprites[position]->SetFlagged();
	}
	updateBoard(tileSprites, window);
}

void Board::revealMines(vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window, int choice) {
	sf::Sprite flag(Images::GetTextures("flag"));
	sf::Sprite mine(Images::GetTextures("mine"));
	sf::Sprite revealed(Images::GetTextures("tile_revealed"));
	for (int i = 0; i < tileSprites.size(); i++){
		if (tileSprites[i]->isAMine()) {
			if (choice == 0) { //user lost or debug
				if (loseCond) {
					revealed.setPosition(tileSprites[i]->getX(), tileSprites[i]->getY());
					window.draw(revealed);
				}
				mine.setPosition(tileSprites[i]->getX(), tileSprites[i]->getY());
				window.draw(mine);
			}
			else if (choice == 1) { //user won
				flag.setPosition(tileSprites[i]->getX(), tileSprites[i]->getY());
				window.draw(flag);
			}
		}
	}
}

void Board::drawCounter(sf::RenderWindow& window) {
	int counter = mineCount - Tile::getTotalFlags();
	cout << "total flags: " << Tile::getTotalFlags() << endl;
	cout << "counter: " << counter << endl;
	sf::Sprite digit(Images::GetTextures("digits"));
	if (counter < 0) {
		counter *= -1;
		digit.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
		digit.setPosition(0, height - 100);
		window.draw(digit);
	}
	for (int i = 0; i < 3; i++) {
		int offset = counter % 10;
		digit.setTextureRect(sf::IntRect(21 * offset, 0, 21, 32));
		digit.setPosition(63 - (i * 21), height - 100);
		window.draw(digit);
		counter /= 10;
	}

}

void Board::drawEndConditions(vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window) {
	if (winCond) {
		Tile::getOtherTiles()[0].setTexture(Images::GetTextures("face_win"));
		revealMines(tileSprites, window, 1);
	}
	if (loseCond) {
		Tile::getOtherTiles()[0].setTexture(Images::GetTextures("face_lose"));
		revealMines(tileSprites, window, 0);
	}
}

void Board::fixMineCount(vector<shared_ptr<Tile>>& tileSprites) {
	mineCount = 0;
	for (int i = 0; i < tileSprites.size(); i++) {
		if (tileSprites[i]->isAMine()) {
			mineCount++;
		}
	}
}

void Board::restart(vector<shared_ptr<Tile>>& tileSprites) {
	winCond = 0;
	loseCond = 0;
	Tile::getOtherTiles()[0].setTexture(Images::GetTextures("face_happy"));
	playable = 1;
}

int Board::getButton(sf::Vector2i& pos) {
	int target = -1;
	if (pos.y >= height-100 && pos.y <= (height-100)+64) {
		for (int i = 0; i < Tile::getOtherTiles().size(); i++) {
			if (pos.x > Tile::getOtherTiles()[i].getPosition().x && pos.x < Tile::getOtherTiles()[i].getPosition().x + 64) {
				target = i;
			}
		}
	}
	return target;
}

int Board::getWidth() {
	return width;
}

int Board::getHeight() {
	return height;
}

int Board::getCol() {
	return col;
}

int Board::getRow() {
	return row;
}

int Board::getMineCount() {
	return mineCount;
}

int Board::getTileCount() {
	return tileCount;
}

bool Board::getPlayable() {
	if (winCond || loseCond) {
		playable = 0;
	}
	return playable;
}