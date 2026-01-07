#!/bin/bash


for e in 1.2 ; do
for V in 1000 ; do
for A in 2400 ; do

for k in $(seq -2 0.4 2) ; do

for l in -0.4 0 0.4 ; do

name=$k-$l-$A
#name2=2.0-$l-$A
#name=$k-$e-$V
rand=$RANDOM

cat > Cfg-$name.txt << EOF
seed			${rand}
tuneAV			0
A			$A
V			$V
startsize		10
lambda			$l
alpha			$e
kappa			$k
epsilon			0.002
initialsteps		$V
steps			${V}
thermal			20000
sweeps			0
name			$name
inname			conf-$name.dat
outname			conf-$name.dat
fromfile		0
badjacency		1
cadjacency		1
cdensity		1
neckstat		0
rhist			0
EOF

done;
done;
done;
done;
done;
