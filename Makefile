CC = gcc
CFLAGS = -Wall -pedantic -ansi
OUTPUT = main.exe
all: main simplecalc supercalc numeric graphic
	$(CC) $(CFLAGS) -o $(OUTPUT) *o -lm
main:
	$(CC) $(CFLAGS) -c -o main.o main.c -lm
supercalc:
	$(CC) $(CFLAGS) -c -o supercalc.o supercalc.c -lm
simplecalc:
	
	$(CC) $(CFLAGS) -c -o simplecalc.o simplecalc.c -lm
numeric:
	$(CC) $(CFLAGS) -c -o numeric.o numeric.c -lm
graphic:
	$(CC) $(CFLAGS) -c -o graphic.o graphic.c -lm
clean:
	rm -rf *.o & rm $(OUTPUT)
