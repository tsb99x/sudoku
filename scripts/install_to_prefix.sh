#!/bin/sh

cd ./lib
for dep in "sdl" "freetype" "sdl_ttf";
        do
                mkdir ./build
                cmake -S ./$dep -B ./build -DCMAKE_PREFIX_PATH=$(pwd)/../.install
                cmake --build ./build
                cmake --install ./build --config Debug --prefix ../.install
                rm -rf ./build
        done
