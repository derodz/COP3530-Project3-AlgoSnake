#include <Graph.h>
#include <Snake.h>
#include <cmath>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

/// @brief Uses A* search to find the shortest path to the food in the grid.
/// The grid is represented by Graph<bool>, where true indicates food.
/// Avoids cells occupied by the snake. Uses Manhattan distance as the
/// heuristic. If no path is found, returns an empty vector (snake may die by
/// hitting a wall or itself).
/// @param graph The game grid.
/// @param snake Queue representing the snake's body.
/// @param startX The starting column (x) of the snake's head.
/// @param startY The starting row (y) of the snake's head.
/// @return Vector of {row, col} pairs representing the path from start to food.
vector<pair<int, int>> aStarGetPath(Graph<bool> &graph, queue<SnakePart> &snake,
                                    int startX, int startY) {
  int rows = graph.getRows(), cols = graph.getCols();

  // set starting coords (y, x)
  pair<int, int> start = {startY, startX};

  // find food coords
  pair<int, int> target = {-1, -1};
  bool foodFound = false;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      // TODO NEED TO CHANGE TO SUPPORT GRID DATA TYPE
      if (graph.getMatrixNode(y, x)) {
        target = {y, x};
        foodFound = true;
        break;
      }
    }
    if (foodFound) {
      break;
    }
  }

  // setup for A*
  using pii = pair<int, int>; // pii = pair int int
  struct Node {
    pii pos;
    double score;
    bool operator>(const Node &other) const { return score > other.score; };
  };

  priority_queue<Node, vector<Node>, greater<Node>> pq;
  vector<vector<double>> scores(
      rows, vector<double>(cols, numeric_limits<double>::infinity()));
  vector<vector<pii>> prev;

  scores[startY][startX] = 0.0;
  double hInit = abs(start.first - target.first) +
                 abs(start.second - target.second); // heuristic at initial pos
	pq.push({start, hInit});

	while (!pq.empty()) {
		Node currNode = pq.top();
		pq.pop();
		pii pos = currNode.pos;

		// if target is reached, reconstruct path
		if (pos == target) {
			vector<pii> path;
			pii curr = target;
			while (curr != start) {
				path.push_back(curr);
				curr = prev[curr.first][curr.second];
			}
			path.push_back(start);
			reverse(path.begin(), path.end());
			return path;
		}

		// explore neighbors
		auto neighbors = graph.getNodeNeighbors(pos.first, pos.second);
		for (const auto &nei : neighbors) {
			int ny = nei.first, nx = nei.second;
			if (SnakeOnGrid(snake, nx, ny)) {
				continue;
			}
			double hScore = scores[pos.first][pos.second] + 1.0;
			if (hScore < scores[ny][nx]) {
				prev[ny][nx] = pos;
				scores[ny][nx] = hScore;
				double heur = abs(ny - target.first) + abs(nx + target.second);
				double score = heur + hScore;
				pq.push({{ny, nx}, score});
			}
		}
	}

  return {};
}
