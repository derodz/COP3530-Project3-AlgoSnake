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
  GamePanel() { bg.setFillColor(sf::Color::Black); };
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
  sf::Texture aStarTexture_pressed;
  sf::Texture bfsTexture_pressed;
  sf::Sprite spriteAstar;
  sf::Sprite spriteBFS;

public:
  StatsPanel();
  void render(sf::RenderWindow &window, const Game &game, float cellSize,
              float panelHeight);
  sf::Sprite getAStarSprite() const { return spriteAstar; }
  sf::Sprite getBFSSprite() const { return spriteBFS; }
  void pressAStarButton() { spriteAstar.setTexture(aStarTexture_pressed); }
  void pressBFSButton() { spriteBFS.setTexture(bfsTexture_pressed); }
  void depressAStarButton() { spriteAstar.setTexture(aStarTexture); }
  void depressBFSButton() { spriteBFS.setTexture(bfsTexture); }

  enum class ButtonType { None, AStar, BFS };
  ButtonType pressedButton = ButtonType::None;
};

class UI {
  GamePanel gamePanel;
  StatsPanel statsPanel;

  float cellSize;
  float panelHeight;

public:
  UI(float cellSize, float panelHeight)
      : cellSize(cellSize), panelHeight(panelHeight) {}
  void render(sf::RenderWindow &window, const Game &game);
  void handleEvent(const sf::Event &event, Game &game);
};
