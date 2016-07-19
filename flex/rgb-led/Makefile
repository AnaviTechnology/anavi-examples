CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = rgb-led.o
EXTRA_LIBS=-lwiringPi

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rgb-led: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f rgb-led $(OBJ) 
