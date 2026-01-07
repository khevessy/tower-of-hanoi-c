CC=gcc
CFLAGS=-Wall -pedantic -O3
objects=hanoi.o

all: $(objects)
	$(CC) $(CFLAGS) $^ -o hanoi

$(objects): %.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o hanoi
