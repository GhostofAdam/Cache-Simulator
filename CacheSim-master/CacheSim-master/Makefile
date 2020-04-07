CC := g++
RM := rm -f
LIBS =
TARGET = main
OBJS = main.o base.o CalcInfo.o CreateCache.o FileTest.o GetInput.o InitVariables.o LRU.o PrintOutput.o

all: $(TARGET)

clean:
	$(RM)  *.exe *.o

main: ${OBJS}
	$(CC) -o main ${OBJS} -Wall

main.o: base.h
	$(CC) -c main.cpp -Wall

base.o: base.h
	$(CC) -c base.cpp -Wall

CalcInfo.o: base.h
	$(CC) -c CalcInfo.cpp -Wall

CreateCache.o: base.h
	$(CC) -c CreateCache.cpp -Wall
	
FileTest.o: base.h
	$(CC) -c FileTest.cpp -Wall

GetInput.o: base.h
	$(CC) -c GetInput.cpp -Wall

InitVariables.o: base.h
	$(CC) -c InitVariables.cpp -Wall

LRU.o: base.h
	$(CC) -c LRU.cpp -Wall

PseudoLRU.o: PseudoLRU.h
	$(CC) -c PseudoLRU.cpp -Wall

PrintOutput.o: base.h
	$(CC) -c PrintOutput.cpp -Wall

_bitset.o: _bitset.h
	$(CC) -c _bitset.cpp -Wall

LRUStack.o: LRUStack.h _bitset.h
	$(CC) -c LRUStack.cpp -Wall

