CC=gcc
CFLAGS=-I.
DEPS =
OBJ = BMP180.o rabbitmax-sensor-temperature.o
EXTRA_LIBS=-lwiringPi -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rabbitmax-sensor-temperature: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f rabbitmax-sensor-temperature $(OBJ)
