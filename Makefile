CC = cc
CFLAGS = -Wall -pedantic
TARGET = build/esh

esh: $(wildcard src/*.c)
	$(CC) $(CFLAGS) -o $(TARGET) $(wildcard src/*.c)

clean:
	rm -f $(TARGET)