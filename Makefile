CC = gcc
CFLAGS = -g -Wall -O -I. -Iref-count
LDFLAGS = -pthread
OBJS = test.o sp_counted_base_gcc_x86.o
SHARED_OBJS = test_shared_ref.o shared_count.o sp_counted_base_gcc_x86.o
WEAK_OBJS = test_weak_ref.o shared_count.o sp_counted_base_gcc_x86.o
all: test test_shared_ref test_weak_ref

test: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

test_shared_ref: $(SHARED_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

test_weak_ref: $(WEAK_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $^ $(CFLAGS)

%.o: ref-count/%.c
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -f *.o test test_shared_ref test_weak_ref

.PHONY: clean
