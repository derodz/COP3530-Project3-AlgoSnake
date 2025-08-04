#pragma once
#include <Graph.h>
#include <chrono>
#include <deque>
#include <iostream>
#include <math.h>
#include <random>
#include <utility>
#include <vector>

using namespace std;

enum class CellType { Empty, Food };
enum class Algorithm { None, BFS, AStar };
enum class Direction { Up, Down, Left, Right };

// forward declarations
vector<pair<int, int>> bfsGetPath(const Graph<CellType> &graph,
                                  const deque<pair<int, int>> &snake,
                                  pair<int, int> start, pair<int, int> target);
vector<pair<int, int>> aStarGetPath(const Graph<CellType> &graph,
                                    const deque<pair<int, int>> &snake,
                                    pair<int, int> start,
                                    pair<int, int> target);

class Game {
private:
  Graph<CellType> grid;
  deque<pair<int, int>> snake;
  pair<int, int> foodPos;
  chrono::_V2::steady_clock::time_point startTime = chrono::steady_clock::now();
  int elapsedTime = 0; // in seconds
  int stepsTaken = 0;
  vector<int> compTimes; // in nanoseconds
  Algorithm algo;

  bool dead;
  Direction curDirection;

  // rng related
  std::mt19937 rng;
  std::uniform_int_distribution<int> rowDist;
  std::uniform_int_distribution<int> colDist;

  void placeFood();
  vector<pair<int, int>> findPath(pair<int, int> target);

public:
  Game(unsigned seed, int rows, int cols);
  void reset(Algorithm newAlgo);
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
  int getFoodsEaten() const { return snake.size() - 3; };
  void calculateElapsedTime();
  void incrementStepsTaken() { stepsTaken++; };
  int getStepsTaken() const { return stepsTaken; };
  int getElapsedTime() const { return elapsedTime; };
  int getAvgCompTime() const;
  void addCompTime(int time);
  void printStats() { cout << "Foods eaten: " << getFoodsEaten() << endl; };
  Algorithm getAlgorithm() const { return algo; }
  void initStatsFile(Algorithm newAlgo);
  void saveStats(int foodEaten, int stepsTaken, int elapsedTime, int compTime);
};
