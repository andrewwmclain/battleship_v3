(update)
# LAB 6 - Battleship Game, Part 3 - Completing the Game
## Goal
Complete the Battleship game.  You will write code to display the game board using a C++ class library built on top of the  _ncurses_ character graphics library, as well as write logic that contols the game. 

## Sample Run

Your program will start out in command line mode, prompting the user for a random seed and asking the user how to place the ships on the user and CPU grids.  In the run below, the user’s ship configuration was read from a file, while the CPU’s ship configuration was determined randomly.

![initialConfiguration.txt](/image1.png)

After configuration has been completed, the game board will be displayed.  The user will manually fire shots, alternating with the CPU algorithmically firing shots.  

The three views of the game board that follow illustrate how ships are displayed when they have not been hit, are partially hit, or completely sunk.  The manner that ships on the user grid are displayed varies from how they are displayed on the CPU grid.

![ShipDisplayA](/image2a.png)
 
This first screen shot shows the initial display.  The user’s ships are displayed on the grid using different colors, and the squares are identified by the first letter of the ship’s name.  The CPU’s ships are not shown.

![ShipDisplayB](/image2b.png)

The second screen shot shows what the game board looks like after a number of volleys have been fired.  The squares of the user’s destroyer that have been hit are now shown in red with white lettering.  The user has been able to hit one square of the CPU’s ship.  It shows as simply a red square without the letter giving a clue to how long it is.

![ShipDisplayC](/image2c.png)

In this third screen shot, the CPU ship has now been sunk.  It continues to be displayed in red, but now the identifying letters are  shown.  

If play continues, eventually either the user or CPU will win – the user won the game displayed below.

![GameOver](/image3.png)

This game board will remain displayed until the user enters a character. 

## Files you are given
The following files are supplied for you:

* **CMakeLists.txt** - This contains the information necessary to build the project.  This file should not be modified.

*	**main.cpp** – This file will contain the main routine that drives the game.  This is left for you to implement.

*	**battleship.h** – This file defines several constants that multiple classes need for their implementation.  This file has been completed for you and should not be modified.

*	**gameBoard.h** – This file declares the _GameBoard_ class which the main program calls to drive the game.  This file has been completed for you and should not be modified.

*	**gameBoard.cpp** – This file implements the _GameBoard_ class.  Methods that are specific to bringing up the initial display of the game board have been implemented for you.  The rest of the methods have been left for you to implement.

*	**gridWindow.h** – This file declares the _GridWindow_ class which bundles together the functionality of the _Grid_ class and display functionality. This file has been completed for you and should not be modified.

*	**gridWindow.cpp** – This file implements the _GridWindow_ class.  Methods that are specific to bringing up the initial display of the game board have been implemented for you.  The rest of the methods have been left for you to implement.

*	**grid.h** – This file contains the definition of the _Grid_ ADT as a C++ class.  This file is the same as the **grid.h** you used in Lab 5, except that the definition of _COUNT_ROWS_ and _COUNT_COLUMNS_ have been moved to **battleship.h**.  This file has been completed for you and should not be modified.

*	**commandWindow.h** - This file declares the _CommandWindow_ class which represents the lower panel of the game board where a prompt string can be displayed, a user can type in a command, and a response can be written.  This file has been completed for you and should not be modified.

*	**commandWindow.cpp** - This file implements the _CommandWindow_ class which represents the lower panel of the game board where a prompt string can be displayed, a user can type in a command, and a response can be written.  This file has been completed for you and should not be modified.

*	**cursesWindow.h** – This file declares a set of C++ classes that wrap the functionality of the _ncurses_ library.  This file has been completed for you and should not be modified.

*	**cursesWindow.cpp** – This file implements a set of C++ classes that wrap the functionality of the _ncurses_ library.    This file has been completed for you and should not be modified.

In your repo you will find a PowerPoint file: **LAB6-Explained.pptx** that explains the steps and grading rubric.

