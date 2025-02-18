#include "../maze.h"

constexpr const char* FILENAME =  "dataset/6.txt";
int main () {
    CMaze maze;
    maze.read(FILENAME);
    maze.print();
    return 0;
}
