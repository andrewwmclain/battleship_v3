// Title: Lab 6 - commandWindow.cpp
//
// Purpose: Implement the method of C++ methods of the
//          CommandWindow class to facilitate user interaction.
//
//          Consists of a vertical grouping of a PlotWindow to
//          display a prompt, an InputWindow to allow the
//          the user to enter a command, and another PlotWindow
//          to display the game status.
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson

#include "battleship.h"
#include "commandWindow.h"

// Sizes
const int COMMAND_AREA_WIDTH = HEIGHT+WIDTH+4;

// Class Constructor
//
CommandWindow::CommandWindow() :
    _commandPrompt( "CommandPrompt", 1, COMMAND_AREA_WIDTH),
    _commandResponse( "CommandResponse", 1, COMMAND_AREA_WIDTH),
    _commandInput("CommandInput", COMMAND_AREA_WIDTH),
    _commandGrouping("CommandGrouping", true) {
}

//
// Public member functions
//

// Perform initial work before displaying UI elements.
//      Specifically, add the three window classes to
//      VGroup so they will be stacked vertically when
//      they are displayed.
// Parameters:
//      none
// Returns:
//      nothing
// Possible Errors:
//     none
void CommandWindow::Init() {
    _commandGrouping.AddChild(&_commandPrompt);
    _commandGrouping.AddChild(&_commandInput);
    _commandGrouping.AddChild(&_commandResponse);
}

// Return a reference to the VGroup so display can be
//      triggered
// Parameters:
//      none
// Returns:
//      reference to the VGroup
// Possible Errors:
//     none
VGroup& CommandWindow::DisplayArea() {
    return _commandGrouping;
}

//  Replace existing text in the prompt area with new
//      text string, then call refresh
// Parameters:
//      message - text to display
//      color - color of text
//      attrib - rendering attribute for text
// Returns:
//      nothing
// Possible Errors:
//     none
void CommandWindow::WritePrompt(const string& message, int color, int attrib) {
    _commandPrompt.Erase();
    _commandPrompt.Write(0, 0, message, color, attrib);
    _commandPrompt.Refresh();
}

// Get input user enters into input area
// Parameters:
//      none
// Returns:
//      string
// Possible Errors:
//     none
string CommandWindow::GetLine() {
    return _commandInput.GetInput();
}

//  Replace existing text in the response area with new
//      text string, then call refresh
// Parameters:
//      message - text to display
//      color - color of text
//      attrib - rendering attribute for text
// Returns:
//      nothing
// Possible Errors:
//     none
void CommandWindow::WriteResponse(const string& message, int color, int attrib) {
    _commandResponse.Erase();
    _commandResponse.Write(0, 0, message, color, attrib);
    _commandResponse.Refresh();
}
