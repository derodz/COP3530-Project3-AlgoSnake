#pragma once
#include <Game.h>
#include <SFML/Graphics.hpp>

const int CELL_SIZE = 20;

class GamePanel {
  sf::RectangleShape bg;
  sf::CircleShape segment;
  sf::CircleShape foodShape;
  sf::RectangleShape bigX[2];

public:
  GamePanel();
  void render(sf::RenderWindow &window, const Game &game);
};

class UI {
  GamePanel gamePanel;

public:
  void render(sf::RenderWindow &window, const Game &game);
  void handleEvent(const sf::Event &event, Game &game);
  void update(const Game &game);
};
