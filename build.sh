#!/usr/bin/env bash

set -xe

CFLAGS="-std=c99 -Wall -Wextra -Werror $(pkg-config --cflags raylib)"
LIBS="$(pkg-config --libs raylib)"

gcc $LIBS -g -o main ./main.c $CFLAGS
