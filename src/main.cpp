#include <Graph.h>
#include <Snake.h>
#include <iostream>

using namespace std;

int main() {
  cout << "Hello AlgoSnake!\n";

  /*vector<SnakePart> snake;
  SnakePart a;
  snake.push_back(a);
  snake[0].setPos(250, 250);
  SnakePart b;
  snake.push_back(b);
  snake[1].setPos(250, 270);
  SnakePart c;
  snake.push_back(c);
  snake[2].setPos(250, 290);*/

  queue<SnakePart> snake;
  SnakePart a;
  a.setPos(250, 250);
  a.setColor();
  snake.emplace(a);
  SnakePart b;
  b.setPos(250, 270);
  snake.emplace(b);
  SnakePart c;
  c.setPos(250, 290);
  snake.emplace(c);

  string dirrection = "None";
  int speed = 0;
  bool isDead = false;

  // starting user interface code inspired from an SFML project done in a
  // previous class set window
  sf::RenderWindow window(sf::VideoMode({(500), (500)}), "AlgoSnake");

  // create shape perappeters and buttons
  //  start button will start the ai snake
  sf::RectangleShape btnStart(sf::Vector2f(25.f, 25.f));
  btnStart.setFillColor(sf::Color::Green);
  btnStart.setPosition(10, 420);

  // reset button will restart the game if the snake dies
  sf::RectangleShape btnReset(sf::Vector2f(25.f, 25.f));
  btnReset.setFillColor(sf::Color::Red);
  btnReset.setPosition(10, 450);

  sf::RectangleShape dead(sf::Vector2f(800.f, 50.f));
  dead.setPosition(0, 0);
  dead.setFillColor(sf::Color::Red);
  dead.setRotation(45);

  sf::RectangleShape dead1(sf::Vector2f(800.f, 50.f));
  dead1.setPosition(500, 0);
  dead1.setFillColor(sf::Color::Red);
  dead1.setRotation(135);

  SnakePart food;
  food.setColor2();
  food.setPos(410, 300);

  while (window.isOpen()) {
    isDead = Died(snake);
    eat(snake, food, speed, dirrection);
    if (dirrection != "None" && speed >= 300 && !isDead) {
      moveSnake(snake, dirrection);
      speed = 0;
    }
    speed++;
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::MouseButtonPressed) {
        auto mousePosition = sf::Mouse::getPosition(window);
        if (btnStart.getGlobalBounds().contains(mousePosition.x,
                                                mousePosition.y)) {
          // clicked on button start the ai snake movement
          cout << "pressed start" << endl;
        } else if (btnReset.getGlobalBounds().contains(mousePosition.x,
                                                       mousePosition.y)) {
          // clicked on button rset the game
          cout << "pressed reset" << endl;
          while (!snake.empty())
            snake.pop();
          SnakePart a;
          a.setPos(250, 250);
          a.setColor();
          snake.emplace(a);
          SnakePart b;
          b.setPos(250, 270);
          snake.emplace(b);
          SnakePart c;
          c.setPos(250, 290);
          snake.emplace(c);

          string dirrection = "None";
          speed = 0;
        }
      } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up)
          dirrection = "up";
        else if (event.key.code == sf::Keyboard::Down)
          dirrection = "down";
        else if (event.key.code == sf::Keyboard::Right)
          dirrection = "right";
        else if (event.key.code == sf::Keyboard::Left)
          dirrection = "left";
        else
          dirrection = "None";
      }
    }

    window.clear();
    if (isDead) {
      window.draw(dead);
      window.draw(dead1);
    }
    window.draw(btnStart);
    window.draw(btnReset);
    window.draw(food.getCircle());
    auto tempSnake = snake;
    while (!tempSnake.empty()) {
      SnakePart tempPart = tempSnake.front();
      window.draw(tempPart.getCircle());
      tempSnake.pop();
    }
    window.display();
  }

  return 0;
}
