# compiler
CXX = g++
# flags
CXXFLAGS = -std=c++17 -Wall -Wextra -fsanitize=address -g

TARGETS = random dfs bfs greedy a_star
OBJS = $(TARGETS).o

all: $(TARGETS)

random: random.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

dfs: dfs.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bfs: bfs.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

greedy: greedy.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

a_star: a_star.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

maze.o: maze.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# unit test
test_maze: $(MAZEOBJ) utest/test_maze.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(TARGETS) $(OBJS) test_maze *dSYM
