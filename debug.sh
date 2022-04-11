#!/bin/sh

pgrep Xephyr || Xephyr -br -ac -resizeable -noreset -screen 1600x900 :2 &
export DISPLAY=":2"
make
if [ $# -eq 1 ] && [ "$1" = "run" ]
then
	sleep 1
	./dwm &
else
	gdb ./dwm
fi
