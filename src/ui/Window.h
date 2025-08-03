#pragma once
#include <Game.h>
#include <SFML/Graphics.hpp>

const int CELL_SIZE = 20;

class GamePanel
{
  sf::RectangleShape bg;
  sf::Font font;
  sf::Text snakeTypeText;
  sf::Text statsText;
  sf::Text elapsedTimeText;
  sf::Text stepsTakenText;
  sf::Text avgCompTimeText;
  sf::RectangleShape statsBg;
  sf::Texture aStarTexture;
  sf::Texture bFSTexture;
  sf::Texture aStarTexture_pressed;
  sf::Texture bFSTexture_pressed;
  sf::Sprite spriteAstar;
  sf::Sprite spriteBFS;
  sf::CircleShape segment;
  sf::CircleShape foodShape;
  sf::RectangleShape bigX[2];

public:
  GamePanel();
  sf::Sprite getAStarSprite() const { return spriteAstar; }
  sf::Sprite getBFSSprite() const { return spriteBFS; }
  void render(sf::RenderWindow &window, const Game &game);
};

class UI
{
  GamePanel gamePanel;

public:
  void render(sf::RenderWindow &window, const Game &game);
  void handleEvent(const sf::Event &event, Game &game);
  void update(const Game &game);
};
