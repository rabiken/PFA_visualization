# compiler
CXX = g++
# flags
CXXFLAGS = -std=c++17 -Wall -Wextra -fsanitize=address -g
SRC = src

TARGETS = random dfs bfs greedy a_star
OBJS = $(TARGETS).o maze.o

all: $(TARGETS)

random: $(SRC)/random.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

dfs: $(SRC)/dfs.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bfs: $(SRC)/bfs.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

greedy: $(SRC)/greedy.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

a_star: $(SRC)/a_star.cpp maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

maze.o: $(SRC)/maze.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# unit test
test_maze: $(MAZEOBJ) $(SRC)/utest/test_maze.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(TARGETS) $(OBJS) test_maze *dSYM
