#include "maze.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <thread>
#include "names.h"

int CMaze::addChars(const std::string& str) {
    for ( char x : str ) {
        _cells.push_back(x);
    }
    return str.length();
}
bool CMaze::read(const char* filename) {
    std::ifstream file(filename);
    if ( ! file.is_open() ) {
        std::cout << "The file couldn't be opened." << std::endl;
        return false;
    }
    
    std::string line;
    std::getline(file, line);
    _width = addChars(line);
    _height ++;
    while ( std::getline(file, line) ) {
        if ( line[0] == Cell::WALL ) 
        {
            if ( _width != addChars(line) )
                return false;
        } else {
            break;
        }
        _height ++;
    }

    int x, y;
    if ( sscanf(line.c_str(), "start %d , %d", &x, &y ) != 2 ) {
    printf("here\n");
        return false;
    } 
    _start = {x, y};
    operator()(x,y) = Cell::START;
    std::getline(file, line);
    if ( sscanf(line.c_str(), "end %d , %d", &x, &y ) != 2 ) {
        return false;
    }
    _end = {x, y};
    operator()(x,y) = Cell::END;
    
    file.close();
    return true;
}

void CMaze::print() const {
    for ( int y = 0; y < _height; y ++ ) {
        for ( int x = 0; x < _width; x ++ ) {
            char cell = operator()(x,y);
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::cout << LABELS;
}
void CMaze::printNoPath() const {
    for ( int y = 0; y < _height; y ++ ) {
        for ( int x = 0; x < _width; x ++ ) {
            char cell = operator()(x,y);
            if ( cell == Cell::PATH ) {
                std::cout << Cell::EMPTY;
            } else {
                std::cout << cell;
            }
        }
        std::cout << std::endl;
    }
    std::cout << LABELS;
}
char& CMaze::operator()(int x, int y)
{
    if ( x < 0 || y < 0 || x >= _width || y >= _height ) {
        std::cout << "(x,y) = (" << x << ", " << y << ")" << std::endl;
        std::cout << "(w,h) = (" << _width << ", " << _height << ")" << std::endl;
        throw std::out_of_range("out of range");
    }
    return _cells[x + y * _width];
}
char CMaze::operator()(int x, int y) const
{
    if ( x < 0 || y < 0 || x >= _width || y >= _height ) {
        std::cout << "(x,y) = (" << x << ", " << y << ")" << std::endl;
        std::cout << "(w,h) = (" << _width << ", " << _height << ")" << std::endl;
        throw std::out_of_range("out of range");
    }
    return _cells[x + y * _width];
}
char& CMaze::operator[](const Position& pos)
{
    return operator()(pos.first, pos.second);
}
char CMaze::operator[](const Position& pos) const
{
    return operator()(pos.first, pos.second);
}
bool CMaze::isInRange(const Position& pos) const {
    return pos.first >= 0 && pos.second >= 0 
        && pos.first < _width && pos.second < _height;
}

char CMaze::getCellStatus(const Position& pos) const
{
    return operator()(pos.first, pos.second);
}

bool CMaze::isStart(Position pos) const
{
    return pos == _start;
}
bool CMaze::isEnd(Position pos) const
{
    return pos == _end;
}
Position CMaze::getStart() const
{ return _start; }
Position CMaze::getEnd() const
{ return _end; }

void flashScreen() {
    std::cout << "\033[2J\033[1;1H";
}
void updateScreen(const CMaze& maze) {
    flashScreen();
    maze.print();
}
void blinkResult(const CMaze& maze) {
    for ( int i = 0; i < 33; i ++ ) {
        flashScreen();
        if ( i % 2 == 0 ) {
            maze.print();
            std::cout << "\nThe path is found!!!\n" << std::endl;
        } else {
            maze.printNoPath();
            std::cout << "\n\nThe path is found!!!" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(MILLISEC));
    }
}
