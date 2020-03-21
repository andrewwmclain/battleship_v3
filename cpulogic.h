// Title: Lab 5 - cpulogic.h
//
// Purpose: Declares a class that encapsulates the logic the computer uses to fire shots
//
// Class: CSC 2430 Winter 2020
// Authors: Max Benson and Andrew McLain

#ifndef BATTLESHIP_CPULOGIC_H
#define BATTLESHIP_CPULOGIC_H

#include "grid.h"
#include "gameBoard.h"

// This class has two methods.
//
// DetermineShot - figures out the next shot to take based on information
//                 it knows about the outcomes of previous shots
// ReportOutcome - this method is called after the shot is made to
//                 tell the computer logic its outcome
//
class CpuLogic {
public:
    CpuLogic();

    void DetermineShot(int &row, int& column) const;
    void ReportOutcome(int row, int column, Outcome outcome);

private:
    SquareStatus _squareStatus[COUNT_ROWS][COUNT_COLUMNS];
};

#endif //BATTLESHIP_CPULOGIC_H
