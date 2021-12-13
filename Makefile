CPP11 = g++-11 -std=c++11
OPT = -pthread

SRC = src

OUT = main

.PHONY: default, clean

default: ${SRC}/*
	${CPP11} ${OPT} $^ -o ${OUT}

clean:
	$(shell rm ${OUT})
