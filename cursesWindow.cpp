// Title: cursesWindow.cpp
//
// Purpose: Implement a set of  C++ classes that wrap the
//          ncurses library.
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson

#include <iostream>
#include <assert.h>
#include "cursesWindow.h"

//
//  Base windowing class for all the other windowing classes

//
//  Constructor
BaseWindow::BaseWindow(const string& name) {
    m_name = name;
    m_pwindow = nullptr;
}

//
//  Destructor
//      Frees WINDOW handle
BaseWindow::~BaseWindow() {
    if (m_pwindow) {
        delwin(m_pwindow);
    }
}

//
//  Class that windowing classes used for plotting or getting
//      input are derived from.

//
//  Constructor
Content::Content(const string& name) :
        BaseWindow(name) {
}

//  Stores the x,y coordinate of the upper left corner
//      of this window with respect to its parent window
//  Parameters:
//      x - x-coordinate
//      y - y-coordinate
//  Returns:
//      nothing
//  Possible errors:
//      none
void Content::SetPosition(int x, int y) {
    m_xULWindow = x;
    m_yULWindow = y;
}

//
//  Windowing class used for plotting text.

//
//  Constructor
PlotWindow::PlotWindow(const string& name, int height, int width)
        : Content(name) {
    m_height = height;
    m_width = width;
}

//  Determine how many lines are required to display this window
//      For this class the # of lines is specified when constructed
//  Parameters:
//      None
//  Returns:
//      height needed
//  Possible errors:
//      none
int PlotWindow::RequiredHeight() {
    return m_height;
}

//  Determine how many characters wide the display area has to be
//      to display the content.  For this class the # of characters
//      is specified when constructed
//  Parameters:
//      None
//  Returns:
//      width needed
//  Possible errors:
//      none
int PlotWindow::RequiredWidth() {
    return m_width;
}

//  Trigger the initial display of this window.  This includes
//      creating an ncurses WINDOW and doing an initial refresh on it
//  Parameters:
//      None
//  Returns:
//      true
//  Possible errors:
//      none
bool PlotWindow::Display() {
    m_pwindow = newwin(m_height, m_width, m_yULWindow, m_xULWindow);
    wrefresh(m_pwindow);
    return true;
}

//  Erase everything that is currently displayed.  This call must
//    eventually be followed by a call on the Refresh method.  The
//    typical call sequence for writing messages to a window is:
//          Erase
//          Write
//          Refresh
//    You get better performance batching calls before calling Refresh.
//  Parameters:
//      None
//  Returns:
//      nothing
//  Possible errors:
//      none
void PlotWindow::Erase() {
    wclear(m_pwindow);
}

//  Plot the character ch at position (x,y) with the specified
//      color and optional character attribute.  This call must eventually
//      be followed by a call on the Refresh method.  Typically
//      usage is:
//          Write
//          Write
//          Write
//          ...
//          Refresh
//      You get better performance by batching Write calls before
//      calling Refresh.
//  Parameters:
//      x - x-coordinate where to write the character
//      y - y-coordinate where to write the character
//      ch - character to write (may be extended character)
//      color - index of color pair to use
//      attrib - optional character attribute
//  Returns:
//      nothing
//  Possible errors:
//      none
void PlotWindow::Write(int x, int y, chtype ch, int color, int attrib) {
    int effectiveAttrib;

    effectiveAttrib = 0;
    if (attrib != A_NORMAL) {
        effectiveAttrib |= attrib;
    }
    if (color != DEFAULT_COLOR) {
        effectiveAttrib |= COLOR_PAIR(color);
    }
    if (effectiveAttrib != 0) {
        wattron(m_pwindow, effectiveAttrib);
    }
    mvwaddch(m_pwindow, y, x, ch);
    if (effectiveAttrib != 0) {
        wattroff(m_pwindow, effectiveAttrib);
    }
}

