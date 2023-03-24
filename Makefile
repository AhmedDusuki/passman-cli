.PHONY: all clean

#Use GNU compiler
CC = g++

all: main

main:
	$(CC) main.cpp libargon2.a -o pman

clean:
	rm -f pman