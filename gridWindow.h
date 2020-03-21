// Title: Lab 6 - gridWindow.h
//
// Purpose: Declare the GridWindow class which bundles the behind the scenes Grid
//          class functionality with the display elements.
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson

#ifndef BATTLESHIP_GRIDWINDOW_H
#define BATTLESHIP_GRIDWINDOW_H

#include "cursesWindow.h"
#include "grid.h"

const int COLORS_MAX = SHIPS_MAX;

//  Class that bundles the behind the scenes Grid class functionality with the display
//      elements
class GridWindow {
public:
    GridWindow(string title, bool isUser);

    // Ship placement
    bool LoadShips(ifstream& file);
    void RandomlyPlaceShips();

    // Two-stage initialization of the display
    void Init();
    void Display();
    VGroup& DisplayArea();

    // Firing shots
    bool FireShot(int row, int column, Outcome& outcome);

private:
    // Submethods called by Display method
    void DisplayLines();
    void DisplayShip(const Ship& ship, int color=DEFAULT_COLOR);

    // User interface elements
    PlotWindow _plot;
    VGroup _plotWithLabels;
    VGroup _labeledPlotWithTitle;
    int _colors[COLORS_MAX];

    // Game state
    Grid _grid;

    // Who am I?
    bool _isUser;
};


#endif //BATTLESHIP_GRIDWINDOW_H
