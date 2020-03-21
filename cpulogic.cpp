// Title: Lab 5 - cpulogic.cpp
//
// Purpose: Implements the logic the computer needs to determin the next shot it fires
//
// Class: CSC 2430 Winter 2020
// Author: Andrew McLain
//

#include <assert.h>
#include "cpulogic.h"
#include "gameBoard.h"

// Class Constructor

CpuLogic::CpuLogic(){

}
// Determine the next shot to take
//  Parameters:
//      row - row it determines for the next shot
//      column -- column it determines for the next shot
//  Returns:
//      nothing
//  Possible Errors:
//      none

void CpuLogic::DetermineShot(int &row, int& column) const{
    do{
        row = rand() % COUNT_ROWS;
        column = rand() % COUNT_COLUMNS;
    }while(_squareStatus[row][column] == MISS ||
           _squareStatus[row][column] == HIT ||
           _squareStatus[row][column] == SUNK &&
           row < COUNT_ROWS && column < COUNT_COLUMNS);
}

//  Method that is called to inform the class about the outcome of a shot
//  Parameters:
//      row - location of shot
//      column - location of shot
//      outcome - outcome of firing the shot
//  Returns:
//      nothing
//  Possible Errors:
//      none

void CpuLogic::ReportOutcome(int row, int column, Outcome outcome){
    if(outcome == SHOT_MISSED) {
        _squareStatus[row][column] = MISS;
    }
    else if(outcome == SHIP_HIT) {
        _squareStatus[row][column] = HIT;
    }
    //if outcome == GAME_WON or SHIP_SUNK. Already determined outcome != SHOT_HERE_BEFORE.
    else {
        _squareStatus[row][column] = SUNK;
    }
}

