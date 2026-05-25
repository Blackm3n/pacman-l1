cmake_minimum_required(VERSION 3.10)
project(PacMan_SFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

set(SOURCES
    src/main.cpp
    src/affichage_sfml.cc
    src/Board.cpp
    src/Pacman.cpp
    src/Ghost.cpp
    src/Pathfinder.cpp
    src/Fruit.cpp
)

add_executable(pacman ${SOURCES})
target_link_libraries(pacman sfml-graphics sfml-window sfml-system)