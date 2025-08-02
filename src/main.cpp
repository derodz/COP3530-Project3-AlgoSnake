#include <Game.h>
#include <SFML/Graphics.hpp>
#include <Window.h>
#include <algorithm>

int main() {
  Game game(42, 20, 20);
  UI ui;

  int width = game.getCols() * CELL_SIZE;
  int height = game.getRows() * CELL_SIZE;

  sf::RenderWindow window(sf::VideoMode(width, height), "AlgoSnake");
  window.setFramerateLimit(30);

  sf::Clock clock;
  float timer = 0.0f;
  float baseDelay = 0.15f;
  float minDelay = 0.01f;
  float delay = baseDelay;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      ui.handleEvent(event, game);
    }

    if (!game.isDead()) {
      float time = clock.restart().asSeconds();
      timer += time;

      if (timer > delay) {
        game.update();
        timer = 0.0f;

        // delay based on food eaten (snake length - initial 3)
        int foodEaten = static_cast<int>(game.getSnake().size()) - 3;
        delay = std::max(minDelay, baseDelay - 0.01f * foodEaten);
      }
    } else {
      // TODO
    }

    window.clear();
    ui.render(window, game);
    window.display();
  }

  return 0;
}
