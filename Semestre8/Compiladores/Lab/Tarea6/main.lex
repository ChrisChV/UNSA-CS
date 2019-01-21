%%

\/\/.+\n {
	printf("");
}

\/\*(.\n?)+\*\/ {
	printf("");
}

\n {
	printf("");
}

\t {
	printf("");
}

[ ] {
 	printf("");
}



while {
	printf("WHILE->%s\n", yytext);
}
if {
	printf("IF->%s\n", yytext);
}



\+ {
	printf("MAS->%s\n", yytext);
}
- {
	printf("MENOS->%s\n", yytext);
}
\* {
	printf("MULT->%s\n", yytext);
}
\/ {
	printf("DIV->%s\n", yytext);
}


>= {
	printf("MAYOR IGUAL->%s\n", yytext);
}
> {
	printf("MAYOR->%s\n", yytext);
}
\<= {
	printf("MENOR IGUAL->%s\n", yytext);
}
\< {
	printf("MENOR->%s\n", yytext);
}
== {
	printf("IGUAL->%s\n", yytext);
}
= {
	printf("ASIG->%s\n", yytext);
}
!= {
	printf("DIF->%s\n", yytext);
}
! {
	printf("NEG->%s\n", yytext);
}




\{ {
	printf("LLAVE IZQ->%s\n", yytext);
}

\} {
	printf("LLAVE DER->%s\n", yytext);
}
\[ {
	printf("COR IZQ->%s\n", yytext);
}
\] {
	printf("COR DER->%s\n", yytext);
}
\( {
	printf("PAR IZQ->%s\n", yytext);
}
\) {
	printf("PAR DER->%s\n", yytext);
}
\" {
	printf("COMILLAS->%s\n", yytext);
}




; {
	printf("PUNTO Y COMA->%s\n", yytext);
}

[0-9]+ {
	printf("NUM->%s\n", yytext);
}
[a-zA-Z]+[0-9]* {
	printf("ID->%s\n", yytext);
}

. {
	printf("NO RECONOCIDO->%s\n", yytext);
}

%%

main(){
	yylex();
}