#include "Board.h"
#include "Images.h"
#include "Tile.h"
#include <iostream>
#include<vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include<unordered_map>
using std::vector;
using std::cout;
using std::endl;
using std::unordered_map;
using std::shared_ptr;

//2 bugs
// 1. test board 1: flag something. then click. you win even though something is flagged
// 2. negative counter statys negative when you lose
// 
//copy 2
//this vers uses smart pointers for both main tile vector and neighbor tile vector

int main() {
    Board board; //set up board width, height, num mines etc
    sf::RenderWindow window(sf::VideoMode(board.getWidth(), board.getHeight()), "Minesweeper");
    Tile::setOtherTiles();
    vector<shared_ptr<Tile>> tileSprites;
    board.InitBoard(tileSprites, window, -1);

    window.display();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            //switch statement way.uses event type.
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) { 
                    sf::Vector2i pos = sf::Mouse::getPosition(window); //gets both x and y positions of mouse relative to window (and not screen)
                    /*cout << "left mouse button pressed: " << endl;
                    cout << "left mouse x pos " << pos.x << endl;
                    cout << "left mouse y pos " << pos.y << endl;   */
                    if (pos.y < board.getHeight() - 100 && pos.x < board.getCol() * 32 && Board::getPlayable()) {
                        window.clear();
                        board.Reveal(pos, tileSprites, window);
                    }
                    else if (pos.y < board.getHeight() && pos.x < board.getCol() * 32){
                        int target = board.getButton(pos);
                        if (target == 1 && Board::getPlayable()) { //debug doesn't do anything per proj 3 pdf rubric
                            Tile::SetDebug();
                            board.updateBoard(tileSprites, window);
                        }
                        else if (target == 0 || (target > 1 && target < 5)) { //0 or 2,3,4
                            Tile::restart(tileSprites);
                            tileSprites.clear();
                            board.InitBoard(tileSprites, window, target-1);
                            window.display();
                        }
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i pos = sf::Mouse::getPosition(window); //gets both x and y positions of mouse relative to window (and not screen)
                    /*cout << "right mouse button pressed: " << endl;
                    cout << "right mouse x pos " << pos.x << endl;
                    cout << "right mouse y pos " << pos.y << endl;*/
                    if (pos.y < board.getHeight() - 100 && pos.x < board.getCol() * 32 && Board::getPlayable()) {
                        window.clear();
                        board.addFlag(pos, tileSprites, window);
                    }
                }
                break;
            }    
        }
    }
    Images::Clear(); 
    return 0;
}


