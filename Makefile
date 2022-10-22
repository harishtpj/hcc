CC = gcc

parser: expr.c interp.c main.c scan.c tree.c
	$(CC) -o parser -g expr.c interp.c main.c scan.c tree.c

clean:
	rm -f parser *.o