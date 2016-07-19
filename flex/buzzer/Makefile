CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = beep.o
EXTRA_LIBS=-lwiringPi

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

beep: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f beep $(OBJ) 
