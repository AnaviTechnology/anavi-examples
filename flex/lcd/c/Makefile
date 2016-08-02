CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = lcd.o
EXTRA_LIBS=-lwiringPi -lwiringPiDev

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lcd: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f lcd $(OBJ) 
