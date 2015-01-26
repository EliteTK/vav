DIR_SRC := src
DIR_TESTS := tests

PREFIX := /usr
LIBDIR := $(PREFIX)/lib
INCLUDEDIR := $(PREFIX)/include

SRCDIR := src

SOURCES := vec.o
SOURCES := $(addprefix $(SRCDIR)/,$(SOURCES))

OUTPUT := libvec.so

VPATH := src:tests

.PHONY: check install uninstall clean

all : $(OUTPUT)

$(SRCDIR)/%.o : CFLAGS = -O2 -Wall -std=gnu11 -fPIC

$(OUTPUT) : LDFLAGS := -shared -lm
$(OUTPUT) : src/vec.o
	$(CC) $(LDFLAGS) -o $@ $^

test.o : CFLAGS = -O2 -Wall -std=gnu11 -I src

test : LDFLAGS := -lcheck -lm
test : test.o vec.o

check : vec.o test
	@./test

install : $(LIBDIR)/

uninstall :

clean :
	rm -f *.o
	rm -f *.so
	rm -f test
	rm -f tags

TAGS :
	ctags -R .
