CC=gcc
CFLAGS=-I.
DEPS =
OBJ = BMP180.o BMP180-example.o
EXTRA_LIBS=-lwiringPi -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

BMP180: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f BMP180 $(OBJ)
