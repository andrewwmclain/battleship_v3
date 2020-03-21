// Title: Lab 6 - battleship.h
//
// Purpose: Declares game board sizes and colors that are needed by
//          multiple code components.
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson

#ifndef BATTLESHIP_BATTLESHIP_H
#define BATTLESHIP_BATTLESHIP_H

// Size of grid
const int COUNT_ROWS = 10;
const int COUNT_COLUMNS = 10;

// Sizes
const int HEIGHT = 2*COUNT_ROWS+1;
const int WIDTH = 2*COUNT_COLUMNS+1;

// Colors that can be used in the application in addiiton to DEFAULT_COLOR
// These are actually indices into a pair of parallel arrays used to initialize colors
// used on the windows (see GameBoard::InitializeDisplay).  Any modifications here
// should be done in tandem with modifications there.
const int RED_INVERSE = 1;
const int GREEN = 2;
const int YELLOW = 3;
const int BLUE = 4;
const int MAGENTA = 5;
const int CYAN = 6;

#endif //BATTLESHIP_BATTLESHIP_H
