# AlgoSnake

Class COP3530 (Summer 2025)
Group 72
Team Members: Hannah Dixon, Daniel Rodriguez, Ian Rodriguez-Page

---

![Demo](/AlgoSnake-Recording.gif?raw=true 'Demo')

This repo contains the C++ source code and assets for building and running our algorithmic snake game. Traditionally, the snake game is played by a user attempting to eat as much food on the board as possible. Eating food causes the length of the snake to grow and thereby taking up valuable space on the board. Eventually it becomes difficult to reach the next food because there are not many free spaces available to traverse. The game ends when all available spaces are taken up by the snake (theoretically possible) or the snake hits a wall or itself.

In the case of this project, the snake is being played by a pathfinding algorithm. Running the game allows you to choose either BFS or A*. The path finding algorithm will then continuously calculate the path to each food. If no path is found the snake will continuing moving until hopefully a new path is opened up (e.g. the snake's body is out of the way) The project is meant to compare the performance of two different path finding algorithms (BFS and A*) in a dense, unweighted graph.

### Prerequisites

---

- [Visual Studio Code](https://code.visualstudio.com/)
- [SFML Library (version 2.6.2, 64 bit)](https://www.sfml-dev.org/download/sfml/2.6.2/)
- [GCC 13.1.0 MinGW (SEH) - 64-bit](https://github.com/brechtsanders/winlibs_mingw/releases/download/13.1.0-16.0.5-11.0.0-msvcrt-r5/winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64msvcrt-11.0.0-r5.zip)

### Building

---

Provided that you have VS Code recognizing your tool chain, the only other step is to ensure that SFML is unzipped into the `C:\SFML-2.6.2-x64\` folder as specified the `CMakeLists.txt` file. All dependencies are built into the main or sim executable so no need to copy paste libraries. You can click the Build, Debug, or Launch from the CMAKE pane.
<br>
![Cmake Pic](/github-cmake.png?raw=true 'CMake Pane')
<br>

### Running

---

As mentioned, all dependencies are built in to the executable. The only thing you need to do is configure the game to your liking in the `game.config` file.

Line 1: Board size (e.g. a value of 20 will produce 20 x 20 game board)
Line 2: Frame rate (make sure it doesn't exceed your monitor's refresh rate)
Line 3: Cell Size (the size of each cell in the game board; useful for scaling when board size is large)

That's it! Just run the Main.exe and you can select which algorithm you want to play the snake. You can reset the game with a different algorithm once it finishes or view final stats.

![Gameplay](/snake-gameplay.png?raw=true 'Gameplay')
![Game Over](/snake-gameover.png?raw=true 'Game Over')

### Simulation Only

---

If you just want to simulate a large data set for exporting stats, you can run the `Sim.exe` file and specify the board size in the command line prompt. Once finished, the following stat files will be produced in the same folder:

- AStar_stats.csv
- BFS_stats.csv
- summary_stats.csv
