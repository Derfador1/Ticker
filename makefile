
CFLAGS+=-std=c11
CLFAGS+=-Wall -Wextra -Wpedantic
CFLAGS+=-Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline

CFLAGS+=-I../llist

driver1: driver1.o ll_stack.o double_llist.o

.PHONY: clean debug

clean:
	-rm *.o

debug: CFLAGS+=-g
