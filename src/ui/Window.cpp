#include <Window.h>

GamePanel::GamePanel()
{
  font.loadFromFile("arial.ttf");
  bg.setFillColor(sf::Color::Black);
  statsBg.setFillColor(sf::Color::White);

  snakeTypeText.setFont(font);
  snakeTypeText.setCharacterSize(static_cast<unsigned int>(24));
  snakeTypeText.setFillColor(sf::Color::Black);

  statsText.setFont(font);
  statsText.setCharacterSize(static_cast<unsigned int>(24));
  statsText.setFillColor(sf::Color::Black);

  elapsedTimeText.setFont(font);
  elapsedTimeText.setCharacterSize(static_cast<unsigned int>(24));
  elapsedTimeText.setFillColor(sf::Color::Black);

  stepsTakenText.setFont(font);
  stepsTakenText.setCharacterSize(static_cast<unsigned int>(24));
  stepsTakenText.setFillColor(sf::Color::Black);

  avgCompTimeText.setFont(font);
  avgCompTimeText.setCharacterSize(static_cast<unsigned int>(24));
  avgCompTimeText.setFillColor(sf::Color::Black);

  btnAstar.setSize(sf::Vector2f(200, 100));
  btnAstarTexture.loadFromFile("btn_astar.jpg");
  btnAstar.setTexture(&btnAstarTexture);

  btnBFS.setSize(sf::Vector2f(200, 100));
  btnBFSTexture.loadFromFile("btn_bfs.jpg");
  btnBFS.setTexture(&btnBFSTexture);

  segment.setRadius(CELL_SIZE / 2.0f);
  foodShape.setRadius(CELL_SIZE / 2.0f);
  foodShape.setFillColor(sf::Color::Red);

  float width = 400.0f;
  float height = 400.0f;
  float diagonal = std::sqrt(width * width + height * height);
  float thickness = 20.0f;

  bigX[0].setFillColor(sf::Color::Red);
  bigX[1].setFillColor(sf::Color::Red);
  bigX[0].setSize(sf::Vector2f(diagonal, thickness));
  bigX[1].setSize(sf::Vector2f(diagonal, thickness));
  bigX[0].setOrigin(bigX[0].getSize().x / 2.0f, bigX[0].getSize().y / 2.0f);
  bigX[1].setOrigin(bigX[1].getSize().x / 2.0f, bigX[1].getSize().y / 2.0f);
  bigX[0].setRotation(45.0f);
  bigX[1].setRotation(-45.0f);
}

void GamePanel::render(sf::RenderWindow &window, const Game &game)
{
  int rows = game.getRows();
  int cols = game.getCols();
  bigX[0].setPosition((cols * CELL_SIZE) / 2.0f, (rows * CELL_SIZE) / 2.0f);
  bigX[1].setPosition((cols * CELL_SIZE) / 2.0f, (rows * CELL_SIZE) / 2.0f);
  bg.setSize(sf::Vector2f(cols * CELL_SIZE, rows * CELL_SIZE));

  // gui and stats panel
  statsBg.setSize(sf::Vector2f(cols * CELL_SIZE, 300));
  statsBg.setPosition(0, rows * CELL_SIZE);

  Algorithm chosenAlgo = game.getAlgorithm();
  string snakeType = "None";
  if (chosenAlgo == Algorithm::BFS)
    snakeType = "BFS";
  else if (chosenAlgo == Algorithm::AStar)
    snakeType = "A* Search";

  snakeTypeText.setString("Snake Type: " + snakeType);
  snakeTypeText.setPosition(10.0f, rows * CELL_SIZE + 10.0f);

  statsText.setString("Food Eaten: " + std::to_string(game.getFoodsEaten()));
  statsText.setPosition(10.0f, rows * CELL_SIZE + 50.0f);

  elapsedTimeText.setString("Elapsed Time: " + std::to_string(game.getElapsedTime()) + " s");
  elapsedTimeText.setPosition(10.0f, rows * CELL_SIZE + 90.0f);

  stepsTakenText.setString("Steps Taken: " + std::to_string(game.getStepsTaken()));
  stepsTakenText.setPosition(10.0f, rows * CELL_SIZE + 130.0f);

  avgCompTimeText.setString("Avg Computation Time: " + std::to_string(game.getAvgCompTime()) + " ns");
  avgCompTimeText.setPosition(10.0f, rows * CELL_SIZE + 170.0f);

  window.draw(btnAstar);
  btnAstar.setPosition(cols * CELL_SIZE - 210, rows * CELL_SIZE + 10);

  window.draw(btnBFS);
  btnBFS.setPosition(cols * CELL_SIZE - 210, rows * CELL_SIZE + 120);

  window.draw(bg);
  window.draw(statsBg);
  window.draw(snakeTypeText);
  window.draw(statsText);
  window.draw(elapsedTimeText);
  window.draw(stepsTakenText);
  window.draw(avgCompTimeText);
  window.draw(btnAstar);
  window.draw(btnBFS);

  // draw food
  auto food = game.getFoodPos();
  foodShape.setPosition(food.second * CELL_SIZE, food.first * CELL_SIZE);
  window.draw(foodShape);

  // if dead, draw bigX
  if (game.isDead())
  {
    window.draw(bigX[0]);
    window.draw(bigX[1]);
  }

  // draw snake
  const auto &snake = game.getSnake();
  if (!snake.empty())
  {
    auto it = snake.begin();
    segment.setFillColor(sf::Color::Magenta);
    segment.setPosition(it->second * CELL_SIZE, it->first * CELL_SIZE);
    window.draw(segment);
    ++it;
    segment.setFillColor(sf::Color::Blue);
    for (; it != snake.end(); ++it)
    {
      segment.setPosition(it->second * CELL_SIZE, it->first * CELL_SIZE);
      window.draw(segment);
    }
  }
}

void UI::render(sf::RenderWindow &window, const Game &game)
{
  gamePanel.render(window, game);
}

void UI::handleEvent(const sf::Event &event, Game &game)
{
  // supports arrow keys or WASD
  if (event.type == sf::Event::KeyPressed)
  {
    switch (event.key.code)
    {
    case sf::Keyboard::Up:
    case sf::Keyboard::W:
      game.setDirection(Direction::Up);
      break;
    case sf::Keyboard::Down:
    case sf::Keyboard::S:
      game.setDirection(Direction::Down);
      break;
    case sf::Keyboard::Left:
    case sf::Keyboard::A:
      game.setDirection(Direction::Left);
      break;
    case sf::Keyboard::Right:
    case sf::Keyboard::D:
      game.setDirection(Direction::Right);
      break;
    default:
      break;
    }
  }
}

// TODO
void UI::update(const Game &game) {}
