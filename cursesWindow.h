// Title: cursesWindow.h
//
// Purpose: Declares a set of  C++ classes that wrap the
//          ncurses library.
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson

// Note: to get curse.h and the associated library
// on Ubuntu execute sudo apt-get install libncurses5-dev

#ifndef BATTLESHIP_CURSESWINDOW_H
#define BATTLESHIP_CURSESWINDOW_H

#include <fstream>
#include <vector>
#include <curses.h>

using namespace std;

const int DEFAULT_COLOR = 0;
enum TextPosition { LEFT, HIGH, CENTER, RIGHT, LOW };

//
//  Base windowing class for all the other windowing classes
//      Holds the ncurses WINDOW handle and
//      coordinates of upper left hand corner
//      of the WINDOW with respect to its parent
//
//  Tree of subclasses derived from BaseWindow:
//
//                          BaseWindow
//                     /                    \
//               Content                    Container
//              /       \                 /          \
//  InputWindow     PlotWindow        HGroup        VGroup
//                                                    |
//                                                MainWindow
//
class BaseWindow {
    friend class Container;
    friend class HGroup;
    friend class VGroup;

public:
    BaseWindow(const string& name);
    ~BaseWindow();

protected:
    virtual int RequiredHeight() = 0;
    virtual int RequiredWidth() = 0;
    virtual void SetPosition(int x, int y) = 0;
    virtual bool Display() = 0;

    int  m_xULWindow;               //Init by SetPosition
    int  m_yULWindow;               //Init by SetPosition
    WINDOW *m_pwindow;
    string m_name;
};

//
//  Class that windowing classes used for plotting or getting
//      input are derived from.
//
class Content : public BaseWindow {
public:
    Content(const string& name);

protected:
    void SetPosition(int x, int y) override ;
};

//
//  Windowing class for writing text at a particular position
//
class PlotWindow : public Content {
public:
    PlotWindow(const string& name, int height, int width);

    void Erase();
    void Write(int x, int y, chtype ch, int color = DEFAULT_COLOR, int attrib = A_NORMAL);
    void Write(int x, int y, const string& text, int color = DEFAULT_COLOR, int attrib = A_NORMAL);
    void Refresh();

protected:
    int RequiredHeight() override;
    int RequiredWidth() override;
    bool Display() override;

private:
    int m_height;
    int m_width;
};

//
//  Windowing class for getting input
//
class InputWindow : public Content {
public:
    InputWindow(const string& name, int requiredWidth);

    string GetInput();

protected:
    int RequiredHeight() override;
    int RequiredWidth() override;
    bool Display() override;

private:
    int m_requiredWidth;

    string m_template;
};

//
//  Class that windowing classes used grouping windows
//      horizontally (HGroup) or vertically (VGroup)
//      are derived from
//
//      Supports displaying both a top horizontal
//      title and left vertical title
//
class Container : public BaseWindow {
    friend class MainWindow;
public:
    Container(const string &name,
              bool hasBorder=false, const string& hTitle="", const string& vTitle="",
              TextPosition htitlePosition = CENTER, TextPosition vTitlePosition = CENTER,
              int hTitleColor=DEFAULT_COLOR, int vTitleColor=DEFAULT_COLOR,
              int hTitleAttrib = A_NORMAL, int vTitleAttrib = A_NORMAL );

    void AddChild(BaseWindow *child);
    bool Display() override;

protected:
    void SetPosition(int x, int y) override;

    // Init by RequiredHeight
    bool m_heightCached;
    int  m_windowHeight;
    int  m_childrenHeight;
    // Init by RequiredWidth
    bool m_widthCached;
    int  m_windowWidth;
    int  m_childrenWidth;

    bool m_hasBorder;               //Init by constructor
    int  m_hTitleWidth;             //Init by constructor
    int  m_vTitleHeight;            //Init by constructor
    int  m_xLeftChildren;           //Init by SetPosition
    int  m_yTopChildren;            //Init by SetPosition
    vector<BaseWindow *> m_children;

private:
    int  m_xOffsetHTitle;            //Init by SetPosition
    int  m_yOffsetHTitle;            //Init by SetPosition
    int  m_xOffsetVTitle;            //Init by SetPosition
    int  m_yOffsetVTitle;            //Init by SetPosition
    string m_hTitle;                 //Init by constructor
    string m_vTitle;                 //Init by constructor
    TextPosition m_hTitlePosition;   //Init by constructor
    TextPosition m_vTitlePosition;   //Init by constructor
    int m_hTitleColor;               //Init by constructor
    int m_vTitleColor;               //Init by constructor
    int m_hTitleAttrib;              //Init by constructor
    int m_vTitleAttrib;              //Init by constructor
};

//
//  Container class used to arrange subwindows horizontally
//
class HGroup : public Container {
public:
    HGroup(const string& name,
           bool hasBorder=false, const string& hTitle="", const string& vTitle="",
           TextPosition htitlePosition = CENTER, TextPosition vTitlePosition = CENTER,
           int hTitleColor=DEFAULT_COLOR, int vTitleColor=DEFAULT_COLOR,
           int hTitleAttrib = A_NORMAL, int vTitleAttrib = A_NORMAL );

protected:
    int RequiredHeight() override;
    int RequiredWidth() override;
    void SetPosition(int x, int y) override;
};

//
//  Container class used to stack windows vertically.
//
class VGroup : public Container {
public:
    VGroup(const string& name,
           bool hasBorder=false, const string& hTitle="", const string& vTitle="",
           TextPosition htitlePosition = CENTER, TextPosition vTitlePosition = CENTER,
           int hTitleColor=DEFAULT_COLOR, int vTitleColor=DEFAULT_COLOR,
           int hTitleAttrib = A_NORMAL, int vTitleAttrib = A_NORMAL );

protected:
    int RequiredHeight() override;
    int RequiredWidth() override;
    void SetPosition(int x, int y) override;
};

//
//  Class representing the main window of a program.
//      Consists of sub-windows stacked vertically.
//
class MainWindow : public VGroup {
public:
    MainWindow(bool hasBorder=false, const string& hTitle="", const string& vTitle="",
               TextPosition htitlePosition = CENTER, TextPosition vTitlePosition = CENTER,
               int hTitleColor=DEFAULT_COLOR, int vTitleColor=DEFAULT_COLOR,
               int hTitleAttrib = A_NORMAL, int vTitleAttrib = A_NORMAL );

    ~MainWindow();

    bool Display(int fgColors[], int bgColors[], int numberColorPairs);
};

#endif //BATTLESHIP_CURSESWINDOW_H

