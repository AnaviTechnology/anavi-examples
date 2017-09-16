CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = HTU21D.o HTU21D-example.o
EXTRA_LIBS=-lwiringPi

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

HTU21D: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f HTU21D $(OBJ) 
