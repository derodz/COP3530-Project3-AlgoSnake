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
SET(SFML_DIR "C:/SFML-2.6.2-x64/lib/cmake/SFML")
SET(SFML_INCLUDE_DIRS "C:/SFML-2.6.2-x64/bin")
find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

add_subdirectory(src/game)
add_subdirectory(src/ui)
add_subdirectory(src/dsa)

add_executable(Main src/main.cpp)
add_executable(Tests test/test.cpp)
add_executable(Sim src/sim.cpp)

target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(Main PUBLIC game ui dsa sfml-graphics sfml-window sfml-system)
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

# bring SFML dlls into build folder
if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        file(GLOB SFML_DLLS "C:/SFML-2.6.2-x64/bin/*-d-2.dll")
    else()
        file(GLOB SFML_DLLS "C:/SFML-2.6.2-x64/bin/sfml-*-2.dll")
    endif()

    foreach(DLL ${SFML_DLLS})
        add_custom_command(TARGET Main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL}" $<TARGET_FILE_DIR:Main>)

        add_custom_command(TARGET Tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL}" $<TARGET_FILE_DIR:Tests>)
    endforeach()
endif()

catch_discover_tests(Tests)
