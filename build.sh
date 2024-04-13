#!/bin/sh

set -xe

gcc -Wall -Wextra -pedantic -g -ggdb2 -o gol src/main.c -lraylib -lm
