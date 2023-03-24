.PHONY: all clean

#Use GNU compiler
CC = g++

all: main

main:
	$(CC) main.cpp libargon2.so -o pman

clean:
	rm -f pman