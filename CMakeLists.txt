cmake_minimum_required(VERSION 3.10)
project(Battleship)

set(CMAKE_CXX_STANDARD 14)

add_executable(Battleship main.cpp grid.cpp grid.h cpulogic.cpp cpulogic.h cursesWindow.cpp cursesWindow.h gameBoard.cpp gameBoard.h gridWindow.cpp gridWindow.h commandWindow.cpp commandWindow.h battleship.h)
target_link_libraries(Battleship ncurses)
