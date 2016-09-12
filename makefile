CC=gcc
CFLAGS=-lncurses

build:
	$(CC) main.c -o android-monitor-cli $(CFLAGS)

run:
	./android-monitor-cli
