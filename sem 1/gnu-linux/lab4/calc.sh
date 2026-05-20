#!/bin/bash
res=$1
shift 1
while [ $#  -gt 1 ]; do
    case $1 in
        +) res=`echo "scale=6;$res + $2" | bc`;;
		-) res=`echo "scale=6;$res - $2" | bc`;; 
		x) res=`echo "scale=6;$res * $2" | bc`;;
        /) 
			if [ `echo "$2 == 0" | bc` = "1" ]; then 
				res="d"
				break
			fi
			res=`echo "scale=6;$res / $2" | bc`;;
		*) res="!"; break;;
    esac
    shift 2
done

if [ $res = "!" ]; then
	echo "Неизвестная операция"
elif [ $res = "d" ]; then
 	echo "Ошибка: деление на 0"
else 	
	echo "result = $res"
fi 
