all: memgrind

memgrind: memgrind.c mymalloc.c
	gcc -std=c11 -o memgrind memgrind.c mymalloc.c

clean:
	rm -f memgrind