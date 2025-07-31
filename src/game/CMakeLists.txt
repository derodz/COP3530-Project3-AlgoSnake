# uncomment below once files are added
# file(GLOB_RECURSE SOURCES "*.cpp")
# add_library(game STATIC ${SOURCES})
SET(SFML_DIR "C:\\SFML-2.6.2-x64\\lib\\cmake\\SFML")
find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
add_library(game
    food.cpp
    snake.cpp
    grid.cpp
)
target_link_libraries(game PUBLIC sfml-graphics sfml-window
    sfml-system)
target_include_directories(game PUBLIC ${SFML_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src/game)
