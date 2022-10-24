CC = gcc
NAME = hcc
SRCS= cg.c decl.c expr.c gen.c main.c misc.c scan.c stmt.c sym.c tree.c

comp: $(SRCS)
	$(CC) -o $(NAME) -g $(SRCS)

clean:
	rm -f $(NAME) *.o *.s out

test: comp scratch.c
	./$(NAME) scratch.c
	$(CC) -o out out.s
	./out