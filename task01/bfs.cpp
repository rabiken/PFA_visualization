#include "maze.h"
#include "player.h"
#include <iostream>
#include <chrono>
#include <thread>

class CBfs: public CPlayer 
{
    std::queue<Position> _q;
public:
    CBfs(Position pos): CPlayer(std::move(pos)) 
    {
        _q.push(pos);   // Enqueue the initial position
        _pred[pos] = ROOT;
    }

    void move(CMaze& maze) override {
        CPlayer::move(maze);
        _pos = _q.front();
        _q.pop();
        maze[_pos] = Cell::PLAYER;
    }

    void enqueueNext(CMaze& maze) {
        if ( available(UP, maze) ) {
            _q.push(UP);
            _pred[UP] = _pos;
            maze[UP] = Cell::OPENED;
        }
        if ( available(RIGHT, maze) ) {
            _q.push(RIGHT);
            _pred[RIGHT] = _pos;
            maze[RIGHT] = Cell::OPENED;
        }
        if ( available(DOWN, maze) ) {
            _q.push(DOWN);
            _pred[DOWN] = _pos;
            maze[DOWN] = Cell::OPENED;
        }
        if ( available(LEFT, maze) ) {   
            _q.push(LEFT);
            _pred[LEFT] = _pos;
            maze[LEFT] = Cell::OPENED;
        }
    }
    bool queueEmpty() const { return _q.empty(); }
};
// Shared pointer of the class CBfs
using BfsSP = std::shared_ptr<CBfs>;

int main (int argv, char* argc[]) {
    if ( argv != 2 ) {
        printf ("%s: Type the name of an input file.\n", argc[0]);
        return 1;
    }
    CMaze maze;
    if ( ! maze.read(argc[1]) ) {
        return 1;
    }
    BfsSP player(new CBfs(maze.getStart()));
    int step = 0;
    while ( ! player->queueEmpty() ) {
        player->move(maze);
        updateScreen(maze);
        std::cout << "step: " << step << std::endl;
        // Wait for 1 second
        std::this_thread::sleep_for(std::chrono::milliseconds(MILLISEC));
        if ( maze.isEnd(player->getPos()) ) {
            maze[player->getPos()] = Cell::END;
            step ++;
            break;
        }
        player->enqueueNext(maze);
        step ++;
    }
    updateScreen(maze);
    std::cout << "step: " << step << std::endl;
    int pathLen = player->drawPath(maze);
    blinkResult(maze);
    std::cout << "step: " << step << std::endl;
    std::cout << "Path length: " << pathLen << std::endl;
    return 0;
}
