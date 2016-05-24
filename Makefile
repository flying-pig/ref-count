CC = gcc
CFLAGS = -Wall -O -I. -Iref-count
LDFLAGS = -pthread
OBJS = test.o sp_counted_base_gcc_x86.o
all: test

test: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $^ $(CFLAGS)

%.o: ref-count/%.c
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -f $(OBJS) test

.PHONY: clean
