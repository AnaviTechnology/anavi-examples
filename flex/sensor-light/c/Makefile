CC=gcc
CFLAGS=-I.
DEPS =
OBJ = BH1750.o rabbitmax-light-sensor.o
EXTRA_LIBS=-lwiringPi

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rabbitmax-light-sensor: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f rabbitmax-light-sensor $(OBJ)