//  Write the text string horizontally starting at position (x,y) with the specified
//      color and optional character attribute.  This call must eventually
//      be followed by a call on the Refresh method.  Typically
//      usage is:
//          Erase
//          Write
//          Refresh
//      You get better performance by batching Write calls before
//      calling Refresh.
//  Parameters:
//      x - x-coordinate where to write the character
//      y - y-coordinate where to write the character
//      text - text string to write to window
//      color - index of color pair to use
//      attrib - optional character attribute
//  Returns:
//      nothing
//  Possible errors:
//      none
void PlotWindow::Write(int x, int y, const string& text, int color, int attrib) {
    int effectiveAttrib;

    effectiveAttrib = 0;
    if (attrib != A_NORMAL) {
        effectiveAttrib |= attrib;
    }
    if (color != DEFAULT_COLOR) {
        effectiveAttrib |= COLOR_PAIR(color);
    }
    if (effectiveAttrib != 0) {
        wattron(m_pwindow, effectiveAttrib);
    }
    mvwprintw(m_pwindow, y, x, text.c_str());
    if (effectiveAttrib != 0) {
        wattroff(m_pwindow, effectiveAttrib);
    }
}

//  Redraw the window including all changes to the WINDOW made since
//      the last Refresh call. You get better performance by batching
//      Write calls before calling Refresh.
//  Parameters:
//      none
//  Returns:
//      nothing
//  Possible errors:
//      none
void PlotWindow::Refresh() {
    wrefresh(m_pwindow);
}

//
//  Windowing class for getting input
//

//
//  Constructor
InputWindow::InputWindow(const string& name, int requiredWidth)
        : Content(name) {
    keypad(m_pwindow,true);
    m_requiredWidth = requiredWidth;
}

//  Get the line of input that the user types
//      First:
//          Erases the window
//          Positions the cursor at the upper left hand corner of the window
//          Refreshes the window
//      Then gets the characters as the user types them building a string from them
//      This function has the logic to handle the user typing BACKSPACE
//  Parameters:
//      none
//  Returns:
//      a string built from what the user types
//  Possible errors:
//      Does not handle other keys.  Returned string may contain unexpected characters
//      when use types special keys on the keyboard
string InputWindow::GetInput() {
    char ch;
    int pos;
    string input;

    werase(m_pwindow);
    wmove(m_pwindow, 0, 0);
    wrefresh(m_pwindow);
    pos = 0;
    while ((ch = wgetch(m_pwindow)) != '\n') {
        if (isprint(ch)) {
            input += ch;
            pos ++;
        }
        else {
            if ('\177' == ch) {    //Backspace
                input = input.substr(0, --pos);
            }
            wclear(m_pwindow);
            mvwprintw(m_pwindow, 0, 0, "%s", input.c_str());
            wrefresh(m_pwindow);
        }
    }
    wmove(m_pwindow, 0, pos);
    wrefresh(m_pwindow);
    return input;
}

//  Determine how many lines are required to display this window
//      For this class, this number is always 1 because it's a
//      single line high
//  Parameters:
//      None
//  Returns:
//      height needed, i.e. 1
//  Possible errors:
//      none
int InputWindow::RequiredHeight() {
    return 1;
}

//  Determine how many characters wide the display area has to be
//      to display the content.  For this class the # of characters
//      is specified when constructed
//  Parameters:
//      None
//  Returns:
//      width needed
//  Possible errors:
//      none
int InputWindow::RequiredWidth() {
    return m_requiredWidth;
}

//  Trigger the initial display of this window.  This includes
//      creating an ncurses WINDOW and doing an initial refresh on it
//  Parameters:
//      None
//  Returns:
//      true
//  Possible errors:
//      none
bool InputWindow::Display() {
    m_pwindow = newwin(1, m_requiredWidth, m_yULWindow, m_xULWindow);
    wrefresh(m_pwindow);
    return true;
}

