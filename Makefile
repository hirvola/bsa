CFLAGS ?= -O3 -Wall
LDFLAGS ?=
ALGS = abm abndm asb bsa cacb cacbr casb cbsa cnaive naive tuabndm tusa twsa

all: $(ALGS)

%: main.c %.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
twsa: main.c twsa.c
	$(CC) $(CFLAGS) -mpopcnt $^ -o $@ $(LDFLAGS)

clean:
	$(RM) $(ALGS)

.PHONY: clean
