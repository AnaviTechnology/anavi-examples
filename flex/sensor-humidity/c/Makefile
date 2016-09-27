CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = HTU21D.o rabbitmax-sensor-humidity.o
EXTRA_LIBS=-lwiringPi

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rabbitmax-sensor-humidity: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f rabbitmax-sensor-humidity $(OBJ) 
