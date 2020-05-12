# Sudoku

Very simple implementation of classic Sudoku game as means to undestand SDL.

## Dependencies

Project is based on:

- SDL2
- SDL2_ttf
- freetype2

## Manual Compilation

If you do not use package manager (e.g., on Windows), use can refer to submodules.
To checkout submodules, use `git submodule update --checkout`.
After that, use `./scripts/install_to_prefix.sh`.
The script will compile all the dependencies in `lib` dir and install them into `.install` dir of project root.
Then, you can provide `.install` as CMake install prefix (e.g., `-DCMAKE_INSTALL_PREFIX=$(pwd)/.install`).
