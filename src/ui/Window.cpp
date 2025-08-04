#include <Window.h>

void GamePanel::render(sf::RenderWindow &window, const Game &game,
                       float cellSize) {
  float thickness = 20.0f * scale_factor;

  float grid_width_px = game.getCols() * cellSize;
  float grid_height_px = game.getRows() * cellSize;

  // draw bg
  bg.setSize(sf::Vector2f(grid_width_px, grid_height_px));
  window.draw(bg);

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

  // draw food
  sf::CircleShape foodShape(cellSize / 2.0f);
  foodShape.setFillColor(sf::Color::Red);
  auto food = game.getFoodPos();
  foodShape.setPosition(food.second * cellSize, food.first * cellSize);
  window.draw(foodShape);

  // if dead, draw bigX
  if (game.isDead()) {
    window.draw(bigX[0]);
    window.draw(bigX[1]);
  }

  // draw snake
  sf::CircleShape segment(cellSize / 2.0f);
  const auto &snake = game.getSnake();
  if (!snake.empty()) {
    auto it = snake.begin();
    segment.setFillColor(sf::Color::Magenta);
    segment.setPosition(it->second * cellSize, it->first * cellSize);
    window.draw(segment);
    ++it;
    segment.setFillColor(sf::Color::Blue);
    for (; it != snake.end(); ++it) {
      segment.setPosition(it->second * cellSize, it->first * cellSize);
      window.draw(segment);
    }
  }
}

StatsPanel::StatsPanel() {
  font.loadFromFile("arial.ttf");
  bg.setFillColor(sf::Color::White);

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

  avgNodesExploredText.setFont(font);
  avgNodesExploredText.setFillColor(sf::Color::Black);

  failureDistanceText.setFont(font);
  failureDistanceText.setFillColor(sf::Color::Black);

  aStarTexture.loadFromFile("btn_astar.png");
  aStarTexture_pressed.loadFromFile("btn_astar_pressed.png");
  spriteAstar.setTexture(aStarTexture);
  spriteAstar.setPosition(0, 0);

  bfsTexture.loadFromFile("btn_bfs.png");
  bfsTexture_pressed.loadFromFile("btn_bfs_pressed.png");
  spriteBFS.setTexture(bfsTexture);
  spriteBFS.setPosition(0, 0);
}

void StatsPanel::render(sf::RenderWindow &window, const Game &game,
                        float cellSize, float panelHeight) {
  float char_size = 24.0f * scale_factor;
  float border_margin = 10.0f * scale_factor;

  float grid_width_px = game.getCols() * cellSize;
  float grid_height_px = game.getRows() * cellSize;

  // gui and stats panel
  bg.setSize(sf::Vector2f(grid_width_px, panelHeight));
  bg.setPosition(0, grid_height_px);

  // set character sizes
  snakeTypeText.setCharacterSize(static_cast<unsigned int>(char_size));
  statsText.setCharacterSize(static_cast<unsigned int>(char_size));
  elapsedTimeText.setCharacterSize(static_cast<unsigned int>(char_size));
  stepsTakenText.setCharacterSize(static_cast<unsigned int>(char_size));
  avgCompTimeText.setCharacterSize(static_cast<unsigned int>(char_size));
  avgNodesExploredText.setCharacterSize(static_cast<unsigned int>(char_size));
  failureDistanceText.setCharacterSize(static_cast<unsigned int>(char_size));

  // set string text and initial positions
  float text_height = snakeTypeText.getGlobalBounds().height;

  Algorithm chosenAlgo = game.getAlgorithm();
  std::string snakeType = "None";
  if (chosenAlgo == Algorithm::BFS)
    snakeType = "BFS";
  else if (chosenAlgo == Algorithm::AStar)
    snakeType = "A* Search";
  snakeTypeText.setString("Snake Type: " + snakeType);
  snakeTypeText.setPosition(border_margin, grid_height_px + border_margin);

  statsText.setString("Food Eaten: " + std::to_string(game.getFoodsEaten()));
  statsText.setPosition(border_margin,
                        grid_height_px + text_height + 2 * border_margin);

  elapsedTimeText.setString(
      "Elapsed Time: " + std::to_string(game.getElapsedTime()) + " s");
  elapsedTimeText.setPosition(border_margin, grid_height_px + 2 * text_height +
                                                 3 * border_margin);

  stepsTakenText.setString("Steps Taken: " +
                           std::to_string(game.getStepsTaken()));
  stepsTakenText.setPosition(border_margin, grid_height_px + 3 * text_height +
                                                4 * border_margin);

  avgCompTimeText.setString(
      "Avg Computation Time: " + std::to_string(game.getAvgCompTime()) + " us");
  avgCompTimeText.setPosition(border_margin, grid_height_px + 4 * text_height +
                                                 5 * border_margin);

  avgNodesExploredText.setString("Avg Nodes Explored: " +
                                 std::to_string(game.getAvgNodesExplored()));
  avgNodesExploredText.setPosition(
      border_margin, grid_height_px + 5 * text_height + 6 * border_margin);

  std::string failStr =
      game.isDead() ? std::to_string(game.getFailureDistance()) : "N/A";
  failureDistanceText.setString("Failure Distance: " + failStr);
  failureDistanceText.setPosition(
      border_margin, grid_height_px + 6 * text_height + 7 * border_margin);

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
  window.draw(avgNodesExploredText);
  window.draw(failureDistanceText);
  window.draw(spriteAstar);
  window.draw(spriteBFS);
}

void UI::render(sf::RenderWindow &window, const Game &game) {
  gamePanel.render(window, game, cellSize);
  statsPanel.render(window, game, cellSize, panelHeight);
}

void UI::handleEvent(const sf::Event &event, Game &game) {
  bool buttonPressed = false;
  if (event.type == sf::Event::KeyPressed &&
      game.getAlgorithm() == Algorithm::None) {
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
    if (statsPanel.getAStarSprite().getGlobalBounds().contains(
            event.mouseButton.x, event.mouseButton.y)) {
      buttonPressed = true;
      statsPanel.pressAStarButton();
      statsPanel.pressedButton = StatsPanel::ButtonType::AStar;
      game.reset(Algorithm::AStar);
    } else if (statsPanel.getBFSSprite().getGlobalBounds().contains(
                   event.mouseButton.x, event.mouseButton.y)) {
      buttonPressed = true;
      statsPanel.pressBFSButton();
      statsPanel.pressedButton = StatsPanel::ButtonType::BFS;
      game.reset(Algorithm::BFS);
    }
  } else if (event.type == sf::Event::MouseButtonReleased) {
    if (statsPanel.pressedButton == StatsPanel::ButtonType::AStar) {
      statsPanel.depressAStarButton();
    } else if (statsPanel.pressedButton == StatsPanel::ButtonType::BFS) {
      statsPanel.depressBFSButton();
    }
    statsPanel.pressedButton = StatsPanel::ButtonType::None;
  }

  if (!buttonPressed) {
    statsPanel.depressAStarButton();
    statsPanel.depressBFSButton();
  }
}
