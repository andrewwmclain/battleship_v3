// Title: Lab 5 - grid.cpp
//
// Purpose:  Convert the  Battleship "grid" ADT implemented in Lab 4 to
//           use a C++ class as well as firing functionality.
//
// Class: CSC 2430 Winter 2020
// Author: Andrew McLain
//

#include <iostream>
#include <assert.h>
#include "grid.h"

// Class constructor
//  - calls Init to ensure the class
//

Grid::Grid(){
    Init();
}

//
// Public member functions
//

// Read the ship configuration for a grid from a text file stream.  Here's an example of
//      how the file is structured:
//          5
//          Submarine
//          3 1 0 0
//          Battleship
//          4 0 9 0
//          Destroyer
//          3 0 3 3
//          PatrolBoat
//          2 1 4 4
//          Carrier
//          5 1 4 5
//      where the first line is the count of ships.  Two lines are used to specify the
//      configuration of a ship.  The first line gives the ship's name.  The second line
//      contains four numbers specifying size, isVertical, startRow, and startColumn
// Parameters:
//      file - an open file stream
// Returns:
//      succcess/failure
// Possible Errors:
//     File read error
//     # of ships out of range
//     Value of isVertical, startRow or startColumn out of range
//     Ship placement would extend beyong grid
//     Ship placement would overlap ship that has already been placed

bool Grid::LoadShips(ifstream& file){
    string name;
    int size;
    bool isVertical;
    int row;
    int column;
    int shipsDeployed;
    //if the file cannot be read properly, return false to LoadShipsOnGrid
    if(!file.good()){
        return false;
    }
    //get # of ships deployed, if there are too many return false to LoadShipsOnGrid
    file >> shipsDeployed;
    if(shipsDeployed > SHIPS_MAX){
        cerr << "Line 1: too many ships specified\n";
        return false;
    }
    //for the number of ships deployed, try adding each one to the grid. If unable to
    //do so return false to LoadShipsOnGrid
    for(size_t i = 0; i < shipsDeployed; i++){
        file >> name;
        file >> size;
        file >> isVertical;
        file >> row;
        file >> column;
        if(!AddShip(name, size, isVertical, row, column)){
            cerr << "Line " << i * 2 + 3 << ": ship '" << name << "' cannot be added.\n";
            return false;
        }
    }
    return true;
}

// Save the ship configuration for a grid to a text file stream.  For the file format
//      Refer to the header comment for Load.
// Parameters:
//      file - an open file stream
// Returns:
//      succcess/failure
// Possible Errors:
//     File write error

bool Grid::SaveShips(ofstream& file){
    //if the file cannot be copied to, return false to SaveShipsOnGrid
    if(!file.good()){
        return false;
    }
    //copy # of ships deployed on first line
    file << _shipsDeployed << "\n";
    //for each ship, input the properly formatted info to the file
    for(size_t i = 0; i < _shipsDeployed; i++){
        file << _ships[i].name << "\n";
        file << _ships[i].size << " ";
        file << _ships[i].isVertical << " ";
        file << _ships[i].startRow << " ";
        file << _ships[i].startColumn << "\n";
    }
    return true;
}

// Randomly place the indicated ships on an empty grid.
//      The grid is initialized and then random values of isVertical, startRow, and startColumn
//      are generated to place the ships.  If the random values would result in a placement that
//      does not fit within the grid or which overlaps an existing ship placement, new random
//      values will generated.  Multiple of attempts for each ship will be made until each of the
//      ships can be placed on the grid without extending beyond the grid boundaries or overlapping.
// Parameters:
//      ship - an array of ship structs with their name and size fields filled in -- other
//             fields of the ship structs are ignored
//      countShips - the number of ships in the array -- must not exceed SHIPS_MAX
// Returns:
//      nothing
// Possible Errors:
//      none

