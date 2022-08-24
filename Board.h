#pragma once
#include "Images.h"
#include"Tile.h"
#include<unordered_map>
#include<vector>
#include <SFML/Graphics.hpp>
using std::vector;
using std::unordered_map;

class Board {
public:
	Board();
	void InitBoard(vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window, int target);
	//setFileMines(...)
	//setRandomMines(...)

	void updateBoard(vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window);
	void Reveal(sf::Vector2i& pos, vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window);
	//setRecursiveReveal(...)
	void addFlag(sf::Vector2i& pos, vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window);
	void revealMines(vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window, int choice);

	//drawCounter(...)
	//drawEndConditions(...)
	//fixMineCount(...)
	void restart(vector<shared_ptr<Tile>>& tileSprites);

	int getButton(sf::Vector2i& pos);
	static int getWidth();
	static int getHeight();
	static int getCol();
	static int getRow();
	static int getMineCount();
	int getTileCount();
	static bool getPlayable();
	
private:
	void setFileMines(int target, vector<shared_ptr<Tile>>& tileSprites);
	void setRandomMines(vector<shared_ptr<Tile>>& tileSprites);
	void RecursiveReveal(int position, vector<shared_ptr<Tile>>& tileSprites);
	void drawCounter(sf::RenderWindow& window);
	void drawEndConditions(vector<shared_ptr<Tile>>& tileSprites, sf::RenderWindow& window);
	void fixMineCount(vector<shared_ptr<Tile>>& tileSprites);
	static int col;
	static int row;
	static int width;
	static int height;
	static int mineCount;
	int tileCount;
	static bool winCond;
	static bool loseCond;
	static bool playable; 
};