You should copy in to this project the files **grid.cpp**, **cpuLogic.cpp**, and **cpuLogic.h** that wrote in Lab 5.

## Step 1 – plumb the _GameBoard_ methods through to a _GridWindow_ object

The _GameBoard_ object instantiates two _GridWindow_ objects, one for the user (_ _user_) and one for the CPU (_ _cpu_).   The three _GameBoard_ methods that are left for you to implement direct their calls into either the _ _user_ object or the _ _cpu_ object based on the value of the _forUser_ parameter (`forUser == true` when dealing with `_user`, `forUser == false` when dealing with `_cpu`):

Your job in this step is to add implementations to **gameBoard.cpp** for the following methods.

1)	_bool GameBoard::LoadShipsFromFile(bool forUser, const string& fileName)_

This method should open an input file stream.  If the file cannot be opened successfully, you should simply return _false_.  If it can be opened, call the _LoadShips_ method on the appropriate grid object and return the result. 

2)	_void GameBoard::RandomlyPlaceShips(bool forUser)_

Call the _RandomlyPlaceShips_ method on the appropriate grid object.

3)	_bool GameBoard::FireShot(bool forUser, int row, int column, Outcome& outcome)_

Call the _FireShot_ method on the appropriate grid object and return the result.

Once these three methods have been correctly implemented, the file **gameBoard.cpp** is complete.

## Step 2 – plumb the _GridWindow_ methods relating to ship configuration through to the _Grid_ object

The _GridWindow_ object instantiates a _Grid_ object (_ _grid_).  A call to _LoadShips_ or _RandomlyPlaceShips_ on the _GridWindow_ object should in turn call into the _ _grid_ object.

Your job in this step is to add implementations to **gridWindow.cpp** for these methods.

1)	_bool GridWindow::LoadShips(ifstream& file)_

All you have to do is call the _LoadShips_ method on the _ _grid_ object.
‘
2)	_void GridWindow::RandomlyPlaceShips()_

In order to call the _RandomlyPlaceShips_ method, you need to pass an array of ships and its size.  To be consistent with the real Battleship game, here are the ships that you should use:

![ListOfShips](/image4.png)

Here is how you can declare and initialize an array for this:

![ArrayOfShips](/image5.png)

And, here is an expression you can use to count the array elements without insert a “magic number” in your code.

![NumberOfShips](/image6.png)

## Step 3 – Start the implementation of the main program

Now start work in **main.cpp**.  Add a main function that does the following:

1)	Prompts the user to enter a random seed, and then calls _srand_ with it.
2)	Prompts the user for a file name to read in the ship configuration for the user (or ENTER for random ship placement).
3)	Prompts the user for a file name to read in the ship configuration for the CPU (or ENTER for random ship placement).
4)	Calls _game.ShowInitialDisplay()_ to bring up the game window.
5)	Calls _getch()_ – this delays the program’s exit so you can see the display!

Tips:
•	Since 2) and 3) are very, very similar, you should design a function to do this, rather repeating a lot of code.
•	Make sure you check errors

When you’ve got this done, you should be able to compile your Battleship program, configure the ship configuration of both the user and CPU grids either from a file or by random placement, and then see what is shown in the screen shot.

![NoLinesInitialDisplay](/image7.png)

If your window size is too small, CTRL-C, resize, and rerun.

## Step 4 – Displaying the grid lines

The method _GridWindow::Display_ controls the display of the interior of the grid.  Currently this method does nothing.  Let’s get the ball rolling by displaying grid lines.  

The code write the grid lines is not especially hard, but a bit tedious.  We’ve saved you the trouble by including the method _GridWindow::DisplayLines_.

Here are the two lines you should add to your _Display_ method:

1)	_DisplayLines();_
2)	_ _plot.Refresh();_

Why is the second line necessary?  The _ncurses_ library caches update you make to the screen, and doesn’t display them until you make a _refresh_ call.  Delaying calling _refresh_ until you are finished making updates makes the visual experience better.

