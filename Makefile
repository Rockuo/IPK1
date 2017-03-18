SRC=genericFunctions.c client/main.c server/main.c
CFLAGS=-std=c11

all: $(SRC)
	gcc -o ftrestd genericFunctions.c server/main.c $(CFLAGS)
	gcc -o ftrest genericFunctions.c client/main.c $(CFLAGS)
