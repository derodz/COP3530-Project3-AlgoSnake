# # uncomment below once files are added
# file(GLOB_RECURSE SOURCES "*.cpp")
# add_library(dsa STATIC ${SOURCES})
SET(SFML_DIR "C:\\SFML-2.6.2-x64\\lib\\cmake\\SFML")
find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
add_library(dsa
    a_star.cpp
    bfs.cpp
    Graph.h
)
target_link_libraries(dsa PUBLIC sfml-graphics sfml-window
    sfml-system)
target_include_directories(dsa PUBLIC ${SFML_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src/dsa ${CMAKE_SOURCE_DIR}/src/ui)
