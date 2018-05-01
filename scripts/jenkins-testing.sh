#!/bin/sh
xdotool search --name Deerportal.* 
WINDOWID=`xdotool search --name Deerportal.*`
xdotool click --window "$WINDOWID" 1

