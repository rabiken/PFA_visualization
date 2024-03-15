#include "maze.h"
#include "player.h"
#include <iostream>
#include <chrono>
#include <thread>

using Cost = int;
using Heur = int;
// position and cost
struct PosCosHeur 
{
    Position _pos;
    Cost _cost;
    Heur _heur;
    PosCosHeur(const Position& pos, Cost cost, Heur heur) 
    : _pos(pos)
    , _cost(cost)
    , _heur(heur)
    {}
    bool operator > (const PosCosHeur& other) const {
        return _cost + _heur > other._cost + other._heur;
    }
};
class CAstar: public CPlayer 
{
    std::priority_queue
    <PosCosHeur, std::vector<PosCosHeur>, std::greater<PosCosHeur> > _pq;
    Cost _cost = 0;
    Position _end;
private: 
    int calcCost() {
        return _cost + 1;
    }
    int calcHeur(const Position& next) {
        return abs(_end.first - next.first) + abs(_end.second - next.second);
    }
public:
    CAstar(Position pos, Position end)
    : CPlayer(std::move(pos))
    , _cost(0)
    , _end(std::move(end))
    {
        _pq.push(PosCosHeur(pos, 0, calcHeur(pos)));   // Enqueue the initial position
        _pred[pos] = ROOT;
    }

    void move(CMaze& maze) override {
        CPlayer::move(maze);
        _pos = _pq.top()._pos;
        _cost = _pq.top()._cost;
        _pq.pop();
        maze[_pos] = Cell::PLAYER;
    }

    void enqueueNext(CMaze& maze) {
        if ( available(UP, maze) ) {
            _pq.push(PosCosHeur(UP, calcCost(), calcHeur(UP)));
            _pred[UP] = _pos;
            maze[UP] = Cell::OPENED;
        }
        if ( available(RIGHT, maze) ) {
            _pq.push(PosCosHeur(RIGHT, calcCost(), calcHeur(RIGHT)));
            _pred[RIGHT] = _pos;
            maze[RIGHT] = Cell::OPENED;
        }
        if ( available(DOWN, maze) ) {
            _pq.push(PosCosHeur(DOWN, calcCost(), calcHeur(DOWN)));
            _pred[DOWN] = _pos;
            maze[DOWN] = Cell::OPENED;
        }
        if ( available(LEFT, maze) ) {   
            _pq.push(PosCosHeur(LEFT, calcCost(), calcHeur(LEFT)));
            _pred[LEFT] = _pos;
            maze[LEFT] = Cell::OPENED;
        }
    }
    bool queueEmpty() const { return _pq.empty(); }
};
// Shared pointer of the class CAstar
using AstarSP = std::shared_ptr<CAstar>;

int main (int argv, char* argc[]) {
    if ( argv != 2 ) {
        printf ("%s: Type the name of an input file.\n", argc[0]);
        return 1;
    }
    CMaze maze;
    if ( ! maze.read(argc[1]) ) {
        return 1;
    }
    AstarSP player(new CAstar(maze.getStart(), maze.getEnd()));
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
