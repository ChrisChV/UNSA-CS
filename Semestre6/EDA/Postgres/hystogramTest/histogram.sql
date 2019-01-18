create type histogram;

CREATE FUNCTION htg_in(cstring)
   RETURNS histogram
   AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/n97/histogramType.so'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION htg_out(histogram)
   RETURNS cstring
  AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/n97/histogramType.so'
   LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE histogram (
   internallength = 1200,
   input = htg_in,
   output = htg_out,
   alignment = double
);

CREATE FUNCTION htg_same(histogram, histogram)
    RETURNS bool
    AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/n97/histogramType.so'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
    leftarg = histogram,
    rightarg = histogram,
    procedure = htg_same,
    commutator = =
);

CREATE FUNCTION htg_lt(histogram, histogram)
    RETURNS bool
    AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/n97/histogramType.so'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
    leftarg = histogram,
    rightarg = histogram,
    procedure = htg_lt,
    commutator = <
);

CREATE FUNCTION htg_lte(histogram, histogram)
    RETURNS bool
    AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/n97/histogramType.so'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
    leftarg = histogram,
    rightarg = histogram,
    procedure = htg_lte,
    commutator = <=
);

CREATE FUNCTION htg_gt(histogram, histogram)
    RETURNS bool
    AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/n97/histogramType.so'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR > (
    leftarg = histogram,
    rightarg = histogram,
    procedure = htg_gt,
    commutator = >
);

CREATE FUNCTION htg_gte(histogram, histogram)
    RETURNS bool
    AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/n97/histogramType.so'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >= (
    leftarg = histogram,
    rightarg = histogram,
    procedure = htg_gte,
    commutator = >=
);

CREATE FUNCTION htg_distance(histogram, histogram)
    RETURNS float8
    AS '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/n97/histogramType.so'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <-> (
    leftarg = histogram,
    rightarg = histogram,
    procedure = htg_distance,
    commutator = <->
);


CREATE OPERATOR CLASS histogram_ops
    DEFAULT FOR TYPE histogram USING btree AS
        OPERATOR        1       <,
        OPERATOR        2       <=,
        OPERATOR        3       =,
        OPERATOR        4       >,
        OPERATOR        5      	>=,



CREATE TABLE test_htg (
	a	histogram
);

INSERT INTO test_htg VALUES('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/00372608.jpeg');
INSERT INTO test_htg VALUES('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/29440854.jpeg');

SELECT * FROM test_htg;

SELECT * FROM test_htg where a = '/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/00372608.jpeg';

INSERT INTO test_htg VALUES('/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/tt');

DROP TABLE test_htg;
DROP TYPE histogram CASCADE;

CREATE TABLE unsa(
	ID serial,
	HTG histogram,
	PATH varchar
);

DROP TABLE unsa;

	DROP TABLE IF EXISTS landscapes;

	CREATE TABLE landscapes (
		ID serial,
		HTG histogram,
		PATH varchar
	);


