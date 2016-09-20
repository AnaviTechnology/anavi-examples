CC=gcc
CFLAGS=-I.
DEPS = 
OBJ1 = beep.o
OBJ2 = starwars.o
EXTRA_LIBS=-lwiringPi
CFLAGS=-std=c99

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

beep: $(OBJ1)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

starwars: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f beep $(OBJ1) starwars $(OBJ2)
