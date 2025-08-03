#include <Window.h>

GamePanel::GamePanel()
{
  font.loadFromFile("arial.ttf");
  bg.setFillColor(sf::Color::Black);
  statsBg.setFillColor(sf::Color::White);
  statsText.setFont(font);
  elapsedTimeText.setFont(font);
  statsText.setCharacterSize(static_cast<unsigned int>(CELL_SIZE * 1.2f));
  statsText.setFillColor(sf::Color::Black);
  elapsedTimeText.setCharacterSize(static_cast<unsigned int>(CELL_SIZE * 1.2f));
  elapsedTimeText.setFillColor(sf::Color::Black);
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
  bigX[0].setPosition(width / 2.0f, height / 2.0f);
  bigX[1].setPosition(width / 2.0f, height / 2.0f);
  bigX[0].setRotation(45.0f);
  bigX[1].setRotation(-45.0f);
}

void GamePanel::render(sf::RenderWindow &window, const Game &game)
{
  int rows = game.getRows();
  int cols = game.getCols();
  bg.setSize(sf::Vector2f(cols * CELL_SIZE, rows * CELL_SIZE));
  window.draw(bg);

  // draw stats
  statsBg.setSize(sf::Vector2f(cols * CELL_SIZE, 4 * CELL_SIZE));
  statsBg.setPosition(0, rows * CELL_SIZE);

  statsText.setString("Food Eaten: " + std::to_string(game.getFoodsEaten()));
  statsText.setPosition(10.0f, rows * CELL_SIZE);

  elapsedTimeText.setString("Elapsed Time: " + std::to_string(game.getElapsedTime()) + "s");
  elapsedTimeText.setPosition(10.0f, (rows + 2) * CELL_SIZE);

  window.draw(statsBg);
  window.draw(statsText);
  window.draw(elapsedTimeText);

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
