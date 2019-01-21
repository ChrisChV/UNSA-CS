--Mkey as key

CREATE OR REPLACE FUNCTION histogram_consistent(internal, histogram, smallint, oid, internal)
RETURNS bool
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/n8/mtreeGist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION histogram_union(internal, internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/n8/mtreeGist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION histogram_compress(internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/n8/mtreeGist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION histogram_decompress(internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/n8/mtreeGist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION histogram_picksplit(internal,internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/n8/mtreeGist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION histogram_penalty(internal,internal,internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/n8/mtreeGist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION histogram_same(histogram,histogram,internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/n8/mtreeGist.so'
LANGUAGE C STRICT;

DROP OPERATOR CLASS IF EXISTS mtree_gist_histogram USING gist;

CREATE OPERATOR CLASS mtree_gist_histogram
DEFAULT FOR TYPE histogram USING gist
AS
	FUNCTION	1	histogram_consistent(internal, histogram, smallint, oid, internal),
	FUNCTION	2	histogram_union(internal, internal),
	FUNCTION	3	histogram_compress(internal),
	FUNCTION	4	histogram_decompress(internal),
	FUNCTION	5	histogram_penalty(internal,internal,internal),
	FUNCTION	6	histogram_picksplit(internal,internal),
	FUNCTION	7	histogram_same(histogram,histogram,internal);

--Histogram as key

CREATE OR REPLACE FUNCTION gist_histogram_consistent(internal, histogram, smallint, oid, internal)
RETURNS bool
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/v1/mtree_gist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION gist_histogram_union(internal, internal)
RETURNS histogram
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/v1/mtree_gist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION gist_histogram_compress(internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/v1/mtree_gist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION gist_histogram_decompress(internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/v1/mtree_gist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION gist_histogram_picksplit(internal,internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/v1/mtree_gist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION gist_histogram_penalty(internal,internal,internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/v1/mtree_gist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION gist_histogram_same(internal,internal,internal)
RETURNS internal
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/v1/mtree_gist.so'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION gist_histogram_distance(internal,histogram,smallint,oid)
RETURNS float8
AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/M-Tree/v1/mtree_gist.so'
LANGUAGE C STRICT;

DROP OPERATOR CLASS IF EXISTS mtree_gist_histogram USING gist;

CREATE OPERATOR CLASS mtree_gist_histogram
DEFAULT FOR TYPE histogram USING gist
AS
	OPERATOR	15 	<->(histogram,histogram) FOR ORDER BY float_ops,
	FUNCTION	1	gist_histogram_consistent(internal, histogram, smallint, oid, internal),
	FUNCTION	2	gist_histogram_union(internal, internal),
	FUNCTION	3	gist_histogram_compress(internal),
	FUNCTION	4	gist_histogram_decompress(internal),
	FUNCTION	5	gist_histogram_penalty(internal,internal,internal),
	FUNCTION	6	gist_histogram_picksplit(internal,internal),
	FUNCTION	7	gist_histogram_same(internal,internal,internal),
	FUNCTION	8	gist_histogram_distance(internal,histogram,smallint,oid);


--test

drop table if exists unsa;

CREATE TABLE unsa(
	ID serial,
	HTG histogram,
	PATH varchar
);

create index mtreeUnsa on unsa using gist(htg);

drop table if exists test;

create table test(
	htg histogram
);

create index abc on test using gist(htg);

INSERT INTO test VALUES('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/00372608.jpeg');
insert into test values('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/29440854.jpeg');

insert into test values('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/10637937.jpeg');
insert into test values('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/30961978.jpeg');

insert into test values('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/44421633.jpeg');
insert into test values('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/61039109.jpeg');

insert into test values('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/72130009.jpeg');
insert into test values('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/73415481.jpeg');

insert into test values('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/80219277.jpeg');
insert into test values('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/81604096.jpeg');

select * from test;

select path, (htg <-> '/home/xnpio/Documentos/Imagenes2/29440854.jpeg'::histogram) as dist
	from unsa order by dist asc limit 10;



DROP TABLE IF EXISTS landscapes;

CREATE TABLE landscapes (
	ID serial,
	HTG histogram,
	PATH varchar
);

create index mtreeLC on landscapes using gist(htg);

