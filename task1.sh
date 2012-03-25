#!/bin/bash
set -e

host=$1;
if [[ $host == '' ]]; then
	host=localhost;
fi;

FR=$2;
if [[ $FR -eq '' ]]; then
	FR=10;
fi;

D=$3;
if [[ $D -eq '' ]]; then
	D=0;
fi;

S=$4;
if [[ $S -eq '' ]]; then
	S=0;
fi;

X=$5;
if [[ $X -eq '' ]]; then
	X=($(ssh $host "DISPLAY=:$D.$S; export DISPLAY; xwininfo -root | awk '/^.*-geometry.*/ { match(\$2, \"^[0-9]+x\"); print(substr(\$2, RSTART, RLENGTH-1)); }';"));
fi;

Y=$6;
if [[ $Y -eq '' ]]; then
	Y=($(ssh $host "DISPLAY=:$D.$S; export DISPLAY; xwininfo -root | awk '/^.*-geometry.*/ { match(\$2, \"x[0-9]+\\+\"); print(substr(\$2, RSTART+1, RLENGTH-2)); }'"));
fi;

ssh $host "ffmpeg -f x11grab -r $FR -s ${X}x$Y -i :$D.$S -f avi pipe:1"|ffplay -f avi -
