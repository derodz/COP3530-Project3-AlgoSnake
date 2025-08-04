#include <Game.h>
#include <SFML/Graphics.hpp>
#include <Window.h>
#include <fstream>
using namespace std;

float scale_factor; // Defined here, computed below

int main() {
  // read config file
  ifstream configFile("game.config");
  if (!configFile.is_open()) {
    throw runtime_error("Failed to open game.config");
  }

  int cellCount = 0;
  configFile >> cellCount;
  if (cellCount == 0) {
    throw runtime_error("Invalid row count in game.config");
  }

  int frameRate = 0;
  configFile >> frameRate;
  if (frameRate == 0) {
    throw runtime_error("Invalid frame rate in game.config");
  }

  // Compute base sizes
  const int BASE_CELL_SIZE = 10;
  const int BASE_PANEL_HEIGHT = 300;
  int base_width = cellCount * BASE_CELL_SIZE;
  int base_height = cellCount * BASE_CELL_SIZE + BASE_PANEL_HEIGHT;

  // Get desktop resolution and compute scale (95% max to fit with margins,
  // increased from 90%)
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  float max_w = static_cast<float>(desktop.width) * 0.95f;
  float max_h = static_cast<float>(desktop.height) * 0.95f;
  scale_factor =
      std::min(1.0f, std::min(max_w / base_width, max_h / base_height));
  // Optional: Clamp to a minimum scale to avoid unreadable UI (e.g., if screen
  // is tiny) scale_factor = std::max(0.5f, scale_factor);

  // Apply scale to actual sizes
  float actual_cell_size = BASE_CELL_SIZE * scale_factor;
  float actual_panel_height = BASE_PANEL_HEIGHT * scale_factor;
  int width = static_cast<int>(cellCount * actual_cell_size);
  int height =
      static_cast<int>(cellCount * actual_cell_size + actual_panel_height);

  // init setup
  Game game(42, cellCount, cellCount);
  game.setAlgorithm(Algorithm::BFS);
  // game.setAlgorithm(Algorithm::AStar);

  UI ui;

  sf::RenderWindow window(sf::VideoMode(width, height), "AlgoSnake");
  window.setFramerateLimit(frameRate);

  cout << "Frame rate: " << frameRate << endl;
  cout << "Cell count: " << cellCount << endl;
  cout << "Window size: " << width << "x" << height
       << " (scale: " << scale_factor << ")" << endl;
  cout << "Game starting..." << endl;
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
