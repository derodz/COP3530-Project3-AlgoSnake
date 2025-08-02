#pragma once
#include <Graph.h>
#include <deque>
#include <random>
#include <utility>
#include <vector>

using namespace std;

enum class CellType { Empty, Food, Obstacle };
enum class Algorithm { BFS, AStar };
enum class Direction { Up, Down, Left, Right };

class Game {
private:
  Graph<CellType> grid;
  deque<pair<int, int>> snake;
  pair<int, int> foodPos;
  Algorithm algo;

  bool dead;
  bool pathFound;

  // rng related
  std::mt19937 rng;
  std::uniform_int_distribution<int> rowDist;
  std::uniform_int_distribution<int> colDist;

  // manual interaction
  Direction curDirection;

  void placeFood();

  // TODO integrate pathfinding algos into snake. manual for now
  vector<pair<int, int>> findPath();

public:
  // Game();
  Game(unsigned seed, int rows, int cols);
  void update();
  void setAlgorithm(Algorithm algorithm) { algo = algorithm; };

  // manual interaction
  void setDirection(Direction d);
  //

  const deque<pair<int, int>> &getSnake() const { return snake; };
  pair<int, int> getFoodPos() const { return foodPos; };
  int getRows() const { return grid.getRows(); };
  int getCols() const { return grid.getCols(); };
  bool isDead() const { return dead; };
};
