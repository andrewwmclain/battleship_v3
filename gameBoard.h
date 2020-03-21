// Title: Lab 6 - gameBoard.h
//
// Purpose: Declares the GameBoard class which is instantiated by the main program
//          and provides 1) user interface, 2) game functions for main program to call
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson

#ifndef BATTLESHIP_GAMEBOARD_H
#define BATTLESHIP_GAMEBOARD_H

#include <string>
#include "gridWindow.h"
#include "commandWindow.h"

using namespace std;

//  Class providing user interface and game functionality that is directly called
//  by the main program
//
class GameBoard {
public:
    GameBoard();

    // Ship placement
    bool LoadShipsFromFile(bool forUser, const string& fileName);
    void RandomlyPlaceShips(bool forUser);

    // Bring up initial display
    bool ShowInitialDisplay();

    // Fire a shot
    bool FireShot(bool forUser, int row, int column, Outcome& outcome);

    // Keyboard interface
    void WritePrompt(const string& message, int color=DEFAULT_COLOR, int attrib=A_STANDOUT);
    void WriteResponse(const string& message, int color=DEFAULT_COLOR, int attrib=A_DIM);
    string GetLine();

private:
    // Main window
    MainWindow _mainWindow;

    // Pair of grids - one for user, one for CPU
    GridWindow _user;
    GridWindow _cpu;
    HGroup _gridGrouping;

    // Command window
    CommandWindow _commandWindow;
};

#endif //BATTLESHIP_GAMEBOARD_H
