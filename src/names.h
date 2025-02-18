#ifndef NAMES_H
#define NAMES_H

using Position = std::pair<int, int>;
constexpr int MILLISEC = 100;
constexpr Position ROOT = {-1, -1};

constexpr const char* LABELS = 
"--------------\n\
'S' Start\n\
'E' End\n\
'?' Opened node\n\
'#' Closed node\n\
'o' Path\n\
'X' Wall\n\
'.' Current node\n\
' ' Fresh node\n\
--------------\n";

namespace Cell {
    const char WALL = 'X';
    const char PLAYER = '.';
    const char START = 'S';
    const char END = 'E';
    const char OPENED = '?';
    const char CLOSED = '#';
    const char EMPTY = ' ';
    const char PATH = 'o';
};

#endif
