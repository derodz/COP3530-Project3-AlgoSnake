#pragma once
#include <Game.h>
#include <SFML/Graphics.hpp>

class GamePanel
{
  int CELL_SIZE = 0;
  sf::RectangleShape bg;
  sf::Font font;
  sf::Text snakeTypeText;
  sf::Text statsText;
  sf::Text elapsedTimeText;
  sf::Text stepsTakenText;
  sf::Text avgCompTimeText;
  sf::RectangleShape statsBg;
  sf::Texture aStarTexture;
  sf::Texture bfsTexture;
  sf::Texture aStarTexture_pressed;
  sf::Texture bfsTexture_pressed;
  sf::Sprite spriteAstar;
  sf::Sprite spriteBFS;
  sf::CircleShape segment;
  sf::CircleShape foodShape;
  sf::RectangleShape bigX[2];

public:
  GamePanel(int cellSize);
  sf::Sprite getAStarSprite() const { return spriteAstar; }
  sf::Sprite getBFSSprite() const { return spriteBFS; }
  void pressAStarButton() { spriteAstar.setTexture(aStarTexture_pressed); }
  void pressBFSButton() { spriteBFS.setTexture(bfsTexture_pressed); }
  void depressAStarButton() { spriteAstar.setTexture(aStarTexture); }
  void depressBFSButton() { spriteBFS.setTexture(bfsTexture); }
  void render(sf::RenderWindow &window, const Game &game);
  int getCellSize() const { return CELL_SIZE; }
};

class UI
{
  GamePanel gamePanel;

public:
  UI(int cellSize) : gamePanel(cellSize) {}
  void render(sf::RenderWindow &window, const Game &game);
  void handleEvent(const sf::Event &event, Game &game);
  void update(const Game &game);
  int getCellSize() const { return gamePanel.getCellSize(); }
};
