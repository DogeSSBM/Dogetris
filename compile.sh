#!/bin/sh
clear
set -ex
output="main.out"
libraries="-lm -lSDL2 -lSDL2_ttf -lSDL2_gfx -lSDL2_image -lSDL2_net"
flags="-std=c23 -Wall -Wextra -Wpedantic -Werror -g"
# flags="-std=c23 -Wall -Wextra -Wpedantic -Werror -Ofast"
gcc main.c $flags $libraries -o $output
