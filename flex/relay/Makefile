CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = relay.o
EXTRA_LIBS=-lwiringPi

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

relay: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f relay $(OBJ) 