//
//  Class that windowing classes used grouping windows
//      horizontally (HGroup) or vertically (VGroup)
//      are derived from
//
//      Supports displaying both a top horizontal
//      title and left vertical title
//

//
//  Constructor
Container::Container(const string& name,
                     bool hasBorder, const string& hTitle, const string& vTitle,
                     TextPosition hTitlePosition, TextPosition vTitlePosition,
                     int hTitleColor, int vTitleColor, int hTitleAttrib, int vTitleAttrib)
        : BaseWindow(name) {
    m_hasBorder = hasBorder;
    m_hTitleWidth = hTitle.length();
    m_vTitleHeight = vTitle.length();

    m_hTitle = hTitle;
    m_vTitle = vTitle;
    m_hTitlePosition = hTitlePosition;
    m_vTitlePosition = vTitlePosition;
    m_hTitleColor = hTitleColor;
    m_vTitleColor = vTitleColor;
    m_hTitleAttrib = hTitleAttrib;
    m_vTitleAttrib = vTitleAttrib;

    m_heightCached = false;
    m_widthCached = false;
}

//  Add a child window to the container class
//  Parameters:
//      child - pointer to a windowing class
//  Returns:
//      nothing
//  Possible errors:
//      none
void Container::AddChild(BaseWindow *child) {
    m_children.push_back(child);
}

//  Save the position of the container with respect to the main window
//      and then figure out offsets of the titles (m_xOffsetVTitle and
//      m_yOffsetHTitle) and offsets of child area (m_xLeftChildren and
//      m_yTopChildren).
//
//      Main program will determine its required height and window via
//      recursive calls.  Then it calls SetPosition which recurses down.
//      This will be called by HGroup or VGroup as part of this recursion.
//      After this Display will be called.
//  Parameters:
//      x - x coordinate of UL of container with respect to the parent window
//      y - y coordinate of UL of container with respect to the parent window
//  Returns:
//      nothing
//  Possible errors:
//      none
void Container::SetPosition(int x, int y) {
    int xOffset;
    int yOffset;

    // Save the coordinates of our position with respect to the parent window
    m_xULWindow = x;
    m_yULWindow = y;

    // Now find the offsets of the titles and the child area
    xOffset = 0;
    yOffset = 0;

    // Account for the border
    if (m_hasBorder) {
        xOffset ++;
        yOffset ++;
    }

    // Save the offset of the titles (if they exist)
    m_xOffsetVTitle = xOffset;
    m_yOffsetHTitle = yOffset;

    // Account for the presence of titles
    if (m_hTitleWidth > 0) {
        yOffset ++;
    }
    if (m_vTitleHeight > 0) {
        xOffset ++;
    }

    // Vertical title positioning
    if (HIGH == m_vTitlePosition) {
        m_yOffsetVTitle = yOffset;
    }
    else if (CENTER == m_vTitlePosition) {
        m_yOffsetVTitle = yOffset + (m_childrenHeight-m_vTitleHeight)/2;
    }
    else {
        m_yOffsetHTitle = yOffset + (m_childrenHeight - m_vTitleHeight);
    }
    // Horizontal title positioning
    if (LEFT == m_hTitlePosition) {
        m_xOffsetHTitle = xOffset;
    }
    else if (CENTER == m_hTitlePosition) {
        m_xOffsetHTitle = xOffset + (m_childrenWidth-m_hTitleWidth)/2;
    }
    else {
        m_xOffsetHTitle = xOffset + (m_childrenWidth - m_hTitleWidth);
    }

    // Save the coordinates of UL corner of child area (with respect to the parent window)
    m_xLeftChildren = xOffset;
    m_yTopChildren = yOffset;
}

