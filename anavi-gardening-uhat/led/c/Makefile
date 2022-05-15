CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = blink.o
EXTRA_LIBS=-lpigpio

%.o: %.c $(DEPS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

blink: $(OBJ)
	$(CC) -Wall -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f blink $(OBJ) 
