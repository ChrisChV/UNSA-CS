MODULES = MtreeGist

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

MtreeGist.so: mtree_gist.o
	cc -shared -o mtree_gist.so mtree_gist.o

MtreeGist.o: mtree_gist.c
	cc -o mtree_gist.o -c mtree_gist.c $(CFLAGS) -I$(INCLUDEDIR)

