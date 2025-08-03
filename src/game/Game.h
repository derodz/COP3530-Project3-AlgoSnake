#pragma once
#include <Graph.h>
#include <deque>
#include <random>
#include <utility>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std;

enum class CellType
{
  Empty,
  Food,
  Obstacle
};
enum class Algorithm
{
  BFS,
  AStar
};
enum class Direction
{
  Up,
  Down,
  Left,
  Right
};

// forward declarations
vector<pair<int, int>> bfsGetPath(const Graph<CellType> &graph,
                                  const deque<pair<int, int>> &snake,
                                  pair<int, int> start, pair<int, int> target);
vector<pair<int, int>> aStarGetPath(const Graph<CellType> &graph,
                                    const deque<pair<int, int>> &snake,
                                    pair<int, int> start,
                                    pair<int, int> target);

class Game
{
private:
  Graph<CellType> grid;
  deque<pair<int, int>> snake;
  pair<int, int> foodPos;
  chrono::_V2::steady_clock::time_point startTime = chrono::steady_clock::now();
  int elapsedTime;
  Algorithm algo;

  bool dead;
  // vector<pair<int, int>> currentPath;
  Direction curDirection;

  // rng related
  std::mt19937 rng;
  std::uniform_int_distribution<int> rowDist;
  std::uniform_int_distribution<int> colDist;

  void placeFood();
  vector<pair<int, int>> findPath(pair<int, int> target);

public:
  Game(unsigned seed, int rows, int cols);
  void update();
  void setAlgorithm(Algorithm algorithm) { algo = algorithm; };

  // manual interaction //
  void setDirection(Direction d);
  ////////////////////////

  const deque<pair<int, int>> &getSnake() const { return snake; };
  pair<int, int> getFoodPos() const { return foodPos; };
  int getRows() const { return grid.getRows(); };
  int getCols() const { return grid.getCols(); };
  bool isDead() const { return dead; };
  int getFoodsEaten() const { return snake.size() - 1; };
  void calculateElapsedTime();
  int getElapsedTime() const { return elapsedTime; };
  void printStats() { cout << "Foods eaten: " << getFoodsEaten() << endl; };
};
