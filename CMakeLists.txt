cmake_minimum_required(VERSION 3.11)
project(Billiard)

set (CMAKE_C_STANDARD 99)

# Set release mode
set(CMAKE_BUILD_TYPE Debug)

# Setting raylib parameters 
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games
set(OPENGL_VERSION "1.1" CACHE STRING "" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/raylib)

add_executable(game src/main.c src/piece_sprites.h src/move_legal_checker.c src/move_legal_checker.h src/board_and_pieces.h)

target_link_libraries(
        game PRIVATE raylib
)