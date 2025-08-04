#include <Game.h>
#include <Graph.h>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

vector<pair<int, int>> bfsGetPath(const Graph<CellType> &graph,
                                  const deque<pair<int, int>> &snake,
                                  pair<int, int> start, pair<int, int> target) {
  int rows = graph.getRows();
  int cols = graph.getCols();

  // BFS setup
  queue<pair<int, int>> q;
  unordered_map<int, pair<int, int>>
      prev; // key: row*cols + col, value: predecessor
  vector<vector<bool>> visited(rows, vector<bool>(cols, false));

  auto toKey = [cols](int row, int col) { return row * cols + col; };

  q.push(start);
  visited[start.first][start.second] = true;
  prev[toKey(start.first, start.second)] = {-1, -1};

  bool found = false;
  while (!q.empty()) {
    auto curr = q.front();
    q.pop();

    if (curr == target) {
      found = true;
      break;
    }

    auto neighbors = graph.getNodeNeighbors(curr.first, curr.second);
    for (const auto &nei : neighbors) {
      int ny = nei.first, nx = nei.second;
      if (ny < 0 || ny >= rows || nx < 0 || nx >= cols)
        continue;
      if (visited[ny][nx])
        continue;
      // avoid snake body (except tail)
      if (find(snake.begin(), snake.end(), make_pair(ny, nx)) != snake.end())
        continue;
      // avoid obstacles
      if (graph.getMatrixNode(ny, nx) == CellType::Obstacle)
        continue;

      visited[ny][nx] = true;
      q.push({ny, nx});
      prev[toKey(ny, nx)] = curr;
    }
  }

  if (!found)
    return {};

  // reconstruct path
  vector<pair<int, int>> path;
  auto curr = target;
  while (curr != start) {
    path.push_back(curr);
    curr = prev[toKey(curr.first, curr.second)];
  }
  reverse(path.begin(), path.end());
  return path;
}
