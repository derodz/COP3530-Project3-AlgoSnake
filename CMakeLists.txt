cmake_minimum_required(VERSION 3.22)
project(AlgoSnake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-Werror -Wuninitialized")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.2 # if you run into issues, try updating your editor or downgrading to a lower version of catch v3
)
FetchContent_MakeAvailable(Catch2)
find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src/game)
add_subdirectory(src/ui)
add_subdirectory(src/dsa)
        
add_executable(Main src/main.cpp)
add_executable(Tests test/test.cpp)
        
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(Main PRIVATE game ui dsa sfml-graphics sfml-window
    sfml-system)

# comment everything below out if you are using CLion
include(CTest)
include(Catch)
catch_discover_tests(Tests)
