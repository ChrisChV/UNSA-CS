create table test1(
   name text,
   histo decimal[]
);

create type complex;

CREATE FUNCTION complex_in(cstring)
   RETURNS complex
   AS '/home/xnpio/Documentos/Xnpio/EDA/complex/complex.so'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_out(complex)
   RETURNS cstring
   AS '/home/xnpio/Documentos/Xnpio/EDA/complex/complex.so'
   LANGUAGE C IMMUTABLE STRICT;


CREATE TYPE complex (
   internallength = 16,
   input = complex_in,
   output = complex_out,
   alignment = double
);

CREATE TABLE test_complex (
	a	complex,
	b	complex
);

INSERT INTO test_complex VALUES ('(1.0, 2.5)', '(4.2, 3.55 )');
INSERT INTO test_complex VALUES ('(1.0)', '(4.2)');

select * from test_complex

DROP TABLE test_complex;
DROP TYPE complex CASCADE;



