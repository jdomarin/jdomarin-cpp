#!/bin/sh
g++ `sdl-config --cflags --libs` -lSDL_gfx -lSDL_mixer *.cpp -o SnakeTD
