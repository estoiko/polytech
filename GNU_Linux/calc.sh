#!/bin/bash
res=$1
shift 1
while [ $#  -gt 1 ]; do
    case $1 in
        +) res=`echo "scale=6;$res + $2" | bc`;;
    esac
    shift 2
done
echo "result - $res"

# bc .0 == 0 при проверке деления на ноль 