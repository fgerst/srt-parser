CC=gcc
CFLAGS=-I.

srt_parser: main.c src/args.c src/cola.c src/subs.c src/lista.c
	$(CC) -o srt_parser main.c src/args.c src/cola.c src/subs.c src/lista.c