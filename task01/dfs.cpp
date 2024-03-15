#include "maze.h"
#include "player.h"
#include <iostream>
#include <chrono>
#include <thread>

class CDfs: public CPlayer 
{
    std::stack<Position> _s;
public:
    CDfs(Position pos): CPlayer(std::move(pos)) 
    {
        _s.push(pos);   // Enqueue the initial position
        _pred[pos] = ROOT;
    }

    void move(CMaze& maze) override {
        CPlayer::move(maze);
        _pos = _s.top();
        _s.pop();
        maze[_pos] = Cell::PLAYER;
    }

    void enqueueNext(CMaze& maze) {
        if ( available(UP, maze) ) {
            _s.push(UP);
            _pred[UP] = _pos;
            maze[UP] = Cell::OPENED;
        }
        if ( available(RIGHT, maze) ) {
            _s.push(RIGHT);
            _pred[RIGHT] = _pos;
            maze[RIGHT] = Cell::OPENED;
        }
        if ( available(DOWN, maze) ) {
            _s.push(DOWN);
            _pred[DOWN] = _pos;
            maze[DOWN] = Cell::OPENED;
        }
        if ( available(LEFT, maze) ) {   
            _s.push(LEFT);
            _pred[LEFT] = _pos;
            maze[LEFT] = Cell::OPENED;
        }
    }
    bool queueEmpty() const { return _s.empty(); }
};
// Shared pointer of the class CDfs
using DfsSP = std::shared_ptr<CDfs>;

int main (int argv, char* argc[]) {
    if ( argv != 2 ) {
        printf ("%s: Type the name of an input file.\n", argc[0]);
        return 1;
    }
    CMaze maze;
    if ( ! maze.read(argc[1]) ) {
        return 1;
    }
    DfsSP player(new CDfs(maze.getStart()));
    bool found = false;
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
            found = true;
            break;
        }
        player->enqueueNext(maze);
        step ++;
    }
    if ( ! found ) {
        std::cout << "Path was not found." << std::endl;
    }
    updateScreen(maze);
    std::cout << "step: " << step << std::endl;
    int pathLen = player->drawPath(maze);
    blinkResult(maze);
    std::cout << "step: " << step << std::endl;
    std::cout << "Path length: " << pathLen << std::endl;
    return 0;
}
