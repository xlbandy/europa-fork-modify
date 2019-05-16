#!/bin/bash

export PLASMA_HOME=$(pwd)

cpu_cores=(grep -c ^processor /proc/cpuinfo)

cd $PLASMA_HOME/src/PLASMA && 
    jam -dx \
        -sLOGGER_TYPE= \
        -sVARIANTS=OPTIMIZED \
        -sLIBRARIES=SHARED \
        -j1 \
        build

cd $PLASMA_HOME && 
     ant \
        -Djam.args="-dx" \
        -Djam.num.cores="$cpu_cores" \
        -Djam.variant=OPTIMIZED \
        -Djam.libraries=SHARED dist \
        dist

