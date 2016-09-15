CC=gcc
CFLAGS=-lncurses

build:
	$(CC) *.c -o android-monitor-cli $(CFLAGS)

run:
	./android-monitor-cli
