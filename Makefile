CC = gcc
CFLAGS = -Wall -Werror -pthread -g
SDLFLAGS = `pkg-config sdl2 --libs`
DEPS = CHIP-8.h Platform.h
OBJECTS = CHIP-8.o Platform.o

PROJ = chip8

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(SDLFLAGS)

main: main.o $(OBJECTS)
	$(CC) -o $(PROJ) main.o $(OBJECTS) $(SDLFLAGS)

.PHONY: clean

clean:
	rm -rf $(PROJ) *.o
