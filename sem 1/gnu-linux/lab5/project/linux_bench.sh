#!/usr/bin/env bash

OPTS_LIST=(
  "-O0" 
  "-Os" 
  "-O1" 
  "-O2" 
  "-O3"
  "-O2 -march=native" 
  "-O3 -march=native"
  "-O2 -march=native -funroll-loops" 
  "-O3 -march=native -funroll-loops"
)

for OPTS in "${OPTS_LIST[@]}"; do
  echo "Compiling with $OPTS ..." >&2
  g++ -std=c++14 $OPTS main.cpp String.cpp Strutils.cpp -o result

  echo 
  echo "TIME(s):"
  time ./result

  echo 
  echo "SIZE(bytes):"
  echo
  SIZE=$(du -sb result)
  echo "$SIZE"

  echo 
  echo "------------------------------------------------" 
  echo
done