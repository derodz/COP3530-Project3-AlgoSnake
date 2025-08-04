#include <Window.h>
#include <algorithm>

GamePanel::GamePanel() { bg.setFillColor(sf::Color::Black); }

void GamePanel::render(sf::RenderWindow &window, const Game &game) {
  float cell_size = 10.0f * scale_factor; // Base 10
  float thickness = 20.0f * scale_factor;

  int rows = game.getRows();
  int cols = game.getCols();
  float grid_width_px = cols * cell_size;
  float grid_height_px = rows * cell_size;

  // bigX setup
  float diagonal = std::sqrt(grid_width_px * grid_width_px +
                             grid_height_px * grid_height_px);
  bigX[0].setSize(sf::Vector2f(diagonal, thickness));
  bigX[1].setSize(sf::Vector2f(diagonal, thickness));
  bigX[0].setOrigin(bigX[0].getSize().x / 2.0f, bigX[0].getSize().y / 2.0f);
  bigX[1].setOrigin(bigX[1].getSize().x / 2.0f, bigX[1].getSize().y / 2.0f);
  bigX[0].setPosition(grid_width_px / 2.0f, grid_height_px / 2.0f);
  bigX[1].setPosition(grid_width_px / 2.0f, grid_height_px / 2.0f);
  bigX[0].setRotation(45.0f);
  bigX[1].setRotation(-45.0f);
  bigX[0].setFillColor(sf::Color::Red);
  bigX[1].setFillColor(sf::Color::Red);

  bg.setSize(sf::Vector2f(grid_width_px, grid_height_px));

  window.draw(bg);

  // draw food
  sf::CircleShape foodShape(cell_size / 2.0f);
  foodShape.setFillColor(sf::Color::Red);
  auto food = game.getFoodPos();
  foodShape.setPosition(food.second * cell_size, food.first * cell_size);
  window.draw(foodShape);

  // if dead, draw bigX
  if (game.isDead()) {
    window.draw(bigX[0]);
    window.draw(bigX[1]);
  }

  // draw snake
  sf::CircleShape segment(cell_size / 2.0f);
  const auto &snake = game.getSnake();
  if (!snake.empty()) {
    auto it = snake.begin();
    segment.setFillColor(sf::Color::Magenta);
    segment.setPosition(it->second * cell_size, it->first * cell_size);
    window.draw(segment);
    ++it;
    segment.setFillColor(sf::Color::Blue);
    for (; it != snake.end(); ++it) {
      segment.setPosition(it->second * cell_size, it->first * cell_size);
      window.draw(segment);
    }
  }
}

StatsPanel::StatsPanel() {
  bg.setFillColor(sf::Color::White);
  font.loadFromFile("arial.ttf");

  snakeTypeText.setFont(font);
  snakeTypeText.setFillColor(sf::Color::Black);

  statsText.setFont(font);
  statsText.setFillColor(sf::Color::Black);

  elapsedTimeText.setFont(font);
  elapsedTimeText.setFillColor(sf::Color::Black);

  stepsTakenText.setFont(font);
  stepsTakenText.setFillColor(sf::Color::Black);

  avgCompTimeText.setFont(font);
  avgCompTimeText.setFillColor(sf::Color::Black);

  aStarTexture.loadFromFile("btn_astar.jpg");
  spriteAstar.setTexture(aStarTexture);

  bfsTexture.loadFromFile("btn_bfs.jpg");
  spriteBFS.setTexture(bfsTexture);
}

