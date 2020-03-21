// Title: Lab 6 - commandWindow.h
//
// Purpose: Declares the C++ CommandWindow class to facilitate user
//          interaction.
//
//          Consists of a vertical grouping of a PlotWindow to
//          display a prompt, an InputWindow to allow the
//          the user to enter a command, and another PlotWindow
//          to display the game status.
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson

#ifndef BATTLESHIP_COMMANDWINDOW_H
#define BATTLESHIP_COMMANDWINDOW_H

#include "cursesWindow.h"

class CommandWindow {
public:
    CommandWindow();

    // Display Initialization
    void Init();
    VGroup& DisplayArea();

    // For interacting with user
    void WritePrompt(const string& message, int color=DEFAULT_COLOR, int attrib=A_STANDOUT);
    void WriteResponse(const string& message, int color=DEFAULT_COLOR, int attrib=A_DIM);
    string GetLine();

private:
    PlotWindow _commandPrompt;
    PlotWindow _commandResponse;
    InputWindow _commandInput;
    VGroup _commandGrouping;
};


#endif //BATTLESHIP_COMMANDWINDOW_H
