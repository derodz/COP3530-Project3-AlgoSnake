#include <Snake.h>
#include <iostream>
#include <vector>

bool SnakeOnGrid(queue<SnakePart> &snake, int x, int y) {
  auto tempSnake = snake;
  while (!tempSnake.empty()) {
    auto tempPart = tempSnake.front();
    float gridXPos = x * 20.0f;
    float gridYPos = y * 20.0f;
    if (tempPart.xPos == gridXPos && tempPart.yPos == gridYPos)
      return true;
    tempSnake.pop();
  }
  return false;
}

void moveSnake(queue<SnakePart> &snake, string dirrection) {
  auto tempSnake = snake;

  // move pers one to new position but save old position
  auto tempPart = tempSnake.front();
  float lastX = tempPart.xPos;
  float lastY = tempPart.yPos;
  if (dirrection == "right")
    tempPart.updateXpos(20);
  if (dirrection == "left")
    tempPart.updateXpos(-20);
  if (dirrection == "up")
    tempPart.updateYpos(-20);
  if (dirrection == "down")
    tempPart.updateYpos(+20);
  tempSnake.pop();
  snake.pop();
  snake.emplace(tempPart);

  // loop through rest of snake and move to predisessors spot
  while (!tempSnake.empty()) {
    auto tempPart = tempSnake.front();
    tempSnake.pop();
    snake.pop();
    float newX = lastX;
    float newY = lastY;
    lastX = tempPart.xPos;
    lastY = tempPart.yPos;
    tempPart.setPos(newX, newY);
    snake.emplace(tempPart);
  }
}

void addPart(queue<SnakePart> &snake, string dirrection) {
  auto tempSnake = snake;

  // move pers one to new position but save old position
  auto tempPart = tempSnake.front();
  float lastX = tempPart.xPos;
  float lastY = tempPart.yPos;
  if (dirrection == "right")
    tempPart.updateXpos(20);
  if (dirrection == "left")
    tempPart.updateXpos(-20);
  if (dirrection == "up")
    tempPart.updateYpos(-20);
  if (dirrection == "down")
    tempPart.updateYpos(+20);
  tempSnake.pop();
  snake.pop();
  snake.emplace(tempPart);

  // loop through rest of snake and move to predisessors spot
  while (!tempSnake.empty()) {
    auto tempPart = tempSnake.front();
    tempSnake.pop();
    snake.pop();
    float newX = lastX;
    float newY = lastY;
    lastX = tempPart.xPos;
    lastY = tempPart.yPos;
    tempPart.setPos(newX, newY);
    snake.emplace(tempPart);
  }

  SnakePart a;
  a.setPos(lastX, lastY);
  snake.emplace(a);
}

bool Died(queue<SnakePart> &snake) {
  auto tempSnake = snake;
  vector<SnakePart> compare;
  while (!tempSnake.empty()) {
    auto tempPart = tempSnake.front();
    compare.push_back(tempPart);
    // check if goes past boarder
    if (tempPart.xPos > 500)
      return true;
    else if (tempPart.xPos < 0)
      return true;
    if (tempPart.yPos > 500)
      return true;
    else if (tempPart.yPos < 0)
      return true;
    tempSnake.pop();
  }

  for (int i = 0; i < compare.size(); i++) {
    // check if it touches itself
    for (int j = 0; j < compare.size(); j++) {
      if (i != j) {
        if (compare[i].xPos == compare[j].xPos &&
            compare[i].yPos == compare[j].yPos)
          return true;
      }
    }
  }
  return false;
}

void eat(queue<SnakePart> &snake, SnakePart &food, int speed,
         string dirrection) {
  // checks if food is eaten
  // changes food's location
  // adds body part
  auto head = snake.front();
  if (head.getCircle().getGlobalBounds().intersects(
          food.getCircle().getGlobalBounds())) {
    addPart(snake, dirrection);
    float y = food.yPos;
    float x = food.xPos;
    if (speed % 2 == 0) {
      y = y + 10000;
      int i = 0;
      while (y > 450) {
        y -= i;
        i++;
      }
      x += 2000;
      while (x > 450) {
        x -= i;
        i++;
      }
      if (y < 0)
        y = 250;
      if (x < 0)
        x = 250;
    } else {
      y = y - 10000;
      int i = 0;
      while (y < 20) {
        y += i;
        i++;
      }
      x -= 2000;
      while (x < 20) {
        x += i;
        i++;
      }
      if (y < 0)
        y = 250;
      if (x < 0)
        x = 250;
    }

    food.setPos(x, y);
    cout << "position set to: " << x << " " << y << endl;
  }
}
