#!/bin/bash

#for l in $(seq -1 0.1 -0.1) $(seq 0.1 0.1 1) $(seq -0.09 0.01 0.09) ; do
#for l in $(seq 0 0.01 0.1) $(seq 0.2 0.1 1) $(seq 2 1 10) ; do
#for l in $(seq 0.3 0.1 1) ; do
#for e in $(seq 1 1 10) $(seq 0 0.1 0.9) $(seq -10 1 -1) ;do
for e in  $(seq -2 0.1 2) $(seq 3 1 10) ; do
#for l in $(seq -0.9 0.1 -0.1) ; do
#for l in 0.0 ; do
for V in 10000 ; do



./Cb Cfg-$e-$V.txt &


done;
done;

