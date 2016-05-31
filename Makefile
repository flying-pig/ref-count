GCC = $(shell cc --version | grep GCC | sed 's/^[^ ]* (\([^)]*\).*/\1/g')

ifeq ($(GCC), GCC)
	LDFLAGS = -pthread
else
	LDFLAGS =
endif

CFLAGS = -g -Wall -O -I. -Iref-count
OBJS = test.o
SHARED_OBJS = test_shared_ref.o shared_count.o
WEAK_OBJS = test_weak_ref.o shared_count.o
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
