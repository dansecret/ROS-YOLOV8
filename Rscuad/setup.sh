#!/bin/bash

echo "
# 
# RSCUAD TEAM
# 

INC_Utils 	=-I utilities
INC_Serial	=-I serial
INC_Com		=-I communication
" > makefile

VAR=path
path+=$(pwd)'/testing.cpp\'$'\n\t'
while read line; do    
    path+=$(pwd)'/'$line'\'$'\n\t'
done < path.txt

echo "src = $path" >> makefile

echo '
all : 
	g++ -C $(INC_Utils) $(INC_Serial) $(INC_Com)  $(src) -o test -pthread `pkg-config --cflags --libs opencv` -std=c++11

cv4 : 
	g++ -C $(INC_Utils) $(INC_Serial) $(INC_Com)  $(src) -o test -pthread `pkg-config --cflags --libs opencv4` -std=c++11

check:
	valgrind --leak-check=full --show-leak-kinds=all --log-file=memoryleak.rpt ./test

clean:
	rm test
' >> makefile
