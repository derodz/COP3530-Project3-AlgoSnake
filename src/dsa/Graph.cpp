// #include <dsa/Graph.h>
// #include <sstream>
// #include <stdexcept>
//
// template <typename T> void Graph<T>::setMatrixDims(int r, int c) {
//   rows = r;
//   cols = c;
//   adjMatrix.resize(r);
//   for (auto &row : adjMatrix) {
//     row.resize(c);
//   }
// }
//
// template <typename T> T &Graph<T>::getMatrixNode(int y, int x) {
//   if (y < 0 || x < 0 || y >= rows || x >= cols) {
//     std::stringstream ss;
//     ss << "Coordinate {" << y << ", " << x << "} is outside of matrix";
//     throw std::out_of_range(ss.str());
//   }
//   return adjMatrix[y][x];
// }
//
// template <typename T> const T &Graph<T>::getMatrixNode(int y, int x) const {
//   if (y < 0 || x < 0 || y >= rows || x >= cols) {
//     std::stringstream ss;
//     ss << "Coordinate {" << y << ", " << x << "} is outside of matrix";
//     throw std::out_of_range(ss.str());
//   }
//   return adjMatrix[y][x];
// }
//
// template <typename T>
// std::vector<std::pair<int, int>> Graph<T>::getNeighborCoords(int y, int x) {
//   std::vector<std::pair<int, int>> neighbors;
//   for (const auto &step : STEPS) {
//     int dy = step.first, dx = step.second;
//     int ny = y + dy, nx = x + dx;
//     if (ny >= 0 && nx >= 0 && ny < rows && nx < cols) {
//       neighbors.push_back({ny, nx});
//     }
//   }
//   return neighbors;
// }
