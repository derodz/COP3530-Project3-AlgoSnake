SET(SFML_DIR "C:\\SFML-2.6.2-x64\\lib\\cmake\\SFML")
find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_library(dsa STATIC ${SOURCES} ${HEADERS})

target_link_libraries(dsa PUBLIC sfml-graphics sfml-window
    sfml-system)
target_include_directories(dsa PUBLIC ${SFML_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/dsa ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/game)
