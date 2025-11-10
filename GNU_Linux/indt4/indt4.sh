#!/bin/bash
N=$1

for ((i=1; i<=N; i++)); do    
    for ((j=0; j<$((N - i)); j++)); do
        echo -n " "
    done
    
    for ((j=0; j<$((2 * i - 1)); j++)); do
        echo -n "*"
    done
    echo
done

echo

for ((i=N; i>=1; i--)); do
    for ((j=0; j<$((N - i)); j++)); do
        echo -n " "
    done
   
    for ((j=0; j<$((2 * i - 1)); j++)); do
        echo -n "*"
    done
    echo
done
