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

// forward declarations
std::pair<vector<pair<int, int>>, int>
bfsGetPath(const Graph<CellType> &graph, const deque<pair<int, int>> &snake,
           pair<int, int> start, pair<int, int> target);
std::pair<vector<pair<int, int>>, int>
aStarGetPath(const Graph<CellType> &graph, const deque<pair<int, int>> &snake,
             pair<int, int> start, pair<int, int> target);

struct PerCallStat {
  int foodEaten;
  int stepsTaken;
  int elapsedTime;
  long long compTime;
  int nodesExplored;
};

class Game {
private:
  Graph<CellType> grid;
  deque<pair<int, int>> snake;
  pair<int, int> foodPos;
  Algorithm algo;
  bool dead;

  void placeFood();
  std::pair<vector<pair<int, int>>, int> findPath(pair<int, int> target);

  // rng related
  std::mt19937 rng;
  std::uniform_int_distribution<int> rowDist;
  std::uniform_int_distribution<int> colDist;

  // stats related
  chrono::_V2::steady_clock::time_point startTime = chrono::steady_clock::now();
  int elapsedTime = 0; // in seconds
  int stepsTaken = 0;
  vector<long long> compTimes; // in microseconds
  vector<int> nodesExplored;
  vector<PerCallStat> perCallStats;
  int failureDistance;
  bool savedSummary;

public:
  Game(unsigned seed, int rows, int cols);
  void reset(Algorithm newAlgo);
  void update();
  void setAlgorithm(Algorithm algorithm) { algo = algorithm; };

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
  long long getAvgCompTime() const;
  int getAvgNodesExplored() const;
  int getFailureDistance() const { return failureDistance; };
  void addCompTime(long long time) { compTimes.push_back(time); };
  void addNodesExplored(int nodes);
  void printStats() { cout << "Foods eaten: " << getFoodsEaten() << endl; };
  Algorithm getAlgorithm() const { return algo; };
  void initStatsFile(Algorithm newAlgo);
  void saveSummary();
  int countReachable(const deque<pair<int, int>> &simSnake,
                     pair<int, int> startPos);
};
