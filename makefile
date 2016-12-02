GCC = gcc -g

all: shell.o parser.o
	$(GCC) -o shell.out shell.o parser.o

shell.o: shell.c
	$(GCC) -c shell.c

parser.o: parser.c
	$(GCC) -c parser.c

run: shell.out
	./shell.out

clean:
	rm -f *~ *.out *.o *.txt
