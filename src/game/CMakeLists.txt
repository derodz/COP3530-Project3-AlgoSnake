# uncomment below once files are added
file(GLOB_RECURSE SOURCES "*.cpp")
add_library(game STATIC ${SOURCES})
