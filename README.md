# RayChess

A chess game in C99 using the raylib graphics library

Goal: Beat the AI. 

How to play: Click the piece you want to move and then click a legal square to move it to. The game ends when either king is captured.

## Differences to FIDE standard:

- Pawns can only promote to queen
- No draw:
    - The game ends when the king is captured -> No stalemate
    - No draw by repetition rules

## Building (Linux/macOS) with cmake

1. Clone the repository using `git clone https://github.com/StefanLenz1/RayChess`
1. cd to the RayChess directory
`cd RayChess`
1. install the raylib git submodule using `git submodule update --init --recursive`
1. make a build folder and cd to it with `mkdir build && cd build`
1. setup cmake `cmake ..`
1. run `make` to compile the project
1. run the binary with `./game`

![chess image](https://user-images.githubusercontent.com/87242588/210804905-efb3f56d-26a3-4d7f-aa1b-f6a78066864a.png)