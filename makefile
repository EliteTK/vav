srcdir = .

CC = gcc

INSTALL = install -c
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

prefix = /usr/local

exec_prefix = $(prefix)

libdir = $(exec_prefix)/lib
includedir = $(prefix)/include

CFLAGS = -O2 -Wall -I$(srcdir) -std=gnu11
LDFLAGS =
LDLIBS = -lm

SRCS = vec.c
OBJS = $(SRCS:.c=.o)

SRCS_TEST = test.c vec.c
OBJS_TEST = $(SRCS_TEST:.c=.o)

.PHONY: all
all: libvec.so

libvec.so: LDFLAGS += -shared
libvec.so: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJS): CFLAGS += -fPIC

.PHONY: check
check: test
	@./test

test: LDLIBS += -lcheck
test: $(OBJS_TEST)

.PHONY: install
install: all
	$(INSTALL) -d $(DESTDIR)$(libdir)
	$(INSTALL_PROGRAM) libvec.so $(DESTDIR)$(libdir)
	$(INSTALL) -d $(DESTDIR)$(includedir)
	$(INSTALL_PROGRAM) vec.h $(DESTDIR)$(includedir)

.PHONY: uninstall
uninstall:
	$(RM) $(DESTDIR)$(libdir)/libvec.so
	$(RM) $(DESTDIR)$(includedir)/vec.h

.PHONY: clean
clean:
	$(RM) *.o *.so test tags

.PHONY: tags
TAGS:
	ctags -R "$(srcdir)"
