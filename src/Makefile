#
# Makefile for aioseq
#

CC=gcc
CFLAGS=-Wall -Werror -pedantic -std=c11 -lcdk -lncurses -lrt -g -D_POSIX_C_SOURCE=200809L -lasound -O2
Q=@

OBJS=$(subst .c,.o,$(wildcard *.c))
DEPS=$(subst .o,.d,$(OBJS))

ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(filter-out %.d,$(MAKECMDGOALS)),)
    -include $(DEPS)
  endif
endif

all: $(DEPS) aioseq

.PRECIOUS: %.d
%.d: %.c
	$(Q)$(CC) -MM -MG $(CFLAGS) $< | grep -v -e '^ \\'>> $@

%.o: %.c
	$(Q)$(CC) -o $@ -c $< $(CFLAGS)

aioseq: $(OBJS)
	$(Q)$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJS) $(DEPS) aioseq *~ *.ass
