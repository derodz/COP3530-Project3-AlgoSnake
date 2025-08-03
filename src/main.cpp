#include <Game.h>
#include <SFML/Graphics.hpp>
#include <Window.h>
#include <fstream>
using namespace std;

int main()
{
  // read config file
  ifstream configFile("game.config");
  if (!configFile.is_open())
  {
    throw runtime_error("Failed to open game.config");
  }

  int cellCount = 0;
  configFile >> cellCount;
  if (cellCount == 0)
  {
    throw runtime_error("Invalid row count in game.config");
  }

  int frameRate = 0;
  ;
  configFile >> frameRate;
  if (frameRate == 0)
  {
    throw runtime_error("Invalid frame rate in game.config");
  }

  // init setup
  Game game(42, cellCount, cellCount);
  game.setAlgorithm(Algorithm::BFS);
  // game.setAlgorithm(Algorithm::AStar);

  UI ui;

  int width = game.getCols() * CELL_SIZE;
  int height = game.getRows() * CELL_SIZE + 300; // 300 for gui and stats panel

  sf::RenderWindow window(sf::VideoMode(width, height), "AlgoSnake");
  window.setFramerateLimit(frameRate);

  // // NOTE: for manual control
  // sf::Clock clock;
  // float timer = 0.0f;
  // float baseDelay = 0.15f;
  // float minDelay = 0.01f;
  // float delay = baseDelay;

  cout << "Frame rate: " << frameRate << endl;
  cout << "Cell count: " << cellCount << endl;
  cout << "Window size: " << width << "x" << height << endl;
  cout << "Game starting..." << endl;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
      ui.handleEvent(event, game);
    }

    // // NOTE: for manual control
    // if (!game.isDead()) {
    //   float time = clock.restart().asSeconds();
    //   timer += time;
    //
    //   if (timer > delay) {
    //     game.update();
    //     timer = 0.0f;
    //
    //     // delay based on food eaten (snake length - initial 3)
    //     int foodEaten = static_cast<int>(game.getSnake().size()) - 3;
    //     delay = std::max(minDelay, baseDelay - 0.01f * foodEaten);
    //   }
    // } else {
    //   // TODO
    // }
    game.update();

    window.clear();
    ui.render(window, game);
    window.display();
  }
  cout << "Game ended." << endl;
  return 0;
}
