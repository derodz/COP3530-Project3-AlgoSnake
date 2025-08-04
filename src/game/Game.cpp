#include <Game.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>

Game::Game(unsigned seed, int rows, int cols)
    : grid(rows, cols), curDirection(Direction::Up), dead(false), rng(seed),
      rowDist(0, grid.getRows() - 1), colDist(0, grid.getCols() - 1) {
  algo = Algorithm::None;
  foodPos = {-1, -1}; // invalid pos to avoid clearing junk on first placeFood
  snake.push_front({grid.getRows() / 2 + 2, grid.getCols() / 2});
  snake.push_front({grid.getRows() / 2 + 1, grid.getCols() / 2});
  snake.push_front({grid.getRows() / 2, grid.getCols() / 2});
  placeFood();
  failureDistance = -1;
  savedSummary = true; // initial no save needed
}

void Game::reset(Algorithm newAlgo) {
  dead = false;
  algo = newAlgo;
  snake.clear();
  snake.push_front({grid.getRows() / 2 + 2, grid.getCols() / 2});
  snake.push_front({grid.getRows() / 2 + 1, grid.getCols() / 2});
  snake.push_front({grid.getRows() / 2, grid.getCols() / 2});
  foodPos = {-1, -1}; // invalid pos to avoid clearing junk on first placeFood
  placeFood();
  startTime = chrono::steady_clock::now();
  elapsedTime = 0;
  stepsTaken = 0;
  compTimes.clear();
  nodesExplored.clear();
  perCallStats.clear();
  curDirection = Direction::Up;
  failureDistance = -1;
  savedSummary = false;
  initStatsFile(newAlgo);
}

void Game::placeFood() {
  int rows = grid.getRows();
  int cols = grid.getCols();

  if (foodPos.first >= 0 && foodPos.first < rows && foodPos.second >= 0 &&
      foodPos.second < cols) {
    grid.setMatrixNode(foodPos.first, foodPos.second, CellType::Empty);
  }

  pair<int, int> newPos = {rowDist(rng), colDist(rng)};
  while (find(snake.begin(), snake.end(), newPos) != snake.end()) {
    newPos = {rowDist(rng), colDist(rng)};
  }
  foodPos = newPos;
  grid.setMatrixNode(foodPos.first, foodPos.second, CellType::Food);
}

std::pair<vector<pair<int, int>>, int> Game::findPath(pair<int, int> target) {
  auto head = snake.front();
  auto start = chrono::steady_clock::now();
  std::pair<vector<pair<int, int>>, int> result;
  if (algo == Algorithm::BFS) {
    result = bfsGetPath(grid, snake, head, target);
  } else {
    result = aStarGetPath(grid, snake, head, target);
  }
  auto end = chrono::steady_clock::now();
  long long compTime =
      chrono::duration_cast<chrono::microseconds>(end - start).count();
  addCompTime(compTime);

  return result;
}

void Game::update() {
  if (dead)
    return;

  calculateElapsedTime();

  auto head = snake.front();
  auto result = findPath(foodPos);
  auto path = result.first;
  auto nodes = result.second;
  addNodesExplored(nodes);
  if (path.empty()) {
    // this condition falls back to chasing the tail
    auto tail = snake.back();
    deque<pair<int, int>> tempSnake = snake;
    tempSnake.pop_back();
    auto start = chrono::steady_clock::now();
    std::pair<vector<pair<int, int>>, int> tailResult;
    if (algo == Algorithm::BFS) {
      tailResult = bfsGetPath(grid, tempSnake, head, tail);
    } else {
      tailResult = aStarGetPath(grid, tempSnake, head, tail);
    }
    auto end = chrono::steady_clock::now();
    long long compTime =
        chrono::duration_cast<chrono::microseconds>(end - start).count();
    addCompTime(compTime);
    addNodesExplored(tailResult.second);
    path = tailResult.first;

    if (path.empty()) {
      // no current path found, try continuing in the current direction
      auto startOfSnakeBody = next(snake.begin());
      int right = head.second + 1;
      int up = head.first + 1;
      int left = head.second - 1;
      int down = head.first - 1;
      if (right < grid.getCols() && right != (*startOfSnakeBody).second) {
        path.push_back({head.first, right});
      } else if (left >= 0 && left != (*startOfSnakeBody).second) {
        path.push_back({head.first, left});
      } else if (up < grid.getRows() && up != (*startOfSnakeBody).first) {
        path.push_back({up, head.second});
      } else if (down >= 0 && down != (*startOfSnakeBody).first) {
        path.push_back({down, head.second});
      } else {
        // no valid moves left, game over
        dead = true;
        failureDistance = std::abs(head.first - foodPos.first) +
                          std::abs(head.second - foodPos.second);
        if (!savedSummary) {
          saveSummary();
          savedSummary = true;
        }
        return;
      }
    }
  }
  pair<int, int> newHead = path.front();

  int rows = grid.getRows();
  int cols = grid.getCols();

  if (newHead.first < 0 || newHead.first >= rows || newHead.second < 0 ||
      newHead.second >= cols) {
    // hit wall
    dead = true;
    failureDistance = std::abs(head.first - foodPos.first) +
                      std::abs(head.second - foodPos.second);
    if (!savedSummary) {
      saveSummary();
      savedSummary = true;
    }
    return;
  }

  bool eat = (newHead == foodPos);
  snake.push_front(newHead);

  if (eat) {
    pair<int, int> oldFood = foodPos;
    placeFood();
    grid.setMatrixNode(oldFood.first, oldFood.second, CellType::Empty);
  } else {
    snake.pop_back();
  }

  // self collision check
  head = snake.front();
  for (auto it = next(snake.begin()); it != snake.end(); ++it) {
    if (*it == head) {
      dead = true;
      failureDistance = std::abs(head.first - foodPos.first) +
                        std::abs(head.second - foodPos.second);
      if (!savedSummary) {
        saveSummary();
        savedSummary = true;
      }
      break;
    }
  }

  incrementStepsTaken();
}

