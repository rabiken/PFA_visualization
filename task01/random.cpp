#include "maze.h"
#include "player.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

class CRandom: public CPlayer 
{
    std::deque<Position> dq;
public:
    CRandom(Position pos): CPlayer(std::move(pos)) 
    {
        dq.push_back(pos);   // Enqueue the initial position
        _pred[pos] = ROOT;
    }

    void move(CMaze& maze) override {
        CPlayer::move(maze);
        int idx = rand() % dq.size();
        std::swap(dq.front(), dq[idx]);
        _pos = dq.front();
        dq.pop_front();
        maze[_pos] = Cell::PLAYER;
    }

    void enqueueNext(CMaze& maze) {
        if ( available(UP, maze) ) {
            dq.push_back(UP);
            _pred[UP] = _pos;
            maze[UP] = Cell::OPENED;
        }
        if ( available(RIGHT, maze) ) {
            dq.push_back(RIGHT);
            _pred[RIGHT] = _pos;
            maze[RIGHT] = Cell::OPENED;
        }
        if ( available(DOWN, maze) ) {
            dq.push_back(DOWN);
            _pred[DOWN] = _pos;
            maze[DOWN] = Cell::OPENED;
        }
        if ( available(LEFT, maze) ) {   
            dq.push_back(LEFT);
            _pred[LEFT] = _pos;
            maze[LEFT] = Cell::OPENED;
        }
    }
    bool queueEmpty() const { return dq.empty(); }
};
// Shared pointer of the class CRandom
using RanodmSP = std::shared_ptr<CRandom>;

int main (int argv, char* argc[]) {
    if ( argv != 2 ) {
        printf ("%s: Type the name of an input file.\n", argc[0]);
        return 1;
    }
    CMaze maze;
    if ( ! maze.read(argc[1]) ) {
        return 1;
    }
    RanodmSP player(new CRandom(maze.getStart()));
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
