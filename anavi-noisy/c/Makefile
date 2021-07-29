CC=gcc
CFLAGS=-I.
DEPS = 
OBJ1 = anavi-noisy.o
EXTRA_LIBS=-lwiringPi
CFLAGS=-std=c99

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

anavi-noisy: $(OBJ1)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f anavi-noisy $(OBJ1)
