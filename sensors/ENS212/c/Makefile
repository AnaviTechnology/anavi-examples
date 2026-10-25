CC=gcc
CFLAGS=-I. -Wall -Wextra
DEPS =
OBJ = ENS212.o
EXTRA_LIBS=-li2c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ENS212: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f ENS212 $(OBJ)
