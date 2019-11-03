# Your Makefile here.
CC = clang
CFLAGS = -c -Wall -Wextra
LFLAGS = -Wall -Wextra

all: rumours test
clean:
	rm -f rumours test *.o

rumours: rumours.o
	${CC} ${LFLAGS} rumours.o -o rumours

test: testcase.o
	${CC} ${LFLAGS} testcase.o -o test -pthread