Now when you recompile and run, the initial display you see is shown in this screen shot.

![NoShipsInitialDisplay](/image8.png)

## Step 5 – Initial display of the ships

Now let’s display the ships on the grid.   To do this, you need to add some code to _GridWindow::Display_ before _ _plot.Refresh_ is called to display each of the ships on the grid. 

When you write this code, you should not assume how many ships there are, their names, their locations, or their sizes.  The _ _grid_ object holds that information, and your code needs to make calls on this object to get that that information.

For each ship, you should call _GridWindow::DisplayShip_ with a ship color.  Since we only have a limited number (_COLORS_MAX_) of colors available, and we could have more ships than colors, a good way to specify the color for the “i-th” ship is

_ _colors[i % COLORS_MAX]_

Make sure you understand how this works – it’s a commonly used technique.

Once you have added this code to _GridWindow::Display_, now it’s time to add an implementation for the function _GridWindow::DisplayShip_.  We’ve isolated the logic into a separate function because it will also be called as the game proceeds and ships get fired on.

This method needs to go through each square the ship sits on and write a character if that square should be displayed.  
*	For the user’s grid, _GridWindow::DisplayShip_ will write one character for each square of a ship. 
*	For the CPU’s grid, _GridWindow::DisplayShip_ only writes characters for squares of ships which have been hit.

The details of how to do this are left to you – refer to the screen shots at the beginning of the write up.  Note that  _RED_INVERSE_ is color you should use for squares of ships that have been hit rather than the color passed in as a parameter.

Once you write this code, and recompile, your initial display should resemble what you see in the screen shot.  

![ShipsInitialDisplay](/image9.png)

## Step 6 – Finishing the implementation of _GridWindow::FireShot_

The method _GridWindow::FireShot_ needs to start out by calling the _FireShot_ method on the _ _grid_ object.   That’s how we get the outcome of the shot.

Then you need to add code based on the outcome to display the outcome on the screen.

*	SHOT_HERE_BEFORE: nothing to do
*	SHOT_MISSED: write an X character in the appropriate square
*	SHIP_HIT, SHIP_SUNK, or GAME_WON: figure out what ship it is and use _DisplayShip_ to update its display

Don’t forget to call _ _plot.Refresh_ when you are done or your updates won’t display!

Congratulations! You’ve finished **gridWindow.cpp**.   But you won’t be able to see changes in the user interface until you finish the next step.

## Step 7 – Enabling game play

Now it’s time to complete the main program, and the game itself.  The game needs to operate in a loop.  On each iteration it will let the user fire, then let the CPU fire.  If either the user’s shot or the CPU’s shot returns with outcome, the loop should terminate after updating the window with the results.

For the user:

1)	Call the method _GameBoard::WritePrompt_ to write a prompt to the user to enter the location of the square to fire at.
2)	Call _GameBoard::GetLine_ to find out the string that the user typed
3)	Validate the user’s input – if the location is not in the right format, use _GameBoard::WriteResponse_ to write an informative error message, then repeat steps 1) and 2).  Do steps 1)-3) until you have a valid location.
4)	Convert the location string into row and column numbers
5)	Call _GameBoard::FireShot_
6)	Using an _ostringstream_ object, begin building a response to display to the user 

For the CPU (assuming the user hasn’t just won the game):

1)	 Call _CpuLogic::DetermineShot_ to determine the row and column
2)	 Call _GameBoard::FireShot_
3)	 Call _CpuLogic::ReportOutcome_
4)	 Finish building a response for the user

At the end of the iteration of each loop use _GameBoard::WriteResponse_ to output the outcome of the user and CPU’s shots.

## Style

