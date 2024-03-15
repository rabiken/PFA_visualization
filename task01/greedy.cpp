#include "maze.h"
#include "player.h"
#include <iostream>
#include <chrono>
#include <thread>

using Cost = int;
// position and cost
struct PosCos : public std::pair<Position, Cost> 
{
    PosCos(const Position& pos, Cost cost) : std::pair<Position, Cost>(pos,cost) {}
    bool operator > (const PosCos& other) const {
        return this->second > other.second;
    }
};
class CGreedy: public CPlayer 
{
    std::priority_queue<PosCos, std::vector<PosCos>, std::greater<PosCos> > _pq;
    Cost _cost = 0;
private:
    int calcCost() {
        return _cost + 1;
    }
public:
    CGreedy(Position pos)
    : CPlayer(std::move(pos)) 
    , _cost(0)
    {
        _pq.push(PosCos(pos, 0));   // Enqueue the initial position
        _pred[pos] = ROOT;
    }

    void move(CMaze& maze) override {
        CPlayer::move(maze);
        _pos = _pq.top().first;
        _cost = _pq.top().second;
        _pq.pop();
        maze[_pos] = Cell::PLAYER;
    }

    void enqueueNext(CMaze& maze) {
        if ( available(UP, maze) ) {
            _pq.push(PosCos(UP, calcCost()));
            _pred[UP] = _pos;
            maze[UP] = Cell::OPENED;
        }
        if ( available(RIGHT, maze) ) {
            _pq.push(PosCos(RIGHT, calcCost()));
            _pred[RIGHT] = _pos;
            maze[RIGHT] = Cell::OPENED;
        }
        if ( available(DOWN, maze) ) {
            _pq.push(PosCos(DOWN, calcCost()));
            _pred[DOWN] = _pos;
            maze[DOWN] = Cell::OPENED;
        }
        if ( available(LEFT, maze) ) {   
            _pq.push(PosCos(LEFT, calcCost()));
            _pred[LEFT] = _pos;
            maze[LEFT] = Cell::OPENED;
        }
    }
    bool queueEmpty() const { return _pq.empty(); }
};
// Shared pointer of the class CGreedy
using GreedySP = std::shared_ptr<CGreedy>;

int main (int argv, char* argc[]) {
    if ( argv != 2 ) {
        printf ("%s: Type the name of an input file.\n", argc[0]);
        return 1;
    }
    CMaze maze;
    if ( ! maze.read(argc[1]) ) {
        return 1;
    }
    GreedySP player(new CGreedy(maze.getStart()));
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
