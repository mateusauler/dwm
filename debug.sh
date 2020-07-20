#!/bin/sh

pgrep Xephyr || Xephyr -ac -noreset -screen 1600x900 :1 &
export DISPLAY=":1"
make
if [ $# -eq 1 ] && [ "$1" = "run" ]
then
	./dwm
else
	gdb ./dwm
fi
