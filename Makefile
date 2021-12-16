CXX = g++-11
FLAG = -std=c++11 -pthread

LIB = lib
SRC = src

SOURCE = $(wildcard ${LIB}/*.cpp ${SRC}/*.cpp)

OUT = main

.PHONY: default, clean

default: ${LIB}/* ${SRC}/*
	${CXX} ${FLAG} ${SOURCE} -o ${OUT}

clean:
	$(shell rm ${OUT})
