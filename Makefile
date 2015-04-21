CFLAGS = -std=c11 -fPIC -Wall -Wextra
LDFLAGS = -Wl,--as-needed
LDLIBS = -lm
OBJECTS = vec.o
BINARIES = libvec.so test

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -Og -g
else
	CFLAGS += -flto -O2 -DNDEBUG
	LDFLAGS += -flto -O2
endif

all: $(BINARIES)

libvec.so: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared $^ $(LDLIBS) -o $@

check: test
	@./test

test: LDLIBS += -lcheck
test: test.c $(OBJECTS)

vec.o: vec.c vec.h

clean:
	rm -f $(OBJECTS) $(BINARIES)

.PHONY: all clean check
