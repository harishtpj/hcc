CC = gcc
SRC = hcc

comp: cg.c expr.c gen.c main.c misc.c scan.c stmt.c tree.c
	$(CC) -o $(SRC) -g cg.c expr.c gen.c main.c misc.c scan.c stmt.c tree.c

clean:
	rm -f $(SRC) *.o *.s out

test: comp scratch
	./$(SRC) scratch
	$(CC) -o out out.s
	./out