//  Display the container and its child windows.  Renders a border and horizontal and
//      vertical titles if desired.  This function assumes that the required heights
//      and widths have already been computed and SetPosition has been called.
//  Parameters:
//      none
//  Returns:
//      true if no errors, false otherwise
//  Possible Errors:
//      unexpected, but theoretically it could return false if there was a problem
//      displaying a child window
bool Container::Display() {
    // Create window and display border if desired
    m_pwindow = newwin(m_windowHeight, m_windowWidth, m_yULWindow, m_xULWindow);
    if (m_hasBorder) {
        box(m_pwindow, ACS_VLINE, ACS_HLINE);
    }

    // Display horizontal title
    if (m_hTitleWidth > 0) {
        if (m_hTitleAttrib != A_NORMAL) {
            wattron(m_pwindow, m_hTitleAttrib);
        }
        if (m_hTitleColor != DEFAULT_COLOR) {
            wattron(m_pwindow, COLOR_PAIR(m_hTitleColor));
        }
        mvwprintw(m_pwindow, m_yOffsetHTitle, m_xOffsetHTitle, m_hTitle.c_str());
        if (m_hTitleColor != DEFAULT_COLOR) {
            wattroff(m_pwindow, COLOR_PAIR(m_hTitleColor));
        }
        if (m_hTitleAttrib != A_NORMAL) {
            wattroff(m_pwindow, m_hTitleAttrib);
        }
    }

    // Display vertical title
    if (m_vTitleHeight > 0) {
        if (m_vTitleAttrib != A_NORMAL) {
            wattron(m_pwindow, m_vTitleAttrib);
        }
        if (m_vTitleColor != DEFAULT_COLOR) {
            wattron(m_pwindow, COLOR_PAIR(m_vTitleColor));
        }
        for (int ich = 0; ich < m_vTitleHeight; ich ++) {
            mvwprintw(m_pwindow, m_yOffsetVTitle+ich, m_xOffsetVTitle, "%c", m_vTitle[ich]);
        }
        if (m_vTitleColor != DEFAULT_COLOR) {
            wattroff(m_pwindow, COLOR_PAIR(m_vTitleColor));
        }
        if (m_vTitleAttrib != A_NORMAL) {
            wattroff(m_pwindow, m_vTitleAttrib);
        }
    }

    wrefresh(m_pwindow);

    // Show my children
    for (int i = 0; i < m_children.size(); i ++) {
        if (!m_children[i]->Display()) {
            return false;
        }
    }
    return true;
}

//
//  Container class used to arrange subwindows horizontally
//

//
//  Constructor
HGroup::HGroup(const string& name,
               bool hasBorder, const string& hTitle, const string& vTitle,
               TextPosition hTitlePosition, TextPosition vTitlePosition,
               int hTitleColor, int vTitleColor,
               int hTitleAttrib, int vTitleAttrib)
        : Container(name, hasBorder, hTitle, vTitle, hTitlePosition, vTitlePosition,
                    hTitleColor, vTitleColor, hTitleAttrib, vTitleAttrib) {
}

//  Calculate and return the height (number of lines) needed to display this HGroup
//      Using recursion this routine finds the maximum of the heights of the subwindows
//      and caches it in the member variable m_childrenHeight.  Then it increments this
//      quantity for any extra height needed for a border and/or horizontal title (if specified)
//      This quantity is cached as m_windowHeight.
//  Parameters:
//      none
//  Returns:
//      m_windowHeight
//  Possible Errors:
//      none
int HGroup::RequiredHeight() {
    if (!m_heightCached) {
        int height;

        // Find maximum height of the children
        height = m_vTitleHeight;
        for (int i = 0; i < m_children.size(); i++) {
            int childHeight;

            childHeight = m_children[i]->RequiredHeight();
            if (childHeight > height) {
                height = childHeight;
            }
        }

        // Save the height
        m_childrenHeight = height;

        // Add on the frame if there is one
        if (m_hasBorder) {
            height += 2;
        }
        // Add in the title line if it appears
        if (m_hTitleWidth > 0) {
            height += 1;
        }
        m_windowHeight = height;
        m_heightCached = true;
    }
    return m_windowHeight;
}

