#include <dsa/Graph.h>
#include <ui/window.h>
#include <iostream>

int main() {
	Graph<int> testing(10, 10);

	std::vector<std::pair<int, int>> neighbors = testing.getNeighborCoords(4, 4);
	std::cout << "Neighbors of (4, 4):\n";
	for (const auto &pair : neighbors) {
		std::cout << "(" << pair.first << ", " << pair.second << ")\n";
	}

	neighbors = testing.getNeighborCoords(0, 0);
	std::cout << "Neighbors of (0, 0):\n";
	for (const auto &pair : neighbors) {
		std::cout << "(" << pair.first << ", " << pair.second << ")\n";
	}

	return test();
}
