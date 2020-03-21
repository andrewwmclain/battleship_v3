// Title: Lab 6 - gameBoard.cpp
//
// Purpose: Implement the GameBoard class which is instantiated by the main program
//          and provides 1) user interface, 2) game functions for main program to call
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson and Andrew McLain

#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>
#include "gameboard.h"

// Titles
const string BATTLESHIP = "BATTLESHIP";
const string USER = "User";
const string CPU = "CPU";

//  Class providing user interface and game functionality that is directly called
//  by the main program
//

//
//  Constructor
GameBoard::GameBoard() :
        _user(USER, true),
        _cpu(CPU, false),
        _gridGrouping("GridGrouping"),
        _mainWindow(true, BATTLESHIP, "",
                     CENTER, CENTER,
                     DEFAULT_COLOR, DEFAULT_COLOR, A_STANDOUT) {
}

//  Connect up the subwindows and display
//  Parameters:
//      none
//  Returns:
//      success/failure
//  Possible errors:
//      none expected
bool GameBoard::ShowInitialDisplay() {
    // Pairs of colors to be used for foreground and background color combinations
    // The colors defined in battleship.h are indices into these parallel arrays
    // If you change these colors, then you need to modify the color definitions in battleship.h
        // Note: These colors can be changed as desired, but six extra colors (in addition
        // to the default color) seem to be the limit.  It appears that if you add a 7th
        // pair of colors to the array it becomes the default color pair
    int fgColors[] = { COLOR_WHITE, COLOR_BLACK, COLOR_BLACK, COLOR_WHITE, COLOR_WHITE, COLOR_BLACK };
    int bgColors[] = { COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN };

    // Create the two grid views
    _user.Init();
    _cpu.Init();

    // Group the two grids horizontally
    _gridGrouping.AddChild(&_user.DisplayArea());
    _gridGrouping.AddChild(&_cpu.DisplayArea());

    // Create the command window
    _commandWindow.Init();

    // Group the above to vertically to make the main window
    _mainWindow.AddChild(&_gridGrouping);
    _mainWindow.AddChild(&_commandWindow.DisplayArea());

    // Attempt initial display
    if (_mainWindow.Display(fgColors, bgColors, sizeof(fgColors)/sizeof(fgColors[0]) )) {
        _user.Display();
        _cpu.Display();
        return true;
    }
    return false;
}

//  Write text to the prompt area
//  Parameters:
//      message - text to write
//      color - color to use
//      attrib - optional text attribute
//  Returns:
//      nothing
//  Possible Errors:
//      none
void GameBoard::WritePrompt(const string& message, int color, int attrib) {
    _commandWindow.WritePrompt(message, color, attrib);
}

//  Write text to the response area
//  Parameters:
//      message - text to write
//      color - color to use
//      attrib - optional text attribute
//  Returns:
//      nothing
//  Possible Errors:
//      none
void GameBoard::WriteResponse(const string& message, int color, int attrib) {
    _commandWindow.WriteResponse(message, color, attrib);
}

//  Get line of text that the user has entered
//  Parameters:
//      none
//  Returns:
//      text string
//  Possible Errors:
//      none
string GameBoard::GetLine() {
    return _commandWindow.GetLine();
}

// Methods for you to add: LoadShips, RandomlyPlaceShips, and FireShots

//Loads ships from file for either the user or cpu gridWindow
//Parameters:
//  bool forUser
//  const string& fileName
//Returns:
//  true if ships can be loaded, false if not
//Possible errors:
//  invalid file name, invalid ship configuration in file
bool GameBoard::LoadShipsFromFile(bool forUser, const string& fileName){
    ifstream fin;
    //check if file will open
    fin.open(fileName);
    if(!fin.is_open()){
        return false;
    }
    //if it is the user gridWindow, load ships there
    if(forUser){
        if(!_user.LoadShips(fin)){
            return false;
        }
        return true;
    }
    //if it is the cpu gridWindow, load ships there
    if(!_cpu.LoadShips(fin)){
        return false;
    }
    return true;
}

//Randomly places ships on given gridWindow
//Parameters:
//  bool forUser
//Returns:
//  nothing
//Possible errors:
//  none
void GameBoard::RandomlyPlaceShips(bool forUser){
    if(forUser){
        _user.RandomlyPlaceShips();
    }
    else{
        _cpu.RandomlyPlaceShips();
    }
}

//Fires shots on a grid with given row and column, updating the outcome
//Parameters:
//  bool forUser
//  int row
//  int column
//  Outcome& outcome
//Returns:
//  true if row and col are valid
//  false if row and col are invalid
//Possible errors:
//  invalid row or col coordinates
bool GameBoard::FireShot(bool forUser, int row, int column, Outcome& outcome){
    if(forUser){
        return (_user.FireShot(row, column, outcome));
    }
    return (_cpu.FireShot(row, column, outcome));
}