// Title: Lab 6 - main.cpp
//
// Purpose: Drive the Battleship game, first asking the user how to configure each of the
//          grids (by reading a file or randomly) and then displaying the game board and
//          allowing the user to start playing
//
// Class: CSC 2430 Winter 2020
// Author: Andrew McLain

#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>
#include "gameBoard.h"
#include "cpulogic.h"

using namespace std;

void PromptConfig(string fileName, const bool& user, GameBoard& game);

int main(){
    string randomSeed;
    string userFileName;
    string cpuFileName;
    GameBoard game;
    Outcome outcome = SHOT_HERE_BEFORE;
    CpuLogic cpu;
    string line;
    int row;
    int col;
    int userRow;
    int userCol;

    //prompt user for seed
    cout << "Enter a random seed: ";
    getline(cin, randomSeed);
    srand(stoi(randomSeed));

    //prompt user for user file name
    cout << "Enter file name for user ship configuration or press [ENTER] for random ship placement" << endl;
    getline(cin, userFileName);
    PromptConfig(userFileName, true, game);

    //prompt user for cpu file name
    cout << "Enter file name for cpu ship configuration or press [ENTER] for random ship placement" << endl;
    getline(cin, cpuFileName);
    PromptConfig(cpuFileName, false, game);

    //Display initial board before shots are fired - CPU ships are not displayed
    game.ShowInitialDisplay();
    getch();

    //Loop prompting the user to enter coordinates until valid coordinates entered, then firing
    //a shot accordingly
    while(true){
        //prompt user for coordinates, store results in response ostringstream
        ostringstream response;
        game.WritePrompt("Enter coordinates, e.g. 1B");
        line = game.GetLine();

        //prompt user for coordinates until non-empty coordinates are typed
        while( line == "" ){
            line = game.GetLine();
        }
        userRow = static_cast<int>(line[0]) - 48;
        userCol = static_cast<int>(line[1]) - 65;
        //prompt user for coordinates until valid coordinates typed, then fire shot
        while(!game.FireShot(false, userRow, userCol, outcome)){
            if(outcome == SHOT_HERE_BEFORE){
                game.WriteResponse("Shot here before");
            }
            else{
                game.WriteResponse("Invalid coordinate");
            }
            game.WritePrompt("Enter coordinates, e.g. 1B");
            line = game.GetLine();

            while( line == "" ) {
                line = game.GetLine();;
            }
            userRow = static_cast<int>(line[0]) - 48;
            userCol = static_cast<int>(line[1]) - 65;
        }
        //store user fire location in response ostringstream to display to user
        response << "You : " << line << " -> ";
        //store fire outcome in response ostringstream to display to user
        switch(outcome){
            case SHOT_MISSED:
                response << "MISS";
                break;
            case SHIP_HIT:
                response << "HIT";
                break;
            case SHIP_SUNK:
                response << "SUNK";
                break;
            case GAME_WON:
                response << "WON, Press [ENTER] to quit";
                break;
            case SHOT_HERE_BEFORE:
                response << "SHOT_HERE_BEFORE";
                break;
        }
        response << ", ";
        //display the response
        game.WriteResponse(response.str());
        //if the game is won, exit the loop with enter key
        if(outcome == GAME_WON){
            game.GetLine();
            break;
        }

        //cpu determines shot and fires, location and result of firing stored in response
        cpu.DetermineShot(row, col);
        game.FireShot(true, row, col, outcome);
        cpu.ReportOutcome(row, col, outcome);
        response << "CPU : " << row << static_cast<char>(col + 65) << " -> ";
        switch(outcome){
            case SHOT_MISSED:
                response << "MISS";
                break;
            case SHIP_HIT:
                response << "HIT";
                break;
            case SHIP_SUNK:
                response << "SUNK";
                break;
            case GAME_WON:
                response << "WON, Press [ENTER] to quit";
                break;
            case SHOT_HERE_BEFORE:
                response << "SHOT_HERE_BEFORE";
                break;
        }
        game.WriteResponse(response.str());
        if(outcome == GAME_WON){
            game.GetLine();
            return 0;
        }
    }

    return 0;
}


//Prompts user for file name or random placement and executes either instruction
//Parameters:
//  string fileName
//  const bool& user
//  Gameboard& game
//Returns:
//  nothing
//Possible errors:
//  invalid file name or invalid ship configuration
void PromptConfig(string fileName, const bool& user, GameBoard& game){
    //if file name is empty, fire randomly
    if(fileName.size() == 0){
        game.RandomlyPlaceShips(user);
    }
    else{
        while(!game.LoadShipsFromFile(user, fileName)){
            cerr << "File '" << fileName << "' could not be loaded.";
            getline(cin, fileName);
            continue;
        }
    }
}