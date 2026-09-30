#!/bin/bash
set -e
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined main.cpp -o main
./main
