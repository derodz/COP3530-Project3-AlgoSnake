#include <Game.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>

Game::Game(unsigned seed, int rows, int cols)
    : grid(rows, cols), dead(false), rng(seed), rowDist(0, grid.getRows() - 1),
      colDist(0, grid.getCols() - 1) {
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
  addNodesExplored(result.second); // add nodes from food path attempt

  bool goToFood = false;
  if (!path.empty()) {
    // simulate snake state after following path and eating food
    deque<pair<int, int>> new_snake = snake;
    for (const auto &pos : path) {
      new_snake.push_front(pos);
    }
    // normal moves: pop tail for all but the last step
    for (size_t i = 0; i < path.size() - 1; ++i) {
      new_snake.pop_back();
    }

    // check if snake can chase tail safely in new new state
    deque<pair<int, int>> temp_body = new_snake;
    temp_body.pop_back(); // exclude tail from obstacles
    auto new_head = new_snake.front();
    auto new_tail = new_snake.back();

    // perform check without adding to stats (validation, not main computation)
    std::pair<vector<pair<int, int>>, int> check_result;
    if (algo == Algorithm::BFS) {
      check_result = bfsGetPath(grid, temp_body, new_head, new_tail);
    } else {
      check_result = aStarGetPath(grid, temp_body, new_head, new_tail);
    }

    if (!check_result.first.empty()) {
      goToFood = true;
    }
  }

  if (!goToFood) {
    auto tail = snake.back();
    auto tail_result = findPath(tail);
    auto tail_path = tail_result.first;
    addNodesExplored(tail_result.second); // track this call's nodes

    if (!tail_path.empty()) {
      path = {tail_path.front()}; // the first safe move toward tail
    } else {
      // start timing fallback computation
      auto start = chrono::steady_clock::now();

      // get possible moves
      vector<pair<int, int>> possibleMoves;
      auto neighbors = grid.getNodeNeighbors(head.first, head.second);
      for (const auto &nei : neighbors) {
        if (find(snake.begin(), snake.end(), nei) == snake.end()) {
          possibleMoves.push_back(nei);
        }
      }

      if (possibleMoves.empty()) {
        dead = true;
        failureDistance = std::abs(head.first - foodPos.first) +
                          std::abs(head.second - foodPos.second);
        if (!savedSummary) {
          saveSummary();
          savedSummary = true;
        }
        return;
      }

      int maxReachable = -1;
      double minDist = std::numeric_limits<double>::infinity();
      pair<int, int> bestMove = {-1, -1};
      int totalExpanded = 0;

      for (const auto &cand : possibleMoves) {
        deque<pair<int, int>> newSnake = snake;
        newSnake.push_front(cand);
        newSnake.pop_back();

        // use BFS to count reachable, and count expanded nodes
        vector<vector<bool>> visited(grid.getRows(),
                                     vector<bool>(grid.getCols(), false));
        queue<pair<int, int>> q;
        q.push(cand);
        visited[cand.first][cand.second] = true;
        int reachable = 1;
        int expanded = 0;

        while (!q.empty()) {
          auto curr = q.front();
          q.pop();
          expanded++;

          auto neis = grid.getNodeNeighbors(curr.first, curr.second);
          for (const auto &nei : neis) {
            int ny = nei.first, nx = nei.second;
            if (ny >= 0 && ny < grid.getRows() && nx >= 0 &&
                nx < grid.getCols() && !visited[ny][nx] &&
                find(newSnake.begin(), newSnake.end(), make_pair(ny, nx)) ==
                    newSnake.end()) {
              visited[ny][nx] = true;
              q.push(nei);
              reachable++;
            }
          }
        }

        totalExpanded += expanded;

        double dist = std::abs(cand.first - foodPos.first) +
                      std::abs(cand.second - foodPos.second);

        if (reachable > maxReachable ||
            (reachable == maxReachable && dist < minDist)) {
          maxReachable = reachable;
          minDist = dist;
          bestMove = cand;
        }
      }

      auto end = chrono::steady_clock::now();
      long long compTime =
          chrono::duration_cast<chrono::microseconds>(end - start).count();
      addCompTime(compTime);
      addNodesExplored(totalExpanded);

      if (bestMove.first == -1) {
        dead = true;
        failureDistance = std::abs(head.first - foodPos.first) +
                          std::abs(head.second - foodPos.second);
        if (!savedSummary) {
          saveSummary();
          savedSummary = true;
        }
        return;
      }

      path = {bestMove};
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

int Game::countReachable(const deque<pair<int, int>> &simSnake,
                         pair<int, int> startPos) {
  int rows = grid.getRows();
  int cols = grid.getCols();

  vector<vector<bool>> visited(rows, vector<bool>(cols, false));
  queue<pair<int, int>> q;
  q.push(startPos);
  visited[startPos.first][startPos.second] = true;
  int count = 1;

  while (!q.empty()) {
    auto curr = q.front();
    q.pop();
    auto neighbors = grid.getNodeNeighbors(curr.first, curr.second);
    for (const auto &nei : neighbors) {
      int ny = nei.first, nx = nei.second;
      if (ny >= 0 && ny < rows && nx >= 0 && nx < cols && !visited[ny][nx] &&
          find(simSnake.begin(), simSnake.end(), make_pair(ny, nx)) ==
              simSnake.end()) {
        visited[ny][nx] = true;
        q.push({ny, nx});
        count++;
      }
    }
  }
  return count;
}