//  Calculate and return the width (number of characters) needed to display this HGroup
//      Using recursion this routine finds the the widths of the subwindows and caches
//      their total in the member variable m_childrenWidth.  Then it increments this
//      quantity for any extra width needed for a border and/or a vertical title (if specified)
//      This quantity is cached as m_windowWidth
//  Parameters:
//      none
//  Returns:
//      m_windowWidth
//  Possible Errors:
//      none
int HGroup::RequiredWidth() {
    if (!m_widthCached) {
        int width;

        // Sum up the widths across
        width = 0;
        for (int i = 0; i < m_children.size(); i++) {
            width += m_children[i]->RequiredWidth();
        }

        // If the title is wider, use that width
        if (m_hTitleWidth > width) {
            width = m_hTitleWidth;
        }
        m_childrenWidth = width;

        // If there is a vertical title, count it
        if (m_vTitleHeight > 0) {
            width ++;
        }

        // If there is a border, count it
        if (m_hasBorder) {
            width += 2;
        }
        m_windowWidth = width;
        m_widthCached = true;
    }
    return m_windowWidth;
}

//  This routine first calls Container::SetPosition to save the position of the container
//      with respect to the main window, and figure out the offsets of the titles and the
//      child area.  Then it recurses to call SetPosition on its subwindows
//
//      Main program will determine its required height and window via
//      recursive calls.  Then it calls SetPosition recursively.  After this
//      After this Display will be called.
//  Parameters:
//      x - x coordinate of UL of container with respect to the parent window
//      y - y coordinate of UL of container with respect to the parent window
//  Returns:
//      nothing
//  Possible errors:
//      none
void HGroup::SetPosition(int x, int y) {
    int xChild;

    // Figure out the coordinates of the child area
    Container::SetPosition(x, y);

    // Now tell the children where they are
    xChild = m_xLeftChildren + m_xULWindow;
    for (int i = 0; i < m_children.size(); i ++) {
        m_children[i]->SetPosition(xChild, m_yTopChildren + m_yULWindow);
        xChild += m_children[i]->RequiredWidth();
    }
}

//
//  Container class used to stack subwindows vertically
//

//
//  Constructor
VGroup::VGroup(const string& name,
               bool hasBorder, const string& hTitle, const string& vTitle,
               TextPosition hTitlePosition, TextPosition vTitlePosition,
               int hTitleColor, int vTitleColor,
               int hTitleAttrib, int vTitleAttrib)
        : Container(name, hasBorder, hTitle, vTitle, hTitlePosition, vTitlePosition,
                    hTitleColor, vTitleColor, hTitleAttrib, vTitleAttrib) {
}

//  Calculate and return the height (number of lines) needed to display this VGroup
//      Using recursion this routine finds and sums the heights of the subwindows.  The
//      total is cached in the member variable m_childrenHeight.  Then it increments this
//      quantity for any extra height needed for a border and/or horizontal title (if specified)
//      This quantity is cached as m_windowHeight.
//  Parameters:
//      none
//  Returns:
//      m_windowHeight
//  Possible Errors:
//      none
int VGroup::RequiredHeight() {
    if (!m_heightCached) {
        int height;

        height = 0;

        // Find the height of the child area
        for (int i = 0; i < m_children.size(); i++) {
            height += m_children[i]->RequiredHeight();
        }
        if (m_vTitleHeight > height) {
            height = m_vTitleHeight;
        }
        m_childrenHeight = height;

        // Save room for the border
        if (m_hasBorder) {
            height += 2;
        }

        // If there is a title count that
        if (m_hTitleWidth > 0) {
            height += 1;
        }
        m_windowHeight = height;
        m_heightCached = true;
    }
    return m_windowHeight;
}

