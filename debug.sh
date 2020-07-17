#!/bin/bash

pgrep Xephyr || Xephyr -ac -noreset -screen 1600x900 :1 &
export DISPLAY=":1"
make clean
make
gdb ./dwm
