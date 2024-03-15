#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <chrono>

using Position = std::pair<int, int>;
class CMaze { 
    int _height = 0;
    int _width = 0;
    Position _start;
    Position _end;
    std::vector<char> _cells;
    
    int addChars(const std::string& str); 
public:
    bool isStart(Position pos) const;
    bool isEnd(Position pos) const;
    Position getStart() const;
    Position getEnd() const;
    bool read(const char* filename);
    void print() const;
    void printNoPath() const;
    bool isInRange(const Position& pos) const;
    char getCellStatus(const Position& pos) const;
    char& operator()(int x, int y);
    char operator()(int x, int y) const;
    char& operator[](const Position& pos);
    char operator[](const Position& pos) const;
};

void flashScreen();
void updateScreen(const CMaze& maze);
void blinkResult(const CMaze& maze);

#endif
