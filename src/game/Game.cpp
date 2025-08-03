#include <Game.h>
#include <algorithm>
#include <chrono>
#include <iostream>

Game::Game(unsigned seed, int rows, int cols)
    : grid(rows, cols), curDirection(Direction::Up), dead(false), rng(seed),
      rowDist(0, grid.getRows() - 1), colDist(0, grid.getCols() - 1)
{
  foodPos = {-1, -1}; // invalid pos to avoid clearing junk on first placeFood
  snake.push_front({12, 5});
  snake.push_front({11, 5});
  snake.push_front({10, 5});
  placeFood();
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
  if (algo == Algorithm::BFS)
  {
    return bfsGetPath(grid, snake, head, target);
  }
  else
  {
    return aStarGetPath(grid, snake, head, target);
  }
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
    if (algo == Algorithm::BFS)
    {
      path = bfsGetPath(grid, tempSnake, head, tail);
    }
    else
    {
      path = aStarGetPath(grid, tempSnake, head, tail);
    }
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
        cout << "Game ended after " << elapsedTime << " seconds.";
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
    cout << "Game ended after " << elapsedTime << " seconds.";
    return;
  }

  bool eat = (newHead == foodPos);
  snake.push_front(newHead);

  if (eat)
  {
    printStats();
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
      cout << "Game ended after " << elapsedTime << " seconds.";
      break;
    }
  }
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
