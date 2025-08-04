#include <Game.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>

Game::Game(unsigned seed, int rows, int cols)
    : grid(rows, cols), curDirection(Direction::Up), dead(false), rng(seed),
      rowDist(0, grid.getRows() - 1), colDist(0, grid.getCols() - 1)
{
  algo = Algorithm::None;
  foodPos = {-1, -1}; // invalid pos to avoid clearing junk on first placeFood
  snake.push_front({12, 5});
  snake.push_front({11, 5});
  snake.push_front({10, 5});
  placeFood();
}

void Game::reset(Algorithm newAlgo)
{
  algo = newAlgo;
  foodPos = {-1, -1}; // invalid pos to avoid clearing junk on first placeFood
  snake.clear();
  compTimes.clear();
  elapsedTime = 0;
  stepsTaken = 0;
  snake.push_front({12, 5});
  snake.push_front({11, 5});
  snake.push_front({10, 5});
  placeFood();
  startTime = chrono::steady_clock::now();
  initStatsFile(newAlgo);
  dead = false;
}

void Game::placeFood()
{
  int rows = grid.getRows();
  int cols = grid.getCols();

  if (foodPos.first >= 0 && foodPos.first < rows && foodPos.second >= 0 &&
      foodPos.second < cols)
  {
    grid.setMatrixNode(foodPos.first, foodPos.second, CellType::Empty);
  }

  pair<int, int> newPos = {rowDist(rng), colDist(rng)};
  while (find(snake.begin(), snake.end(), newPos) != snake.end())
  {
    newPos = {rowDist(rng), colDist(rng)};
  }
  foodPos = newPos;
  grid.setMatrixNode(foodPos.first, foodPos.second, CellType::Food);
}

vector<pair<int, int>> Game::findPath(pair<int, int> target)
{
  auto head = snake.front();
  auto start = chrono::steady_clock::now();
  vector<pair<int, int>> path;
  if (algo == Algorithm::BFS)
  {
    path = bfsGetPath(grid, snake, head, target);
  }
  else
  {
    path = aStarGetPath(grid, snake, head, target);
  }
  auto end = chrono::steady_clock::now();
  int compTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
  addCompTime(compTime);

  return path;
}

void Game::update()
{
  if (dead)
    return;

  calculateElapsedTime();

  auto head = snake.front();
  auto path = findPath(foodPos);
  if (path.empty())
  {
    // this condition falls back to chasing the tail
    auto tail = snake.back();
    deque<pair<int, int>> tempSnake = snake;
    tempSnake.pop_back();
    auto start = chrono::steady_clock::now();
    if (algo == Algorithm::BFS)
    {
      path = bfsGetPath(grid, tempSnake, head, tail);
    }
    else
    {
      path = aStarGetPath(grid, tempSnake, head, tail);
    }
    auto end = chrono::steady_clock::now();
    int compTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    addCompTime(compTime);

    if (path.empty())
    {
      // no current path found, try continuing in the current direction
      auto startOfSnakeBody = next(snake.begin());
      int right = head.second + 1;
      int up = head.first + 1;
      int left = head.second - 1;
      int down = head.first - 1;
      if (right < grid.getCols() - 1 && right != (*startOfSnakeBody).second)
      {
        path.push_back({head.first, right});
      }
      else if (left >= 0 && left != (*startOfSnakeBody).second)
      {
        path.push_back({head.first, left});
      }
      else if (up < grid.getRows() - 1 && up != (*startOfSnakeBody).first)
      {
        path.push_back({up, head.second});
      }
      else if (down >= 0 && down != (*startOfSnakeBody).first)
      {
        path.push_back({down, head.second});
      }
      else
      {
        // no valid moves left, game over
        dead = true;
        return;
      }
    }
  }
  pair<int, int> newHead = path.front();

  int rows = grid.getRows();
  int cols = grid.getCols();

  if (newHead.first < 0 || newHead.first >= rows || newHead.second < 0 ||
      newHead.second >= cols)
  {
    // hit wall
    dead = true;
    return;
  }

  bool eat = (newHead == foodPos);
  snake.push_front(newHead);

  if (eat)
  {
    pair<int, int> oldFood = foodPos;
    placeFood();
    grid.setMatrixNode(oldFood.first, oldFood.second, CellType::Empty);
  }
  else
  {
    snake.pop_back();
  }

  // self collision check
  head = snake.front();
  for (auto it = next(snake.begin()); it != snake.end(); ++it)
  {
    if (*it == head)
    {
      dead = true;
      break;
    }
  }

  incrementStepsTaken();
}

void Game::setDirection(Direction d)
{
  if ((curDirection == Direction::Up && d == Direction::Down) ||
      (curDirection == Direction::Down && d == Direction::Up) ||
      (curDirection == Direction::Left && d == Direction::Right) ||
      (curDirection == Direction::Right && d == Direction::Left))
  {
    return;
  }
  curDirection = d;
}

void Game::calculateElapsedTime()
{
  auto now = std::chrono::steady_clock::now();
  elapsedTime = chrono::duration_cast<chrono::seconds>(now - startTime).count();
}

int Game::getAvgCompTime() const
{
  if (compTimes.empty() || !isDead())
    return 0;
  int sum = 0;
  for (int t : compTimes)
  {
    sum += t;
  }
  return sum / compTimes.size();
}

void Game::addCompTime(int time)
{
  compTimes.push_back(time);
  saveStats(getFoodsEaten(), getStepsTaken(), getElapsedTime(), time);
}

void Game::initStatsFile(Algorithm newAlgo)
{
  try
  {
    string algoName = (newAlgo == Algorithm::AStar) ? "AStar" : "BFS";
    string filename = algoName + "_stats.csv";
    ofstream file(filename, ios::app);
    if (!file.is_open())
    {
      cerr << "Failed to open " << filename << endl;
      return;
    }

    file << "Food Eaten, Steps Taken, Elapsed Time (s), Comp Time (ns)\n";
    file.close();
  }
  catch (const std::exception &e)
  {
    cerr << e.what() << '\n';
  }
}

void Game::saveStats(int foodEaten, int stepsTaken, int elapsedTime, int compTime)
{
  try
  {
    string algoName = (algo == Algorithm::AStar) ? "AStar" : "BFS";
    string filename = algoName + "_stats.csv";
    ofstream file(filename, ios::app);

    if (!file.is_open())
    {
      cerr << "Failed to open " << filename << endl;
      return;
    }

    file << foodEaten << ", " << stepsTaken << ", " << elapsedTime << ", " << compTime << "\n";
    file.close();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
  }
}
