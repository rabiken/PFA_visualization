#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include "names.h"
#include "maze.h"

using Position = std::pair<int, int>;

#define UP {_pos.first,_pos.second - 1}
#define RIGHT {_pos.first + 1,_pos.second}
#define DOWN {_pos.first, _pos.second + 1}
#define LEFT {_pos.first - 1, _pos.second}

class CPlayer {
protected:
    Position _pos;
    std::map<Position, Position> _pred; // predecessors({opened, from where})
public: 
    bool available(Position pos, const CMaze& maze) const {
        return maze[pos] != Cell::WALL && _pred.count(pos) == 0;
    }
    CPlayer(Position pos): _pos(std::move(pos)) {}
    virtual ~CPlayer() = default;
    void virtual setPos(const Position& pos) { _pos = pos; }
    Position getPos() const { return _pos; }
    /**
     * @brief Update Player's position and the state of the cell in the maze.
     * 
     * @param maze 
     * @return Position 
     */
    virtual void move(CMaze& maze) {
        if ( maze.isStart(_pos) ) {
            maze[_pos] = Cell::START;
        }
        else if ( maze.isEnd(_pos) ) {
            maze[_pos] = Cell::END;
        } else {
            maze[_pos] = Cell::CLOSED;
        }
    }
    int drawPath(CMaze& maze) {
        Position pos = maze.getEnd();
        pos = _pred[pos];
        int cnt = 0;
        while ( pos != ROOT && !maze.isStart(pos) ) {
            maze[pos] = Cell::PATH;
            pos = _pred[pos];
            cnt++;
        }
        return cnt;
    }
};

using APlayer = std::shared_ptr<CPlayer>;

#endif
