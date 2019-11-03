CC = clang
CFLAGS = -c -Wall -Wextra
LFLAGS = -Wall -Wextra

all: faro_shuffle
clean: 
	rm faro_shuffle.o functions.o

faro_shuffle: faro_shuffle.o functions.o print_faro_val.o
	${CC} ${LFLAGS} faro_shuffle.o functions.o print_faro_val.o -o faro_shuffle

faro_shuffle.o: faro_shuffle.c print_faro_val.h
	${CC} ${CFLAGS} faro_shuffle.c -o faro_shuffle.o

functions.o:  functions.c faro.h
	${CC} ${CFLAGS} functions.c -o functions.o	