//  Calculate and return the width (number of characters) needed to display this VGroup
//      Using recursion this routine finds the the widths of the subwindows.  The maximum
//      of these widths is cached in the member variable m_childrenWidth.  Then it increments this
//      quantity for any extra width needed for a border and/or a vertical title (if specified)
//      This quantity is cached as m_windowWidth
//  Parameters:
//      none
//  Returns:
//      m_windowWidth
//  Possible Errors:
//      none
int VGroup::RequiredWidth() {
    if (!m_widthCached) {
        int width;

        // Start with the title width
        width = m_hTitleWidth;

        // Now see if any children are wider
        for (int i = 0; i < m_children.size(); i++) {
            int childWidth;

            childWidth = m_children[i]->RequiredWidth();
            if (childWidth > width) {
                width = childWidth;
            }
        }
        m_childrenWidth = width;

        // Add space for vertical title if set
        if (m_vTitleHeight > 0) {
            width ++;
        }
        // Add the border width
        if (m_hasBorder) {
            width += 2;
        }
        m_windowWidth = width;
        m_widthCached = true;
    }
    return m_windowWidth;
}

//  This routine first calls Container::SetPosition to save the position of the container
//      with respect to the main window, and figure out the offsets of the titles and the
//      child area.  Then it recurses to call SetPosition on its subwindows
//
//      Main program will determine its required height and window via
//      recursive calls.  Then it calls SetPosition recursively.  After this
//      After this Display will be called.
//  Parameters:
//      x - x coordinate of UL of container with respect to the parent window
//      y - y coordinate of UL of container with respect to the parent window
//  Returns:
//      nothing
//  Possible errors:
//      none
void VGroup::SetPosition(int x, int y) {
    int yChild;

    // Figure out the coordinates of the child area
    Container::SetPosition(x, y);

    // Now tell the children where they are
    yChild = m_yTopChildren + m_yULWindow;
    for (int i = 0; i < m_children.size(); i ++) {
        m_children[i]->SetPosition(m_xLeftChildren + m_xULWindow, yChild);
        yChild += m_children[i]->RequiredHeight();
    }
}

//
//  Class representing the main window of a program.
//      Consists of sub-windows stacked vertically.
//

//
//  Constructor
MainWindow::MainWindow(bool hasBorder, const string& hTitle, const string& vTitle,
                       TextPosition hTitlePosition, TextPosition vTitlePosition,
                       int hTitleColor, int vTitleColor,
                       int hTitleAttrib, int vTitleAttrib)
        : VGroup("MainWindow", hasBorder, hTitle, vTitle, hTitlePosition, vTitlePosition,
                 hTitleColor, vTitleColor, hTitleAttrib, vTitleAttrib) {

}

//
//  Destructor
MainWindow::~MainWindow() {
    endwin();
}

//  Displays the main window and its subwindows
//      Follows the sequence
//          Initialize ncurses including creating the color pairs
//          Find required height and width
//          Set positions of the subwindows
//          Create the WINDOWs and display
//  Parameters:
//       fgColors - array of foreground colors
//       bgColors - array of background colors
//       numberColorPairs - number of elements in each of the above arrays
//  Returns:
//       success/failure
//  Possible errors:
//       none expected
bool MainWindow::Display(int fgColors[], int bgColors[], int numberColorPairs) {
    int height;
    int width;
    int heightAvail;
    int widthAvail;
    int line;

    initscr();
    refresh();      // a refresh before creating subwindows appears to be necessary
    start_color();
    for (int i = 0; i < numberColorPairs; i ++) {
        init_pair(i+1,fgColors[i], bgColors[i]);
    }

    // Find how big this window must be
    height = RequiredHeight();
    width = RequiredWidth();

    // Check with ncurses to see how much room we have
    //getmaxyx(stdscr, heightAvail, widthAvail);
    //if (height > heightAvail || width > widthAvail) {
    //    cerr << "Initialization failed, try resizing the command window to size ";
    //    cerr << height << " rows by " << width << " columns." << endl;
    //    return false;
    //}

    // Now tell the subwindows where they start
    SetPosition(0,0);

    // Display the containers
    return Container::Display();
}


