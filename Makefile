CC=gcc
CCFLAGS=-Wall -g
LDFLAGS=-pthread -lrt

SOURCES_COMMON=./common.c

SOURCES_WAITER=$(SOURCES_COMMON) ./waiter.c
OBJECTS_WAITER=$(SOURCES_WAITER:.c=.o)
TARGET_WAITER=waiter

SOURCES_COOK=$(SOURCES_COMMON) ./cook.c
OBJECTS_COOK=$(SOURCES_COOK:.c=.o)
TARGET_COOK=cook

COMPNAME=waiter

all: $(TARGET_WAITER) $(TARGET_COOK)

$(TARGET_WAITER): $(OBJECTS_WAITER)
		$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET_COOK): $(OBJECTS_COOK)
		$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
		$(CC) $(CCFLAGS) -c $<

%.o: %.c
		$(CC) $(CCFLAGS) -c $<

clean:
		rm -f *.o $(TARGET_WAITER) $(TARGET_COOK)

wc: 
		wc -l *.c *.h

zip:
		zip $(COMPNAME).zip *.c *.h Makefile README.md

tar:
		tar -cvzf $(COMPNAME).tar *.c *.h Makefile README.md

delzip:
		rm $(COMPNAME).zip

deltar: 
		rm $(COMPNAME).tar

sha:
		sha256sum *.c *.h
