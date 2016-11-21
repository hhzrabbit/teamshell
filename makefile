GCC = gcc -g

all: shell.o
	$(GCC) -o shell.out shell.o

shell.o: shell.c
	$(GCC) -c shell.c

run: shell.out
	./shell.out

clean:
	rm -f *~ *.out *.o *.txt
