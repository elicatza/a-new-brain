#!/usr/bin/env bash

set -xe

CFLAGS="-std=c99 -Wall -Wextra -Werror $(pkg-config --cflags raylib)"
LIBS="$(pkg-config --libs raylib)"

gcc $LIBS -g -o a-new-brain ./main.c $CFLAGS
