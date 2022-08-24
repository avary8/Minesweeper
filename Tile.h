#pragma once
#include "Random.h"
#include <SFML/Graphics.hpp>
#include<string>
#include<vector>
using std::string;
using std::vector;
using std::shared_ptr;

class Tile {
public:	
	Tile();
    Tile(int index, int x, int y);
	void SetMine(bool set);
	static void SetDebug();
	static void setTotalFlags();
	void SetFlagged();//this function set flagged to !flagged , then increments/decrements totalFlags based on bool
	void setRevealedSprite(vector<shared_ptr<Tile>>& tileSprites);
	static void setOtherTiles();
	static void restart(vector<shared_ptr<Tile>>& tileSprites);

	bool& isAMine();
	bool& isRevealed();
	bool& isFlagged();
	static bool& isDebug();

	static vector<sf::Sprite>& getOtherTiles();
	vector<shared_ptr<Tile>>& getNeighborTiles();
	sf::Sprite& getHiddenSprite();
	sf::Sprite& getRevealedSprite();

	static int& getTotalRevealed();
	static int& getTotalFlags();
	int& getNeighbors();
	int& getIndex();
	int& getX();
	int& getY();
	

private:
	void findNeighbors(vector<shared_ptr<Tile>>& tileSprites);
	sf::Sprite hiddenSprite;
	sf::Sprite revealedSprite;
	vector<shared_ptr<Tile>> neighborTiles;
	static vector<sf::Sprite> otherTiles;
	int index = 0;
	int x = 0;
	int y = 0;
	int neighbors = 0;
	static int totalFlags;
	static int totalRevealed;
	static bool debug;
	bool mine = false;
	bool neighborsFound = false;
	bool flagged = false;
	bool revealed = false;	
};


