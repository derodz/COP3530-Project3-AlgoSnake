#pragma once
#include <Game.h>
#include <SFML/Graphics.hpp>

extern float scale_factor;

class GamePanel {
  sf::RectangleShape bg;
  sf::CircleShape segment;
  sf::CircleShape foodShape;
  sf::RectangleShape bigX[2];

public:
  GamePanel();
  void render(sf::RenderWindow &window, const Game &game, float cellSize);
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
  sf::Sprite spriteAstar;
  sf::Sprite spriteBFS;

public:
  StatsPanel();
  void render(sf::RenderWindow &window, const Game &game, float cellSize,
              float panelHeight);
  sf::Sprite getAStarSprite() const { return spriteAstar; }
  sf::Sprite getBFSSprite() const { return spriteBFS; }
};

class UI {
  GamePanel gamePanel;
  StatsPanel statsPanel;

  float cellSize;
  float panelHeight;

public:
  UI(float cellSize, float panelHeight);
  void render(sf::RenderWindow &window, const Game &game);
  void handleEvent(const sf::Event &event, Game &game);
};