void StatsPanel::render(sf::RenderWindow &window, const Game &game) {
  float cell_size = 10.0f * scale_factor; // Base 10
  float char_size = 18.0f * scale_factor;
  float panel_height = 300.0f * scale_factor;
  float border_margin = 10.0f * scale_factor;
  // float left_margin = 10.0f * scale_factor;
  // float button_margin = 10.0f * scale_factor; // Space between text and
  // buttons

  int rows = game.getRows();
  int cols = game.getCols();
  float grid_width_px = cols * cell_size;
  float grid_height_px = rows * cell_size;

  // gui and stats panel
  bg.setSize(sf::Vector2f(grid_width_px, panel_height));
  bg.setPosition(0, grid_height_px);

  // Set character sizes
  snakeTypeText.setCharacterSize(static_cast<unsigned int>(char_size));
  statsText.setCharacterSize(static_cast<unsigned int>(char_size));
  elapsedTimeText.setCharacterSize(static_cast<unsigned int>(char_size));
  stepsTakenText.setCharacterSize(static_cast<unsigned int>(char_size));
  avgCompTimeText.setCharacterSize(static_cast<unsigned int>(char_size));

  // Set strings and initial positions (y only; x will be left_margin)
  Algorithm chosenAlgo = game.getAlgorithm();
  std::string snakeType = "None";
  if (chosenAlgo == Algorithm::BFS)
    snakeType = "BFS";
  else if (chosenAlgo == Algorithm::AStar)
    snakeType = "A* Search";
  snakeTypeText.setString("Snake Type: " + snakeType);
  snakeTypeText.setPosition(border_margin,
                            grid_height_px + 10.0f * scale_factor);

  statsText.setString("Food Eaten: " + std::to_string(game.getFoodsEaten()));
  statsText.setPosition(border_margin, grid_height_px + 50.0f * scale_factor);

  elapsedTimeText.setString(
      "Elapsed Time: " + std::to_string(game.getElapsedTime()) + " s");
  elapsedTimeText.setPosition(border_margin,
                              grid_height_px + 90.0f * scale_factor);

  stepsTakenText.setString("Steps Taken: " +
                           std::to_string(game.getStepsTaken()));
  stepsTakenText.setPosition(border_margin,
                             grid_height_px + 130.0f * scale_factor);

  avgCompTimeText.setString(
      "Avg Computation Time: " + std::to_string(game.getAvgCompTime()) + " ns");
  avgCompTimeText.setPosition(border_margin,
                              grid_height_px + 170.0f * scale_factor);

  // Compute max text width (after strings are set)
  float max_text_width = 0.0f;
  max_text_width =
      std::max(max_text_width, snakeTypeText.getGlobalBounds().width);
  max_text_width = std::max(max_text_width, statsText.getGlobalBounds().width);
  max_text_width =
      std::max(max_text_width, elapsedTimeText.getGlobalBounds().width);
  max_text_width =
      std::max(max_text_width, stepsTakenText.getGlobalBounds().width);
  max_text_width =
      std::max(max_text_width, avgCompTimeText.getGlobalBounds().width);

  // button scale and placement
  spriteAstar.setScale(scale_factor, scale_factor);
  spriteBFS.setScale(scale_factor, scale_factor);
  float btnWidth = spriteAstar.getGlobalBounds().width;
  float btnHeight = spriteAstar.getGlobalBounds().height;
  float button_x = grid_width_px - btnWidth - border_margin;
  spriteAstar.setPosition(button_x, grid_height_px + border_margin);
  spriteBFS.setPosition(button_x,
                        grid_height_px + btnHeight + 2 * border_margin);

  window.draw(bg);
  window.draw(snakeTypeText);
  window.draw(statsText);
  window.draw(elapsedTimeText);
  window.draw(stepsTakenText);
  window.draw(avgCompTimeText);
  window.draw(spriteAstar);
  window.draw(spriteBFS);
}

void UI::render(sf::RenderWindow &window, const Game &game) {
  gamePanel.render(window, game);
  statsPanel.render(window, game);
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
  } else if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (statsPanel.getAStarSprite().getGlobalBounds().contains(
              event.mouseButton.x, event.mouseButton.y)) {
        // game.setAlgorithm(Algorithm::AStar);
      } else if (statsPanel.getBFSSprite().getGlobalBounds().contains(
                     event.mouseButton.x, event.mouseButton.y)) {
        // game.setAlgorithm(Algorithm::BFS);
      }
    }
  }
}

// TODO
void UI::update(const Game &game) {}
