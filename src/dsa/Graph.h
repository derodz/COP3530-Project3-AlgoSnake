#pragma once
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

// stored {up, down, left, right}
const std::pair<int, int> STEPS[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// TODO ALTER GRAPH TO SPECIFIC TYPE, NOT T
template <typename T> class Graph {
  int rows = 0, cols = 0;
  std::vector<std::vector<T>> adjMatrix;

public:
  Graph();
  Graph(int r, int c) : rows(r), cols(c), adjMatrix(r, std::vector<T>(c)) {};

  void setMatrixDims(int r, int c) {
    rows = r;
    cols = c;
    adjMatrix.resize(r);
    for (auto &row : adjMatrix) {
      row.resize(c);
    }
  };

  int getRows() const { return rows; };
  int getCols() const { return cols; };

  T getMatrixNode(int y, int x) {
    if (y < 0 || x < 0 || y >= rows || x >= cols) {
      std::stringstream ss;
      ss << "Coordinate {" << y << ", " << x << "} is outside of matrix";
      throw std::out_of_range(ss.str());
    }
    return adjMatrix[y][x];
  }
  const T getMatrixNode(int y, int x) const {
    if (y < 0 || x < 0 || y >= rows || x >= cols) {
      std::stringstream ss;
      ss << "Coordinate {" << y << ", " << x << "} is outside of matrix";
      throw std::out_of_range(ss.str());
    }
    return adjMatrix[y][x];
  }

  std::vector<std::pair<int, int>> getNodeNeighbors(int y, int x) {
    std::vector<std::pair<int, int>> neighbors;
    for (const auto &step : STEPS) {
      int dy = step.first, dx = step.second;
      int ny = y + dy, nx = x + dx;
      if (ny >= 0 && nx >= 0 && ny < rows && nx < cols) {
        neighbors.push_back({ny, nx});
      }
    }
    return neighbors;
  }
};
