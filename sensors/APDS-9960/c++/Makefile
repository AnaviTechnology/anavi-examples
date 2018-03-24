CC=g++
CPPFLAGS=-Wall -I.
DEPS =
OBJC = APDS9960.o color.o
OBJG = APDS9960.o gesture.o
EXTRA_LIBS=-lwiringPi

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

all: color gesture

color: $(OBJC)
	$(CC) -o $@ $^ $(CPPFLAGS) $(EXTRA_LIBS)

gesture: $(OBJG)
	$(CC) -o $@ $^ $(CPPFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f $(EXEC) $(OBJC) $(OBJ2)
