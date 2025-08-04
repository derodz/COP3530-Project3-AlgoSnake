#pragma once
#include <Game.h>
#include <SFML/Graphics.hpp>

extern float scale_factor; // Defined in main.cpp

class GamePanel {
  sf::RectangleShape bg;
  sf::CircleShape segment;
  sf::CircleShape foodShape;
  sf::RectangleShape bigX[2];

public:
  GamePanel();
  void render(sf::RenderWindow &window, const Game &game);
};

class StatsPanel {
  sf::RectangleShape bg;
  sf::Font font;
  sf::Text snakeTypeText;
  sf::Text statsText;
  sf::Text elapsedTimeText;
  sf::Text stepsTakenText;
  sf::Text avgCompTimeText;
  sf::Texture aStarTexture;
  sf::Texture bfsTexture;
  sf::Texture aStarTexture_pressed;
  sf::Texture bfsTexture_pressed;
  sf::Sprite spriteAstar;
  sf::Sprite spriteBFS;

public:
  StatsPanel();
  void render(sf::RenderWindow &window, const Game &game);
  sf::Sprite getAStarSprite() const { return spriteAstar; }
  sf::Sprite getBFSSprite() const { return spriteBFS; }
};

class UI {
  GamePanel gamePanel;
  StatsPanel statsPanel;

public:
  void render(sf::RenderWindow &window, const Game &game);
  void handleEvent(const sf::Event &event, Game &game);
  void update(const Game &game);
};
