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

  UI ui;
  int cellSize = 0;
  configFile >> cellSize;
  if (cellSize == 0)
  {
    throw runtime_error("Invalid cell size in game.config");
  }
  ui.setCellSize(cellSize);

  int width = game.getCols() * ui.getCellSize();
  int height = game.getRows() * ui.getCellSize() + 250; // 250 for gui and stats panel

  sf::RenderWindow window(sf::VideoMode(width, height), "AlgoSnake");
  window.setFramerateLimit(frameRate);

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

    if (game.getAlgorithm() != Algorithm::None)
    {
      game.update();
    }

    window.clear();
    ui.render(window, game);
    window.display();
  }
  cout << "Game ended." << endl;
  return 0;
}
