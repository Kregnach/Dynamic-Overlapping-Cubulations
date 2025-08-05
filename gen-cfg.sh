#!/bin/bash

#for l in $(seq -1 0.1 1) $(seq -0.09 0.01 0.09) ; do
#for l in $(seq 0 0.01 0.1) $(seq 0.2 0.1 1) $(seq 2 1 10) ; do
#for e in $(seq 1 1 10) $(seq 0 0.1 0.9) $(seq -10 1 -1)  ; do
for e in  $(seq -2 0.1 2) 3 4 5 6 7 8 9 10; do 
#for e in 5 ; do
#for e in 150 200 400 800 ; do
for V in 10000 ; do


rand=$RANDOM

cat > Cfg-$e-$V.txt << EOF
seed			${rand}
A			150
V			$V
startsize		2
lambda			0.0
alpha			$e
epsilon			0.001
steps			10000
thermal			100000
sweeps			0
name			$e-$V
EOF


done;
done;
#