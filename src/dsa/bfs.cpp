#include "Graph.h"
#include "snake.cpp"
#include <vector>
#include <set>
#include <queue>
#include <stack>
using namespace std;

/// @brief This function uses a modified  BFS to find the shortest path to the food in a grid represented by a Graph.
/// Before adding a node to the path, it checks if the snake occupies the grid cell at the given coordinates.
/// Once adding the node, it also adds it and its predecessor to the path stack.
/// Finally, once the food is found, it converts the stack to a vector to return the path from the start to the food.
/// If no food is found, the snake will eventually die by running into a wall or itself since the BFS will have
/// traversed all possible paths.
/// @param graph
/// @param snake
/// @param startX
/// @param startY
/// @return
vector<pair<int, int>> GetShortestPath(Graph<bool> &graph, queue<snakePart> &snake, int startX, int startY)
{
    // Pair of pairs,
    // First pair: coordinates (x, y). The first on top of the stack is where the food was found, else the next step.
    // Second pair: coordinates (x, y) of the predecessor in the path.
    stack<pair<pair<int, int>, pair<int, int>>> path;
    queue<pair<int, int>> q;
    q.push({startX, startY});
    path.push({{startX, startY}, {-1, -1}});

    while (!q.empty())
    {
        auto coordinates = q.front();
        auto node = graph.getMatrixNode(coordinates.first, coordinates.second);

        if (node)
            // food is at node
            break;

        auto neighbors = graph.getNodeNeighbors(startY, startX);

        q.pop();

        for (const auto &neighbor : neighbors)
        {
            int ny = neighbor.first, nx = neighbor.second;

            if (!SnakeOnGrid(snake, nx, ny))
            {
                q.push({nx, ny});
                path.push({{nx, ny}, coordinates});
            }
        }
    }

    // Convert the stack path to a vector so the snake can follow it from beginning to end.
    vector<pair<int, int>> finalPath;
    while (!path.empty())
    {
        auto current = path.top();
        path.pop();

        // If the predecessor is -1, it means we reached the start of the path.
        if (current.second.first == -1 && current.second.second == -1)
            break;

        finalPath.push_back(current.first);
    }
    return finalPath;
}