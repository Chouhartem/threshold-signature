DBGFLAGS=-ggdb3
PREFIX=/usr
INCLUDES_RELIC=$(PREFIX)/include/relic
LIB_RELIC=$(PREFIX)/lib
CFLAGS=-I $(INCLUDES_RELIC) -L $(LIB_RELIC) -W -Wall -O3
CC=g++

all : main

main : main.cpp config.o threshold.o relic++.o polynomials.o
	$(CC) $(CFLAGS) -lrelic $^ -o $@

%.o : %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o
