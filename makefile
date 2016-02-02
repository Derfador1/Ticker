
CFLAGS+=-std=c11
CFLAGS+=-Wall -Wextra -Wpedantic
CFLAGS+=-Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline

profile:CFLAGS+=-pg
profile:LDFLAGS+=-pg
profile:ticker

CFLAGS+=-D_BSD_SOURCE

LDLIBS+=-lm

ticker: ticker.o

.PHONY: clean debug

clean:
	-rm *.o

debug: CFLAGS+=-g
debug: ticker
