#
#	Makefile for Intel9.1 compiler
#
BHOME	=$(PWD)
#
CC	= icpc
#
#
COMP	= -qopenmp
FOMP	= -qopenmp
COPTS	= -Ofast -xHOST -fno-alias -qopt-zmm-usage=high
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

