cmake_minimum_required(VERSION 3.22)
project(AlgoSnake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-Werror -Wuninitialized")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2 # if you run into issues, try updating your editor or downgrading to a lower version of catch v3
)
FetchContent_MakeAvailable(Catch2)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
SET(SFML_DIR "C:/SFML-2.6.2-x64/lib/cmake/SFML")
link_directories("C:/SFML-2.6.2-x64/lib")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

add_subdirectory(src/game)
add_subdirectory(src/ui)
add_subdirectory(src/dsa)

add_executable(Main src/main.cpp)
add_executable(Tests test/test.cpp)
add_executable(Sim src/sim.cpp)

target_compile_definitions(Main PRIVATE SFML_STATIC)
target_compile_definitions(Tests PRIVATE SFML_STATIC)

set(SFML_STATIC_LIBS
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    freetype
    opengl32
    winmm
    gdi32
    user32
)

target_link_libraries(Main PUBLIC game ui dsa ${SFML_STATIC_LIBS})
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain ${SFML_STATIC_LIBS})
target_link_libraries(Sim PUBLIC game dsa)

# comment everything below out if you are using CLion
include(CTest)
include(Catch)

# Copy fonts and images to the output directories of Main and Tests
foreach(target Main Tests)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/src/game.config" $<TARGET_FILE_DIR:${target}>)
    file(GLOB ASSET_FILES "${CMAKE_SOURCE_DIR}/assets/*")

    foreach(asset ${ASSET_FILES})
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${asset}" $<TARGET_FILE_DIR:${target}>)
    endforeach()
endforeach()

catch_discover_tests(Tests)
