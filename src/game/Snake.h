#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <queue>
#include <string>

using namespace std;

// this is a struct for each segment of the snake. The snakes will be added into
// a queue to move the snake, the top segment will have it's position updated
struct SnakePart {
private:
  sf::CircleShape segment;
  // sf::RectangleShape segment(sf::Vector2f(25.f, 25.f));

public:
  float xPos;
  float yPos;
  SnakePart(float initialRadius = 10.f) : segment(initialRadius) {
    segment.setFillColor(sf::Color::Blue);
  }
  void setPos(float x, float y) {
    segment.setPosition(x, y);
    xPos = x;
    yPos = y;
  }
  void updateXpos(float add) {
    xPos += add;
    setPos(xPos, yPos);
  }
  void updateYpos(float add) {
    yPos += add;
    setPos(xPos, yPos);
  }
  void setColor() { segment.setFillColor(sf::Color::Magenta); }
  void setColor2() { segment.setFillColor(sf::Color::Cyan); }
  sf::CircleShape getCircle() { return segment; }
};

bool SnakeOnGrid(queue<SnakePart> &snake, int x, int y);

void moveSnake(queue<SnakePart> &snake, string dirrection);

void addPart(queue<SnakePart> &snake, string dirrection);

bool Died(queue<SnakePart> &snake);

void eat(queue<SnakePart> &snake, SnakePart &food, int speed,
         string dirrection);
