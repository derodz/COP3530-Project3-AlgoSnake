#include <Window.h>

GamePanel::GamePanel() {
  bg.setFillColor(sf::Color::Black);
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

void GamePanel::render(sf::RenderWindow &window, const Game &game) {
  int rows = game.getRows();
  int cols = game.getCols();
  bg.setSize(sf::Vector2f(cols * CELL_SIZE, rows * CELL_SIZE));
  window.draw(bg);

  // draw food
  auto food = game.getFoodPos();
  foodShape.setPosition(food.second * CELL_SIZE, food.first * CELL_SIZE);
  window.draw(foodShape);

  // if dead, draw bigX
  if (game.isDead()) {
    window.draw(bigX[0]);
    window.draw(bigX[1]);
  }

  // draw snake
  const auto &snake = game.getSnake();
  if (!snake.empty()) {
    auto it = snake.begin();
    segment.setFillColor(sf::Color::Magenta);
    segment.setPosition(it->second * CELL_SIZE, it->first * CELL_SIZE);
    window.draw(segment);
    ++it;
    segment.setFillColor(sf::Color::Blue);
    for (; it != snake.end(); ++it) {
      segment.setPosition(it->second * CELL_SIZE, it->first * CELL_SIZE);
      window.draw(segment);
    }
  }
}

void UI::render(sf::RenderWindow &window, const Game &game) {
  gamePanel.render(window, game);
}

void UI::handleEvent(const sf::Event &event, Game &game) {
  // supports arrow keys or WASD
  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.code) {
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
