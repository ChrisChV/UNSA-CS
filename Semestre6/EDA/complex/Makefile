MODULES = randomness

PG_CONFIG = pg_config
PGXS = $(shell $(PG_CONFIG) --pgxs)
INCLUDEDIR = $(shell $(PG_CONFIG) --includedir-server)
include $(PGXS)

randomness.so: complex.o
	cc -shared -o complex.so complex.o

complex.o: complex.c
	cc -o complex.o -c complex.c $(CFLAGS) -I$(INCLUDEDIR)
