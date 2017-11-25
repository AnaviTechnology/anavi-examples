CC=gcc
CFLAGS=-I. -Wall
LDFLAGS=-pthread
DEPS =
OBJ = demo.o command.o
EXTRA_LIBS=-lpigpio -lrt

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

demo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f demo $(OBJ)
