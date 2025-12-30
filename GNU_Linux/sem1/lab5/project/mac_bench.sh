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

echo "Options;Time(s);Size(bytes)"

for OPTS in "${OPTS_LIST[@]}"; do
  echo "Compiling with $OPTS ..." >&2
  g++ -std=c++14 $OPTS main.cpp String.cpp Strutils.cpp -o result

  /usr/bin/time -f "%e" -o time_tmp.txt -- ./result >/dev/null 2>/dev/null
  TIME=$(cat time_tmp.txt)

  SIZE=$(stat -c%s result)
  echo "\"$OPTS\";$TIME;$SIZE"
done

rm -f time_tmp.txt result