void Grid::RandomlyPlaceShips(const Ship ships[], int countShips){
    //Initialize the grid
    Init();
    bool isVertical;
    int startRow;
    int startColumn;
    //for each ship, try adding it to the grid with the random configuration.
    //if it doesn't fit, generate a new random configuration and try again
    for(size_t i = 0; i < countShips; i++){
        do{
            isVertical = rand() % 2;
            startRow = rand() % COUNT_ROWS;
            startColumn = rand() % COUNT_COLUMNS;
        }while(!AddShip(ships[i].name, ships[i].size, isVertical, startRow, startColumn));
    }
}

// Attempt to place a ship on the grid.  Checks that the placement would not cause
//      the ship to extend outside the grid or overlap another ship that has already
//      been placed on the grid.
// Parameters:
//      name - name of ship to be placed
//      size - number of squares the ship covers
//      isVertical - whether the ship should be placed vertically or horizontally
//      startRow - row of uppermost square (if isVertical) or leftmost square (if !isVertical)
//      startColumn - column of uppermost square (if isVertical) or leftmost square (if !isVertical)
// Returns:
//      true if the ship can be placed, false if it would extend outside grid or overlap another ship
// Possible Errors:
//      Too many ships deployed
//      Size or position of ship is not reasonable
//      Ship placement would extend outside gird
//      Ship placement would overlap another ship

bool Grid::AddShip(const string& name, int size, bool isVertical, int startRow, int startColumn){
    //if no more ships can be added, return false to calling function
    if(_shipsDeployed > 5){
        return false;
    }
    //if ship is being placed off the grid, return false to calling function
    if(startRow >= COUNT_ROWS || startRow < 0 ||
       startColumn >= COUNT_COLUMNS || startColumn < 0){
        return false;
    }
    //make sure the ship will not go off the grid before designating squares
    //and placing it
    if(isVertical){
        if(startRow + size > COUNT_ROWS){
            return false;
        }
        for(size_t j = 0; j < size; j++){
            if(_squareStatus[startRow + j][startColumn] != WATER) {
                return false;
            }
        }
        for(size_t j = 0; j < size; j++){
            _squareStatus[startRow + j][startColumn] = SHIP;
        }
    }
    else {
        if (startColumn + size > COUNT_COLUMNS) {
            return false;
        }
        for(size_t j = 0; j < size; j++){
            if(_squareStatus[startRow][startColumn + j] != WATER){
                return false;
            }
        }
        for(size_t j = 0; j < size; j++){
            _squareStatus[startRow][startColumn + j] = SHIP;
        }
    }

    //update the ships array in the grid
    _shipsDeployed++;
    _ships[_shipsDeployed - 1].hits = 0;
    _ships[_shipsDeployed - 1].name = name;
    _ships[_shipsDeployed - 1].size = size;
    _ships[_shipsDeployed - 1].isVertical = isVertical;
    _ships[_shipsDeployed - 1].startRow = startRow;
    _ships[_shipsDeployed - 1].startColumn = startColumn;

    return true;
}


// Returns the number of ships that have been sunk
// Parameters:
//      none
// Returns:
//      number of ships
// Possible Errors:
//      none

int Grid::GetShipsSunk() const{
    return _shipsSunk;
}


// Returns the number of ships placed on the grid
// Parameters:
//      none
// Returns:
//      number of ships
// Possible Errors:
//      none

int Grid::GetShipsDeployed() const{
    return _shipsDeployed;
}



// Retrieves information about the i-th ship.  Meant to used
// in a loop to fetch information about each the ships
// Parameters:
//      i - index of ship that ranges between 0 to number of ships deployed -1
//      ship - struct to be filled with information about ship
// Returns:
//      nothing
// Possible Errors:
//      asserts that index is within range

void Grid::GetShip(int i, Ship& ship) const{
//    cout << i << "\n";
    assert(i >= 0 && i < _shipsDeployed);
    ship = _ships[i];
}

// Determines if there is a ship covering the specified square.  If found, returns
// the configuration index of the ship
// Parameters:
//      row - row of the square
//      column - column of the square
// Returns:
//      < 0 if not found, otherwise returns the index of the ship
// Possible Errors:
//      none

