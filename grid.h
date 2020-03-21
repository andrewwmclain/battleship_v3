// Title: Lab 6 - grid.h
//
// Purpose: Declares the functions that manipulate a battleship grid.
//          This version uses a C++ class to represent a grid
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson

#ifndef BATTLESHIP_GRID_H
#define BATTLESHIP_GRID_H

#include <fstream>
#include "battleship.h"

using namespace std;

// Maximum number of ships on board
const int SHIPS_MAX = 5;

// Possible states for a square on the grid
// depending on whether a ship occupies it
// or a shot has landed there
enum SquareStatus { WATER, SHIP, MISS, HIT, SUNK };

// Possible outcomes for a shot
enum Outcome { SHOT_MISSED, SHIP_HIT, SHIP_SUNK, GAME_WON, SHOT_HERE_BEFORE };

// Describes a ship and its placement on the grid
//      name - name of ship, its first letter is displayed
//      size - number of squares it occupies
//      isVertical - if true, the ship is positioned vertically, else it's horizontal
//      startRow - row (0-9) of uppermost (if vertical) or leftmost (if horizontal) square it occupies
//      startColumn - column (0-9) of uppermost (if vertical) or leftmost (if horizontal) square it occupies
//      hits - number of different squares that ship occupies that have been hit, it's sunk if hits == size
struct Ship {
    string name;
    int size;
    bool isVertical;
    int startRow;
    int startColumn;
    int hits;
};

// Describes the state of the grid
//      ships - the ships placed on teh grid
//      shipsDeployed -- the number of ships that are on the grip (<= SHIPS_MAX)
//      shipsSunk -- the number of ships that have been sunk (game is over if == shipsDeployed)
class Grid {
public:
    Grid();

    bool LoadShips(ifstream& file);
    bool SaveShips(ofstream& file);

    void RandomlyPlaceShips(const Ship ships[], int shipCount);

    bool AddShip(const string& name, int size, bool isVertical, int startRow, int startColumn);
    int GetShipsSunk() const;
    int GetShipsDeployed() const;
    void GetShip(int i, Ship& ship) const;
    int FindShip(int row, int column) const;

    bool FireShot(int row, int column, Outcome& outcome);

    SquareStatus GetSquareStatus(int row, int column) const;

private:
    void Init();

    Ship _ships[SHIPS_MAX];
    int _shipsDeployed;
    int _shipsSunk;
    SquareStatus _squareStatus[COUNT_ROWS][COUNT_COLUMNS];
};

#endif //BATTLESHIP_GRID_H
