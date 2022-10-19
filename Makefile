CC = gcc

scanner: main.c scan.c
	$(CC) -o scanner -g main.c scan.c

clean:
	rm -f scanner *.o