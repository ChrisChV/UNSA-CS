MODULES = randomness

PG_CONFIG = pg_config
PGXS = $(shell $(PG_CONFIG) --pgxs)
INCLUDEDIR = $(shell $(PG_CONFIG) --includedir-server)
include $(PGXS)

randomness.so: histogramType.o
	cc -shared -o histogramType.so histogramType.o

complex.o: complex.c
	cc -o histogramType.o -c histogramType.c $(CFLAGS) -I$(INCLUDEDIR)
