#include <Game.h>
#include <Graph.h>
#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

vector<pair<int, int>> aStarGetPath(const Graph<CellType> &graph,
                                    const deque<pair<int, int>> &snake,
                                    pair<int, int> start,
                                    pair<int, int> target) {
  int rows = graph.getRows();
  int cols = graph.getCols();

  // A* setup
  // lambda function for manhattan distance
  auto manhattan = [](pair<int, int> a, pair<int, int> b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
  };

  using pii = pair<int, int>;
  struct Node {
    pii pos;
    double score;
    bool operator>(const Node &other) const { return score > other.score; }
  };

  priority_queue<Node, vector<Node>, greater<Node>> pq;
  vector<vector<double>> scores(
      rows, vector<double>(cols, numeric_limits<double>::infinity()));
  unordered_map<int, pii> prev;

  // lambda for calculating to position efficiently
  auto toKey = [cols](int row, int col) { return row * cols + col; };

  scores[start.first][start.second] = 0.0;
  double fInit = manhattan(start, target);
  pq.push({start, fInit});

  while (!pq.empty()) {
    auto currNode = pq.top();
    pq.pop();
    pii curr = currNode.pos;

    // path reconstruction condition
    if (curr == target) {
      vector<pii> path;
      pii pos = target;
      while (pos != start) {
        path.push_back(pos);
        pos = prev[toKey(pos.first, pos.second)];
      }
      reverse(path.begin(), path.end());
      return path;
    }

    auto neighbors = graph.getNodeNeighbors(curr.first, curr.second);
    for (const auto &nei : neighbors) {
      int ny = nei.first, nx = nei.second;
      if (ny < 0 || ny >= rows || nx < 0 || nx >= cols)
        continue;
      // avoid snake body (except tail)
      if (find(snake.begin(), snake.end(), make_pair(ny, nx)) != snake.end())
        continue;
      // avoid obstacles
      if (graph.getMatrixNode(ny, nx) == CellType::Obstacle)
        continue;

      double hScore =
          scores[curr.first][curr.second] + 1.0; // uniform cost 1 per step
      if (hScore < scores[ny][nx]) {
        prev[toKey(ny, nx)] = curr;
        scores[ny][nx] = hScore;
        double huer = manhattan({ny, nx}, target);
        double score = hScore + huer;
        pq.push({{ny, nx}, score});
      }
    }
  }
  return {};
}
