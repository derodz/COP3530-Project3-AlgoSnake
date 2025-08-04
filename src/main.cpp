#include <Game.h>
#include <SFML/Graphics.hpp>
#include <Window.h>
#include <fstream>
using namespace std;

float scale_factor;

int main() {
  // read config file
  ifstream configFile("game.config");
  if (!configFile.is_open()) {
    throw runtime_error("Failed to open game.config");
  }

  int cell_count = 0;
  configFile >> cell_count;
  if (cell_count == 0) {
    throw runtime_error("Invalid row count in game.config");
  }

  int frame_rate = 0;
  configFile >> frame_rate;
  if (frame_rate == 0) {
    throw runtime_error("Invalid frame rate in game.config");
  }

  // define base sizes
  const int BASE_CELL_SIZE = 20;
  const int BASE_PANEL_HEIGHT = 300;
  int base_width = cell_count * BASE_CELL_SIZE;
  int base_height = cell_count * BASE_CELL_SIZE + BASE_PANEL_HEIGHT;

  // get desktop resolution and compute scale
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  float desktopResolution = 0.90f; // % of desktop resolution
  float max_w = static_cast<float>(desktop.width) * desktopResolution;
  float max_h = static_cast<float>(desktop.height) * desktopResolution;
  scale_factor =
      std::min(1.0f, std::min(max_w / base_width, max_h / base_height));

  // apply scale to actual sizes
  float actual_cell_size = BASE_CELL_SIZE * scale_factor;
  float actual_panel_height = BASE_PANEL_HEIGHT * scale_factor;
  int width = static_cast<int>(cell_count * actual_cell_size);
  int height =
      static_cast<int>(cell_count * actual_cell_size + actual_panel_height);

  // initial setup
  Game game(42, cell_count, cell_count);
  game.setAlgorithm(Algorithm::BFS);
  // game.setAlgorithm(Algorithm::AStar);

  UI ui(actual_cell_size, actual_panel_height);

  sf::RenderWindow window(sf::VideoMode(width, height), "AlgoSnake");
  window.setFramerateLimit(frame_rate);

  cout << "Frame rate: " << frame_rate << endl;
  cout << "Cell count: " << cell_count << endl;
  cout << "Window size: " << width << "x" << height
       << " (scale: " << scale_factor << ")" << endl;
  cout << "Game starting..." << endl;

	// main game loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      ui.handleEvent(event, game);
    }

    game.update();

    window.clear();
    ui.render(window, game);
    window.display();
  }
  cout << "Game ended." << endl;
  return 0;
}
