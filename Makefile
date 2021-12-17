CXX = g++-11
FLAG = -std=c++11 -pthread

SRC = src

SOURCE = $(wildcard ${SRC}/*.cpp)

OUT = main

.PHONY: default, clean

default: ${SRC}/*
	${CXX} ${FLAG} ${SOURCE} -o ${OUT}

clean:
	$(shell rm ${OUT})
