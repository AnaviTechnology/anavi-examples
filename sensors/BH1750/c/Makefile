CC=gcc
CFLAGS=-I.
DEPS =
OBJ = BH1750.o BH1750-example.o
EXTRA_LIBS=-li2c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

BH1750: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f BH1750 $(OBJ)
