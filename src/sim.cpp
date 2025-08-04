#include <Game.h>
#include <iostream>

// int main() {
//   int choice = 0;
//   std::cout << "Enter 1 for BFS, 2 for A* Search: ";
//   std::cin >> choice;
//
//   if (choice != 1 && choice != 2) {
//     std::cerr << "Invalid choice. Exiting.\n";
//     return 1;
//   }
//
//   int grid_len = 0;
//   std::cout << "Enter the side length of the (square) grid. N = ";
//   std::cin >> grid_len;
//   if (grid_len <= 0) {
//     throw runtime_error("Invalid grid length in game.config");
//   }
//
//   unsigned seed = 42;
//   // unsigned seed = 9001;
//
//   Game game(seed, grid_len, grid_len);
//
//   Algorithm algo = (choice == 1) ? Algorithm::BFS : Algorithm::AStar;
//   game.reset(algo);
//
//   while (!game.isDead()) {
//     game.update();
//   }
//
//   std::cout << "Simulation complete. Check BFS_stats.csv, AStar_stats.csv, and "
//                "summary_stats.csv for results.\n";
//
//   return 0;
// }

int main() {
    int grid_len = 0;
    std::cout << "Enter the side length of the (square) grid. N = ";
    std::cin >> grid_len;
    if (grid_len <= 0) {
        throw std::runtime_error("Invalid grid length");
    }

    unsigned seed = 42;
    // unsigned seed = 9001;

    // run BFS simulation
    {
        Game gameBFS(seed, grid_len, grid_len);
        gameBFS.reset(Algorithm::BFS);
        while (!gameBFS.isDead()) {
            gameBFS.update();
        }
    }

    // run A* simulation
    {
        Game gameAStar(seed, grid_len, grid_len);
        gameAStar.reset(Algorithm::AStar);
        while (!gameAStar.isDead()) {
            gameAStar.update();
        }
    }

    std::cout << "Simulations complete for both BFS and A*. Check BFS_stats.csv, AStar_stats.csv, and "
                 "summary_stats.csv for results.\n";

    return 0;
}