int Grid::FindShip(int row, int column) const{
    //make sure the square being searched exists on the grid
    assert(row < COUNT_ROWS && row >= 0 &&
           column < COUNT_COLUMNS && column >= 0);
    //if there is a ship on that square, find out which ship it is
    if(_squareStatus[row][column] == SHIP || _squareStatus[row][column] == HIT ||
        _squareStatus[row][column] == SUNK){
        //check each ship in grid's ship array to see if it is on the square
        for(size_t i = 0; i < _shipsDeployed; i++){
            //if the ship is vertical
            if(_ships[i].isVertical){
                if(_ships[i].startColumn == column){
                    if(_ships[i].startRow <= row && _ships[i].startRow + _ships[i].size >= row){
                        return i;
                    }
                }
            }
                //if the ship is horizontal
            else{
                if(_ships[i].startRow == row){
                    if(_ships[i].startColumn <= column && _ships[i].startColumn + _ships[i].size >= column){
                        return i;
                    }
                }
            }
        }
        //if no ship is found, return -1
        return -1;
    }
    //if no ship is on that square, return -1
    return -1;
}


// Gets the status of the specified square
// Parameters:
//      row - row of the square
//      column - column of the square
// Returns:
//      status of the square
// Possible Errors:
//      asserts that row nad column are within range

SquareStatus Grid::GetSquareStatus(int row, int column) const{
    assert(row < COUNT_ROWS && row >= 0 &&
           column < COUNT_COLUMNS && column >= 0);
    return _squareStatus[row][column];
}


// Fires a shot on the specified square.  Updates the two dimensional array and determines the outcome
// Parameters:
//      row - row of the square
//      column - column of the square
//      outcome - upon return this is set equal to the outcome
// Returns:
//      true if shot lands within grid, false otherwise
// Possible Errors:
//      row or column is out of range

bool Grid::FireShot(int row, int column, Outcome& outcome){
    if(row >= COUNT_ROWS || row < 0 ||
       column >= COUNT_COLUMNS || column < 0){
        return false;
    }
    if(_squareStatus[row][column] != WATER && _squareStatus[row][column] != SHIP){
        outcome = SHOT_HERE_BEFORE;
        return true;
    }
    else if(_squareStatus[row][column] == WATER){
        outcome = SHOT_MISSED;
        _squareStatus[row][column] = MISS;
        return true;
    }
    else {
        int index = FindShip(row, column);
        _ships[index].hits++;
        if (_ships[index].hits < _ships[index].size) {
            _squareStatus[row][column] = HIT;
            outcome = SHIP_HIT;
            return true;
        }
        else {
            if(_ships[index].isVertical){
                for(size_t i = 0; i < _ships[index].size; i++){
                    _squareStatus[_ships[index].startRow + i][_ships[index].startColumn] = SUNK;
                }
                outcome = SHIP_SUNK;
            }
            else {
                for(size_t i = 0; i < _ships[index].size; i++){
                    _squareStatus[_ships[index].startRow][_ships[index].startColumn + i] = SUNK;
                }
                outcome = SHIP_SUNK;
            }
//            If not all ships have been sunk, report SHIP_SUNK and return true
            for(size_t i = 0; i < _shipsDeployed; i++){
                if(_ships[i].hits < _ships[i].size){
                    return true;
                }
            }
//            Otherwise report GAME_WON and return true
            outcome = GAME_WON;
            return true;
        }
    }
}

//
// Private member functions
//

// Initialize the grid struct
//      Set counts of deployed and sunk ships to zero
//      Set the SquareStatus of all grid squares equal to Water
// Parameters:
//      none
// Returns:
//      nothing
// Possible Errors:
//      none

void Grid::Init(){
    //set everything to 0
    _shipsDeployed = 0;
    _shipsSunk = 0;;
    //set each square to water
    for(size_t i = 0; i < COUNT_ROWS; i++){
        for(size_t j = 0; j < COUNT_COLUMNS; j++){
            _squareStatus[i][j] = WATER;
        }
    }
}