Your program needs to be orderly and readable.  If you are working a development team and don’t write clean looking code, you will be unpopular among your colleagues and may not work on that team very long.  Because of this your program will be graded on style as well as correctness.  Please refer to the style guidelines discussed in class and in reading assignments concerning

  *	Indention of code
  *	Consistent use of {}, preferably Stroustrup style
  *	Meaningful names of identifiers
  *	Naming conventions for identifiers (camelCase for variables, CamelCase for function names. First word of a function name typically should be a verb.)
  *	No use of global variables. Global variables sometimes are appropriate, but not in the assignments we will be giving this quarter.
  *	Making sure *struct* and object parameters are passed by reference (and declared const if appropriate)
 
## Documentation
    
At the start of your file you should have a header comment that gives the title of the assignment, the purpose, and your name.  Here is an example (from Lab 1) of what that could look like:

      ```
      /*
          Title:      Lab 1 – evalSteps.cpp
          Purpose:    implement the GetEvalSteps functions called by the main program to generate the evaluation
                      steps for an expression passed in as a parameter.
          Author:     Sally Johnson
          Date:       January 6, 2020
      */
      ```
Each subordinate function should also start with a header that describes what it does, it’s parameters, what it returns, and how errors are handled.  Here is an example (again from Lab 1):

    ```
    // Produce a string of evaluation steps for simplifying an expression
    // For example:
    //      if “2+3*4-5”  is passed to this function, the string “2+3*4-5 = 2+12-5\n        = 14-5\n        = 9”
    //      would be returned
    // Parameters:
    //      expression - an arithmetic expression
    // Returns:
    //      a string consisting of the evaluation steps needed to simplify the expression
    // Possible Error Conditions: 
    //      If evaluating the expression causes division by zero, this function crash.
    //      If evaluating the expression causes an oveflow, the results may not be reliable.
    ```

You should include additional comments in your code to describe what you are doing.   If it is hard to understand what a variable is for, add a comment after it.   It possible, though, to put in too many comments, so be judicious and make sure you have time left to do well in your other classes when it comes to commenting.

## Submitting your code
Your solution should be contained within files that were provided.  Of the supplied files, the only ones you should modify are **main.cpp**, **gameBoard.cpp**, and **gridWindow.cpp**.

Make sure you test your code thoroughly.  

Your code needs to be submitted through GitHub Classroom.  You will need to push your last version of your program before the lab deadline. As a good programming practice remember to commit frequently and to push every time you have a functioning version of your code.

## Grading
Correctness is essential.  Make sure your solution builds as described above and correctly handles the test cases displayed in the screen shot.  We will test on other input values as well. Your code must compile and should not have runtime errors (crash).

Even if your solution operates correctly, points will be taken off for:
- Not following the design described above
- Not adhering to style guidelines described above
- Using techniques not presented in class
- Programming error not caught by other testing
- Not following good programming practices

## Academic Integrity
This programming assignment is to be done on an individual basis. At the same time, it is understood that learning from your peers is valid and you are encouraged to talk among yourselves about programming in general and current assignments in particular.  Keep in mind, however, that each individual student must do the work in order to learn.  Hence, the following guidelines are established:
- Feel free to discuss any and all programming assignments but do not allow other students to look at or copy your code. Do not give any student an electronic or printed copy of any program you write for this class.
- Gaining the ability to properly analyze common programming errors is an important experience. Do not deprive a fellow student of his/her opportunity to practice problem solving: control the urge to show them what to do by writing the code for them.
- If you’ve given the assignment a fair effort and still need help, see the instructor or a lab assistant.
- **If there is any evidence that a program or other written assignment was copied from another student, neither student will receive any credit for it. This rule will be enforced.**
- Protect yourself: Handle throw-away program listings carefully, keep your repository private.

Refer to the ECS Department Policy on Academic Integrity that is included in the class syllabus.

## Grading Rubric Summary
The following aspects are going to be consider during grading. Make sure you comply with all of them.
- The program compiles (there will be no partial credit for programs that do not compile)
- Provides the correct output for the test cases
- Catches errors gracefully, the program does not crash on incorrect input
- The program outputs the information in the specified format
- The assignment follows all the instructions
- In general the program does not crash 
