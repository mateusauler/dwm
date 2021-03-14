#!/bin/sh

pgrep Xephyr || Xephyr -br -ac -resizeable -noreset -screen 1600x900 :1 &
export DISPLAY=":1"
make
if [ $# -eq 1 ] && [ "$1" = "run" ]
then
	sleep 1
	./dwm &
else
	gdb ./dwm
fi
