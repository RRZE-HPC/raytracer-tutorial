#
#	Makefile for GNU compiler
#
BHOME	=$(PWD)
#
CC	= g++
#
#
COMP	= -fopenmp
FOMP	= -fopenmp
COPTS	= -Ofast -march=native -fargument-noalias
FLONG	= -132
#
#
NOLINK= -c
#

all: raytrc.exe

raytrc.exe: main.o
	$(CC) $(COMP) -o raytrc.exe $<

%.o: %.cpp
	$(CC) $(COPTS) $(COMP) $(NOLINK) $<


clean:
	rm -f *.o *.exe *.mod