void Game::setDirection(Direction d) {
  if ((curDirection == Direction::Up && d == Direction::Down) ||
      (curDirection == Direction::Down && d == Direction::Up) ||
      (curDirection == Direction::Left && d == Direction::Right) ||
      (curDirection == Direction::Right && d == Direction::Left)) {
    return;
  }
  curDirection = d;
}

void Game::calculateElapsedTime() {
  auto now = std::chrono::steady_clock::now();
  elapsedTime = chrono::duration_cast<chrono::seconds>(now - startTime).count();
}

long long Game::getAvgCompTime() const {
  if (compTimes.empty())
    return 0;
  long long sum = 0;
  for (long long t : compTimes) {
    sum += t;
  }
  return sum / compTimes.size();
}

int Game::getAvgNodesExplored() const {
  if (nodesExplored.empty())
    return 0;
  long long sum = 0;
  for (int n : nodesExplored) {
    sum += n;
  }
  return static_cast<int>(sum / nodesExplored.size());
}

void Game::addCompTime(long long time) { compTimes.push_back(time); }

void Game::addNodesExplored(int nodes) {
  nodesExplored.push_back(nodes);
  perCallStats.push_back({getFoodsEaten(), getStepsTaken(), getElapsedTime(),
                          compTimes.back(), nodes});
}

void Game::initStatsFile(Algorithm newAlgo) {
  try {
    string algoName = (newAlgo == Algorithm::AStar) ? "AStar" : "BFS";
    string filename = algoName + "_stats.csv";

    bool need_header = true;
    {
      std::ifstream check(filename);
      if (check.good()) {
        check.seekg(0, std::ios::end);
        if (check.tellg() > 0) {
          need_header = false;
        }
      }
    }

    ofstream file(filename, ios::app);
    if (!file.is_open()) {
      cerr << "Failed to open " << filename << endl;
      return;
    }

    if (need_header) {
      file << "Food Eaten,Steps Taken,Elapsed Time (s),Comp Time (us),Nodes "
              "Explored\n";
    }
    file.close();
  } catch (const std::exception &e) {
    cerr << e.what() << '\n';
  }
}

void Game::saveSummary() {
  if (savedSummary)
    return;
  try {
    string filename = "summary_stats.csv";

    bool need_header = true;
    {
      std::ifstream check(filename);
      if (check.good()) {
        check.seekg(0, std::ios::end);
        if (check.tellg() > 0) {
          need_header = false;
        }
      }
    }

    ofstream file(filename, ios::app);
    if (!file.is_open()) {
      cerr << "Failed to open " << filename << endl;
      return;
    }

    if (need_header) {
      file << "Algorithm,Food Eaten,Steps Taken,Elapsed Time (s),Avg Comp Time "
              "(us),Avg Nodes Explored,Failure Distance\n";
    }

    string algoName = (algo == Algorithm::AStar) ? "AStar" : "BFS";
    file << algoName << "," << getFoodsEaten() << "," << getStepsTaken() << ","
         << getElapsedTime() << "," << getAvgCompTime() << ","
         << getAvgNodesExplored() << "," << getFailureDistance() << "\n";
    file.close();

    // Now write the per-call stats
    string perCallFilename =
        (algo == Algorithm::AStar) ? "AStar_stats.csv" : "BFS_stats.csv";
    ofstream perCallFile(perCallFilename, ios::app);
    if (!perCallFile.is_open()) {
      cerr << "Failed to open " << perCallFilename << endl;
      return;
    }

    for (const auto &stat : perCallStats) {
      perCallFile << stat.foodEaten << "," << stat.stepsTaken << ","
                  << stat.elapsedTime << "," << stat.compTime << ","
                  << stat.nodesExplored << "\n";
    }
    perCallFile.close();

    savedSummary = true;
  } catch (const std::exception &e) {
    cerr << e.what() << '\n';
  }
}
