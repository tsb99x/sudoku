# Sudoku

Very simple implementation of classic Sudoku game as means to undestand SDL.

## Dependencies

Project is based on:

- SDL2
- SDL2_ttf
- freetype2

If you do not use package manager (e.g., on Windows), use `./scripts/install_to_prefix.sh`.
This script will compile all the dependencies in `lib` dir and install them into `.install` dir of project root.
Then, you can provide `.install` as CMake install prefix (e.g., `-DCMAKE_INSTALL_PREFIX=$(pwd)/.install`).
