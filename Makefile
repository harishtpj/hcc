CC = gcc
SRC = hcc

comp: cg.c expr.c gen.c interp.c main.c scan.c tree.c
	$(CC) -o $(SRC) -g cg.c expr.c gen.c interp.c main.c scan.c tree.c

clean:
	rm -f $(SRC) *.o *.s out