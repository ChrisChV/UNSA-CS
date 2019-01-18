%{
	#include <stdio.h>
	#include <math.h>
	#include <ctype.h>
	#include <string.h>
	#include "sad.h"

	#define TAM_TABLAS 256
	#define TAM_COD 256
	#define TAM_PARAM 256

	extern int yylineno;
	extern char * yytext;
	extern FILE *yyin;

	typedef union {
		int entero;
		float real;
		int boleano;
		double realLargo;
		long enteroLargo;
	} TipoValor;



	typedef struct{
		char nombre[256];
		int a1, a2;
		TipoValor a3;
		/*
		a1 -> Tipo int,float, etc
		a2 -> Ambito
		a3 -> Guarda el valor
		*/
	} TipoTablasSimbolo;

	TipoTablasSimbolo TS[TAM_TABLAS];
	int tamTS = 0;
	int insertaSimbolo(char *, int); //(nombre,tipo)
	int localizaSimbolo(char *); // (nombre) retorna -1 si no ha sido localizado
	int getSimbolo(char *); //funcion para semantica, llama a error cuando no existe el simbolo
	int IS(char * name);
	void muestraSimbolo();



	typedef struct {
		char nombre[256];
		int a1;
		int a2[TAM_PARAM];
		int tamA2;
		/*
		a1 -> Tipo int,float, etc
		a2 -> id de los parametros
		*/

	} TipoTablasFunciones;

	TipoTablasFunciones TF[TAM_TABLAS];
	int tamTF = 0;
	int indiceFunTemp = 0;
	int insertarFuncion(char *, int);
	int localizaFuncion(char *);
	int getFuncion(char *);
	int IFu(char * name);
	void muestraFuncion();
	int generateFuncion(char * fun);

	typedef struct{
		int op, a1, a2, a3;
	} TipoCodigo;

	TipoCodigo TC[TAM_COD];
	int tamTC = 0;
	int indiceVarTemp = 0;
	void genCodigo(int, int, int, int);
	void addCodigo(int pos, int, int, int, int);
	int genVarTemp();
	void muestraCodigo();
	int parseBolean(int);

	int tipoVar = 0;
	int ambitoActual = AMBITO_GLOBAL;
	int tipoActual = 0;

	int TStempTipo[TAM_TABLAS];
	int TStempAmbito[TAM_TABLAS];
	int TStempParam[TAM_TABLAS];
	int tamTStempTipo = 0;
	int tamTStempAmbito = 0;
	int tamTStempParam = 0;
	void setTipoTS(int tipo);
	void setAmbito(char * ambito);
	void setParam(int funcion);




	TipoCodigo TCtemp[TAM_COD];
	int tamTCtemp = 0;
	void addCodTemp(int, int, int ,int);
	void genCodTemp();



	void genCodAsig(int op_asig, int destino, int source);

	int getOpTipe(int tipe1, int tipe2);

	void opMover(int destino, int source);
	void runCode(char * funcion);
	void interprete();
	int aceptado;

	typedef struct{
		char * nombre;
		int tamCod;
		TipoCodigo cod[TAM_COD];
	}CodFunciones;

	CodFunciones Funciones[TAM_TABLAS];
	int tamFunciones = 0;
	int buscarFuncion(char * name);

	typedef struct{
		int vars[TAM_PARAM];
		int tamVars;
	}Parametros;

	Parametros params[TAM_TABLAS];
	int tamParams = 0;


	int varReturn[TAM_TABLAS]; //id de las variables donde se guardan los returns de las funciones;
	int actualVarReturn = -1;


%}

%union {
	int ival;
	float fval;
	char *sval;
}


%token <ival> T_INT T_FLOAT T_BOOL T_DOUBLE T_LONG T_VOID
%token <sval> ID
%token <ival> NUM_INT
%token <fval> NUM_FLOAT
%token NUM_DOUBLE NUM_LONG
%token PARENTESIS_IZQUIERDO PARENTESIS_DERECHO
%token LLAVE_IZQUIERDA LLAVE_DERECHA
%token COMA PUNTO_Y_COMA
%token FOR WHILE DO
%token IF ELSE
%token BREAK CONTINUE GOTO
%token ASIGNACION ASIGNACION_MAS ASIGNACION_MENOS ASIGNACION_POR ASIGNACION_ENTRE
%token OR_LOGICO AND_LOGICO
%token MAS MENOS POR ENTRE ELEVADO MODULO
%token IGUALDAD MAYOR MAYOR_IGUAL MENOR MENOR_IGUAL DIFERENTE
%token NEGACION INCREMENTO DECREMENTO
%token RETURN
%token B_TRUE B_FALSE
%token PRINT IN

%right ASIGNACION_POR ASIGNACION_ENTRE
%right ASIGNACION_MAS ASIGNACION_MENOS
%right ASIGNACION
%left OR_LOGICO
%left AND_LOGICO
%left IGUALDAD DIFERENTE
%left MENOR MENOR_IGUAL MAYOR MAYOR_IGUAL
%left MAS MENOS
%left POR ENTRE MODULO
%right ELEVADO


%%
programC	: listaDeclC ;
listaDeclC	: listaDeclC declC | ;
declC 		: Tipo listaVar PUNTO_Y_COMA {setTipoTS($<ival>1); setAmbito("global");}
			| T_VOID ID declFun bloqueFun 
						{int pos = IFu($2);
						 TF[pos].a1 = $<ival>1;
						 setAmbito($2); 
						 setParam(pos);
						 addCodigo($<ival>3, FUNC, pos, DIR_NULL, DIR_NULL);
						 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);};
			| Tipo ID declFun bloqueFun 
						{int pos = IFu($2);
						 TF[pos].a1 = $<ival>1;
						 setAmbito($2); 
						 setParam(pos);
						 addCodigo($<ival>3, FUNC, pos, DIR_NULL, DIR_NULL);
						 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);};
declFun		: PARENTESIS_IZQUIERDO Params PARENTESIS_DERECHO {$<ival>$ = tamTC;};
listaVar 	: declAsig COMA listaVar
			| ID COMA listaVar {int pos = IS($1); 
								TStempTipo[tamTStempTipo++] = pos;
								TStempAmbito[tamTStempAmbito++] = pos;}
			| declAsig
			| ID {int pos = IS($1);
				  TStempTipo[tamTStempTipo++] = pos;
				  TStempAmbito[tamTStempAmbito++] = pos;};
Params		: listaPar | ;
listaPar	: Tipo ID COMA listaPar 
				{int pos = IS($2);
				 TS[pos].a1 = $<ival>1;
				 TStempAmbito[tamTStempAmbito++] = TStempParam[tamTStempParam++] = pos;}
			| Tipo ID 
				{int pos = IS($2);
				 TS[pos].a1 = $<ival>1;
				 TStempAmbito[tamTStempAmbito++] = TStempParam[tamTStempParam++] = pos;};
bloqueFun 	: LLAVE_IZQUIERDA listaInstruc LLAVE_DERECHA ;
listaInstruc: listaInstruc instruc | ;
instruc 	: Tipo listaVar PUNTO_Y_COMA {setTipoTS($<ival>1);}
			| asig PUNTO_Y_COMA
			| declBucle
			| declCondicional
			| PRINT expr PUNTO_Y_COMA {genCodigo(IMPRIMIR, $<ival>2, DIR_NULL, DIR_NULL);}
			| IN ID PUNTO_Y_COMA {int pos = getSimbolo($2); genCodigo(OP_IN, pos, DIR_NULL, DIR_NULL);}
			| RETURN expr PUNTO_Y_COMA {genCodigo(INST_RETURN, $<ival>2, DIR_NULL, DIR_NULL);};
			| ID callFunc PUNTO_Y_COMA 
					{int fun = getFuncion($1);
					 genCodigo(SALTAR, fun, DIR_NULL, $<ival>2);
					 if(TF[fun].tamA2 != params[$<ival>2].tamVars){
					 	char er[80];
						strcpy(er, "error en los parametros de ");
						strcat(er, TF[fun].nombre);
						yyerror(er);
					 }};
declAsig	: ID op_asig asig 
					{int pos = IS($1);
					 TStempTipo[tamTStempTipo++] = TStempAmbito[tamTStempAmbito++] = pos;
					 genCodAsig($<ival>2, pos, $<ival>3);};
			| ID op_asig expr 
					{int pos = IS($1);
					 TStempTipo[tamTStempTipo++] = pos;
					 TStempAmbito[tamTStempAmbito++] = pos;
					 genCodAsig($<ival>2, pos, $<ival>3);};
			| ID op_asig ID callFunc 
					 {int pos = IS($1);
					  int fun = getFuncion($3);
					  TStempTipo[tamTStempTipo++] = TStempAmbito[tamTStempAmbito++] = pos;
					  genCodigo(SALTAR, fun, pos, $<ival>4);
					  if(TF[fun].tamA2 != params[$<ival>4].tamVars){
					 	char er[80];
						strcpy(er, "error en los parametros de ");
						strcat(er, TF[fun].nombre);
						yyerror(er);
					 }};
asig 		: ID op_asig asig
					{int pos = getSimbolo($1);
					 genCodAsig($<ival>2, pos, $<ival>3);
					 $<ival>$ = pos;}
			| ID op_asig expr 
					{int pos = getSimbolo($1);
					 genCodAsig($<ival>2, pos, $<ival>3);
					 $<ival>$ = pos;}
			| ID op_asig ID callFunc 
					{int pos = getSimbolo($1);
					 int fun = getFuncion($3);
					 genCodigo(SALTAR, fun, pos, $<ival>4);
					 $<ival>$ = pos;
					 if(TF[fun].tamA2 != params[$<ival>4].tamVars){
					 	char er[80];
						strcpy(er, "error en los parametros de ");
						strcat(er, TF[fun].nombre);
						yyerror(er);
					 }};
op_asig		: ASIGNACION {$<ival>$ = ASIGNACION;}
			| ASIGNACION_MENOS {$<ival>$ = ASIGNACION_MENOS;}
			| ASIGNACION_ENTRE {$<ival>$ = ASIGNACION_ENTRE;}
			| ASIGNACION_MAS {$<ival>$ = ASIGNACION_MAS;}
			| ASIGNACION_POR {$<ival>$ = ASIGNACION_POR};
callFunc 	: PARENTESIS_IZQUIERDO listaParam PARENTESIS_DERECHO {$<ival>$ = $<ival>2; tamParams++};
declBucle	: bucleFor PARENTESIS_IZQUIERDO initFor PUNTO_Y_COMA condicionFor parBucle bloqueBucle listaAsigFor PUNTO_Y_COMA
					{addCodigo($<ival>6, FUNC, $<ival>1, DIR_NULL, DIR_NULL);
					 genCodigo(SALTAR, $<ival>5, DIR_NULL, DIR_NULL);
					 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);
					 genCodigo(SALTAR, $<ival>5, DIR_NULL, DIR_NULL);}
			| bucleWhile PARENTESIS_IZQUIERDO condicionWhile parBucle bloqueBucle
					{addCodigo($<ival>4, FUNC, $<ival>1, DIR_NULL, DIR_NULL);
					 genCodigo(SALTAR, $<ival>3, DIR_NULL, DIR_NULL);
					 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);
					 genCodigo(SALTAR, $<ival>3, DIR_NULL, DIR_NULL);	}
			| bucleWhile PARENTESIS_IZQUIERDO condicionWhile PARENTESIS_DERECHO bucleDo bloqueBucle
					{addCodigo($<ival>5, FUNC, $<ival>1, DIR_NULL, DIR_NULL);
					 genCodigo(SALTAR, $<ival>3, DIR_NULL, DIR_NULL);
					 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);
					 genCodigo(SALTAR, $<ival>1, DIR_NULL, DIR_NULL);};
declCondicional : condicionalIf condicionalElse
					{if($<ival>2 != -1){
					 	genCodigo(SALTARF, $<ival>2, $<ival>1, DIR_NULL);
					 }};
condicionalIf	: tokenIf PARENTESIS_IZQUIERDO condicionIf parDerIf bloqueFun
						{ $<ival>$ = $<ival>3;
						 addCodigo($<ival>4, FUNC, $<ival>1, DIR_NULL, DIR_NULL);
						 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);
						 genCodigo(SALTARV, $<ival>1, $<ival>3, DIR_NULL);};						

condicionalElse	: tokenElse condicionalIf condicionalElse
						{$<ival>$ = generateFuncion("ELSE_IF");
						 addCodigo($<ival>1, FUNC, $<ival>$, DIR_NULL, DIR_NULL);
						 if($<ival>3 != -1){
					 	 	genCodigo(SALTARF, $<ival>3, $<ival>2, DIR_NULL);
					 	 }
					 	 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);
					 	 };

				| tokenElse bloqueFun 
						{$<ival>$ = generateFuncion("ELSE");
						 addCodigo($<ival>1, FUNC, $<ival>$, DIR_NULL, DIR_NULL);
						 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);}
				| {$<ival>$ = -1;};
tokenElse		: ELSE {$<ival>$ = tamTC;};
tokenIf			: IF {$<ival>$ = generateFuncion("IF");};
condicionIf 	: expr {$<ival>$ = $<ival>1};
parDerIf		: PARENTESIS_DERECHO {$<ival>$ = tamTC;};
bucleFor	: FOR {$<ival>$ = generateFuncion("FOR");}
bucleWhile	: WHILE {$<ival>$ = generateFuncion("WHILE");}
bucleDo		: DO {$<ival>$ = tamTC;}
initFor		: Tipo listaVar {setTipoTS($<ival>1);}
			| listaVarFor
			| ;
listaVarFor : asig COMA listaVarFor
			| asig;			
condicionFor: expr {$<ival>$ = generateFuncion("IF-FOR");
					 addCodigo(tamTC - 1, FUNC, $<ival>$, DIR_NULL, DIR_NULL);
					 genCodigo(SALTARV, $<ival>$ - 1, $<ival>1, DIR_NULL);
					 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);
					}
			| {$<ival>$ = generateFuncion("IF-FOR");
				int pos = genVarTemp();
				TS[pos].a1 = T_BOOL;
				TS[pos].a3.boleano = 1;
				genCodigo(FUNC, $<ival>$, DIR_NULL, DIR_NULL);
				genCodigo(SALTARV, $<ival>$ - 1, pos, DIR_NULL);
				genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);
				};
condicionWhile: expr {$<ival>$ = generateFuncion("IF-WHILE");
					 addCodigo(tamTC - 1, FUNC, $<ival>$, DIR_NULL, DIR_NULL);
					 genCodigo(SALTARV, $<ival>$ - 1, $<ival>1, DIR_NULL);
					 genCodigo(END, DIR_NULL, DIR_NULL, DIR_NULL);
					}
parBucle	: PARENTESIS_DERECHO {$<ival>$ = tamTC;};

listaAsigFor : asig COMA listaAsig {$<ival>$ = tamTC;}
 			 | incrementos COMA listaAsig {$<ival>$ = tamTC;}
 			 | incrementos {$<ival>$ = tamTC;}
			 | asig {$<ival>$ = tamTC;}
			 | {$<ival>$ = tamTC;};


listaAsig 	: asig COMA listaAsig
			| asig
			| ;

listaParam	: expr COMA listaParam {$<ival>$ = $<ival>3;
					 				params[$<ival>$].vars[params[$<ival>$].tamVars++] = $<ival>1;}
			| expr {$<ival>$ = tamParams; params[$<ival>$].vars[params[$<ival>$].tamVars++] = $<ival>1;}
			| {$<ival>$ = -1};

//listaExpr	: expr COMA listaExpr
//			| expr 
//			| ;

bloqueBucle : LLAVE_IZQUIERDA listaInstrucBucle LLAVE_DERECHA ;
listaInstrucBucle: listaInstrucBucle instruc
				 | listaInstrucBucle instrucBucle
				 | ;
instrucBucle: BREAK PUNTO_Y_COMA {genCodigo(OP_BREAK, DIR_NULL, DIR_NULL, DIR_NULL);}
			| CONTINUE PUNTO_Y_COMA ;
expr 		: term op_bin_arit expr 
					{$<ival>$ = genVarTemp();
					 genCodigo($<ival>2, $<ival>$, $<ival>1, $<ival>3);
					 TS[$<ival>$].a1 = getOpTipe(TS[$<ival>1].a1, TS[$<ival>3].a1);}
			| term op_bin_bool expr 
					{$<ival>$ = genVarTemp(); 
					 genCodigo($<ival>2, $<ival>$, $<ival>1, $<ival>3);
					 TS[$<ival>$].a1 = T_BOOL;}
			| term {$<ival>$ = $<ival>1;}
			| B_TRUE {$<ival>$ = genVarTemp(); 
					  TS[$<ival>$].a3.boleano = 1;
					  TS[$<ival>$].a1 = T_BOOL;}
			| B_FALSE {$<ival>$ = genVarTemp(); 
					   TS[$<ival>$].a3.boleano = 0;
					   TS[$<ival>$].a1 = T_BOOL;}
			| NEGACION term  {$<ival>$ = genVarTemp();
							  genCodigo(OP_NEGACION, $<ival>$, $<ival>2, DIR_NULL);
							  TS[$<ival>$].a1 = T_BOOL;}
			| incrementos {$<ival>$ = $<ival>1}
incrementos	: INCREMENTO term {int oneTemp = genVarTemp();
							   TS[oneTemp].a1 = T_INT;
							   TS[oneTemp].a3.entero = 1;
							   genCodigo(SUMAR, $<ival>2, $<ival>2, oneTemp);
							   $<ival>$ = $<ival>2;}
			| DECREMENTO term {int oneTemp = genVarTemp();
							   TS[oneTemp].a1 = T_INT;
							   TS[oneTemp].a3.entero = 1;
							   genCodigo(RESTAR, $<ival>2, $<ival>2, oneTemp);
							   $<ival>$ = $<ival>2;}
			| term INCREMENTO {int oneTemp = genVarTemp();
							   TS[oneTemp].a1 = T_INT;
							   TS[oneTemp].a3.entero = 1;
							   genCodigo(SUMAR, $<ival>1, $<ival>1, oneTemp);
							   $<ival>$ = $<ival>2;}
			| term DECREMENTO {int oneTemp = genVarTemp();
							   TS[oneTemp].a1 = T_INT;
							   TS[oneTemp].a3.entero = 1;
							   genCodigo(RESTAR, $<ival>1, $<ival>1, oneTemp);
							   $<ival>$ = $<ival>2;};
term 		: ID {$<ival>$ = localizaSimbolo($1);}
			| NUM_INT {	$<ival>$ = genVarTemp(); 
					 	TS[$<ival>$].a3.entero = $1;
					 	TS[$<ival>$].a1 = T_INT;
					 	}
			| NUM_FLOAT {$<ival>$ = genVarTemp(); 
						 TS[$<ival>$].a3.real = $1;
						 TS[$<ival>$].a1 = T_FLOAT;
						}

			| PARENTESIS_IZQUIERDO expr PARENTESIS_DERECHO {$<ival>$ = $<ival>2};
op_bin_arit	: MAS {$<ival>$ = SUMAR;}
			| MENOS {$<ival>$ = RESTAR;}
			| POR {$<ival>$ = MULTIPLICAR;}
			| ENTRE {$<ival>$ = DIVIDIR;}
			| ELEVADO {$<ival>$ = ELEVAR;}
			| MODULO {$<ival>$ = OP_MODULO;}
op_bin_bool : IGUALDAD {$<ival>$ = OP_IGUALDAD;}
			| MAYOR {$<ival>$ = OP_MAYOR;}
			| MAYOR_IGUAL {$<ival>$ = OP_MAYOR_IGUAL;}
			| MENOR {$<ival>$ = OP_MENOR;}
			| MENOR_IGUAL {$<ival>$ = OP_MENOR_IGUAL;}
			| DIFERENTE {$<ival>$ = OP_DIFERENTE;}
			| OR_LOGICO {$<ival>$ = OP_OR_LOGICO;}
			| AND_LOGICO {$<ival>$ = OP_AND_LOGICO;};

Tipo 		: T_INT {$<ival>$ = $1;}
			| T_FLOAT {$<ival>$ = $1;}
			| T_BOOL {$<ival>$ = $1;}
			| T_DOUBLE {$<ival>$ = $1;}
			| T_LONG {$<ival>$ = $1;};
%%


//Funciones TablaSimbolo

int localizaSimbolo(char * name){
	for(int i = 0; i < tamTS; i++){
		if(strcmp(name, TS[i].nombre) == 0 && TS[i].a2 == ambitoActual) return i;
	}
	return -1;
}

int getSimbolo(char * name){
	int res = localizaSimbolo(name);
	if(res < 0){
		char er[80];
		strcpy(er, "no existe la variable ");
		strcat(er, name);
		yyerror(er);
	}
	return res;
}

int insertaSimbolo(char * name, int tipo){
	if(localizaSimbolo(name) >= 0) return -1;
	strcpy(TS[tamTS].nombre,name);
	//TS[tamTS].a1  tipo;
	TS[tamTS].a2 = ambitoActual;
	return tamTS++;
}

int IS(char * name){
	int i = insertaSimbolo(name, tipoActual);
	if(i < 0){
		char er[80];
		strcpy(er, "re-declaracion de la variable ");
		strcat(er, name);
		yyerror(er);
	} 
	return i;
}

void muestraSimbolo(){
	TipoTablasSimbolo * iter;
	int i = 0;
	for(i = 0, iter = TS; i < tamTS; i++, iter++){
		switch(iter->a1){
			case T_INT: printf("%20s %s %d %d\n", iter->nombre, "INT", iter->a2, iter->a3.entero); break;
			case T_FLOAT: printf("%20s %s %d %f\n", iter->nombre, "FLOAT", iter->a2, iter->a3.real); break;
			case T_BOOL: printf("%20s %s %d %d\n", iter->nombre, "BOOL", iter->a2, parseBolean(iter->a3.boleano)); break;
			case T_LONG: printf("%20s %s %d %ld\n", iter->nombre, "LONG", iter->a2, iter->a3.enteroLargo); break;
			case T_DOUBLE: printf("%20s %s %d %lf\n", iter->nombre, "DOUBLE", iter->a2, iter->a3.realLargo); break;
		}
	}
}


//Funciones TablaFunciones

int localizaFuncion(char * name){
	for(int i = 0; i < tamTF; i++){
		if(strcmp(name, TF[i].nombre) == 0) return i;
	}
	return -1;
}

int getFuncion(char * name){
	int res = localizaFuncion(name);
	if(res < 0){
		char er[80];
		strcpy(er, "no exsite la funcion ");
		strcat(er, name);
		yyerror(er);
	}
}

int insertarFuncion(char * name, int tipo){
	if(localizaFuncion(name) >= 0) return -1;
	strcpy(TF[tamTF].nombre, name);
	return tamTF++;
}

int IFu(char * name){
	int i = insertarFuncion(name, tipoActual);
	if(i < 0){
		char er[80];
		strcpy(er, "re-declaracion de la funcion ");
		strcat(er, name);
		yyerror(er);
	}
	return i;
}

void muestraFuncion(){
	TipoTablasFunciones * iter;
	int i = 0;
	for(i = 0, iter = TF; i < tamTF; i++, iter++){
		printf("%20s %d", iter->nombre, iter->a1);
		for(int j = 0; j < iter->tamA2; j++){
			printf(" %s ", TS[iter->a2[j]].nombre);
		}
		printf("\n");
	}
}

int generateFuncion(char * fun){
	char t[30];
	sprintf(t, "_F%d_%s", indiceFunTemp++, fun);
	strcpy(TF[tamTF].nombre , t);
	return tamTF++;
}

void genCodigo(int op, int a1, int a2, int a3){
	TipoCodigo *p;
	p = &TC[tamTC];
	p->op = op;
	p->a1 = a1;
	p->a2 = a2;
	p->a3 = a3;
	tamTC++;
}

void addCodigo(int pos, int op, int a1, int a2, int a3){
	TipoCodigo temp1;
	TipoCodigo temp2;
	temp1 = TC[pos];
	temp2 = TC[pos];
	for(int i = pos + 1; i < tamTC + 1; i++){
		temp1 = temp2;
		temp2 = TC[i];
		TC[i] = temp1;
	}
	TipoCodigo *p;
	p = &TC[pos];
	p->op = op;
	p->a1 = a1;
	p->a2 = a2;
	p->a3 = a3;
	tamTC++;
}

int genVarTemp(){
	char t[30];
	sprintf(t, "_T%-2d", indiceVarTemp++);
	strcpy(TS[tamTS].nombre , t);
	TS[tamTS].a2 = ambitoActual;
	return tamTS++;
}

int parseBolean(int boleano){
	if(boleano) return 1;
	return 0;
}

//TODO
void muestraCodigo(){
	int op, a1, a2, a3;
	for(int i = 0; i < tamTC; i++){
		op = TC[i].op;
		a1 = TC[i].a1;
		a2 = TC[i].a2;
		a3 = TC[i].a3;
		printf("%2d) ", i);
		switch(op){
			case MOVER: printf("MOVER %s %s\n", TS[a1].nombre, TS[a2].nombre); break;
			case SUMAR: printf("SUMAR %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case RESTAR: printf("RESTAR %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case MULTIPLICAR: printf("MULTIPLICAR %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case DIVIDIR: printf("DIVIDIR %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case ELEVAR: printf("ELEVAR %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_MODULO: printf("OP_MODULO %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_IGUALDAD: printf("OP_IGUALDAD %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_MAYOR: printf("OP_MAYOR %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_MAYOR_IGUAL: printf("OP_MAYOR_IGUAL %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_MENOR: printf("OP_MENOR %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_MENOR_IGUAL: printf("OP_MENOR_IGUAL %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_DIFERENTE: printf("OP_DIFERENTE %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_OR_LOGICO: printf("OP_OR_LOGICO %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_AND_LOGICO: printf("OP_AND_LOGICO %s %s %s\n", TS[a1].nombre, TS[a2].nombre, TS[a3].nombre); break;
			case OP_NEGACION: printf("OP_NEGACION %s %s\n", TS[a1].nombre, TS[a2].nombre); break;
			case FUNC: printf("FUNC %s\n", TF[a1].nombre); break;
			case END: printf("END\n"); break;
			case SALTARV: printf("SALTARV %s %s\n", TF[a1].nombre, TS[a2].nombre); break;
			case SALTARF: printf("SALTARF %s %s\n", TF[a1].nombre, TS[a2].nombre); break;	
			case INST_RETURN: printf("INST_RETURN %s\n", TS[a1].nombre); break;
			case IMPRIMIR: printf("IMPRIMIR %s\n", TS[a1].nombre); break;
			case OP_IN: printf("OP_IN %s\n", TS[a1].nombre); break;
			case SALTAR: {
				printf("SALTAR %s ", TF[a1].nombre);
				if(a2 == -1) printf("VOID ");
				else printf("%s ", TS[a2].nombre);
				if(a3 == -1) printf("VOID\n");
				else {
					for(int i = 0; i < params[a3].tamVars; i++){
						printf("%s ", TS[params[a3].vars[i]].nombre);
					}
					printf("\n");
				}
				break;
			}
		}
	}
}

void setTipoTS(int tipo){
	for(int i = 0; i < tamTStempTipo; i++){
		TS[TStempTipo[i]].a1 = tipo;
	}
	tamTStempTipo = 0;
	//muestraSimbolo();
	//printf("\n");
}

void setAmbito(char * ambito){
	int ambt = localizaFuncion(ambito);
	if(ambt == -1) ambt = AMBITO_GLOBAL;
	for(int i = 0; i < tamTStempAmbito; i++){
		TS[TStempAmbito[i]].a2 = ambt;
	}
	tamTStempAmbito = 0;
}

void setParam(int funcion){
	for(int i = 0; i < tamTStempParam; i++){
		TF[funcion].a2[TF[funcion].tamA2++] = TStempParam[i];
	}
	tamTStempParam = 0;
}

void addCodTemp(int op, int a1, int a2, int a3){
	TipoCodigo *p;
	p = &TCtemp[tamTCtemp];
	p->op = op;
	p->a1 = a1;
	p->a2 = a2;
	p->a3 = a3;
	tamTCtemp++;
}

void genCodTemp(){
	int i = 0;
	TipoCodigo * iter;
	for(i = 0, iter = TCtemp; i < tamTCtemp; i++, iter++){
		genCodigo(iter->op, iter->a1, iter->a2, iter->a3);
	}
	tamTCtemp = 0;
}

void genCodAsig(int op_asig, int destino, int source){
	switch(op_asig){
		case ASIGNACION: genCodigo(MOVER, destino, source, DIR_NULL); break;
		case ASIGNACION_MENOS: genCodigo(RESTAR, destino, destino, source); break;
		case ASIGNACION_MAS: genCodigo(SUMAR, destino, destino, source); break;
		case ASIGNACION_ENTRE: genCodigo(DIVIDIR, destino, destino, source); break;
		case ASIGNACION_POR: genCodigo(MULTIPLICAR, destino, destino, source); break;
	}
}

int getOpTipe(int tipe1, int tipe2){
	if(tipe1 == T_DOUBLE || tipe2 == T_DOUBLE) return T_DOUBLE;
	if(tipe1 == T_FLOAT || tipe2 == T_FLOAT) return T_FLOAT;
	if(tipe1 == T_LONG || tipe2 == T_LONG) return T_LONG;
	return T_INT;
}

int buscarFuncion(char * name){
	for(int i = 0; i < tamFunciones; i++){
		if(strcmp(name, Funciones[i].nombre) == 0) return i;
	}
	return -1;
}


void setVal(TipoValor * val, void * numPtr, int tipoVal, int tipoNum){
	switch(tipoNum){
		case T_INT:{
			int num = *((int *) numPtr);
			switch(tipoVal){
				case T_INT: val->entero = num; break;
				case T_FLOAT: val->real = (float) num; break;
				case T_BOOL: val->boleano = num != 0; break;
				case T_LONG: val->enteroLargo = (long) num ; break;
				case T_DOUBLE: val->realLargo = (double) num; break;
			}
			break;
		}
		case T_FLOAT: {
			float num = *((float *) numPtr);
			switch(tipoVal){
				case T_INT: val->entero = (int) num; break;
				case T_FLOAT: val->real = num; break;
				case T_BOOL: val->boleano = num != 0; break;
				case T_LONG: val->enteroLargo = (long) num ; break;
				case T_DOUBLE: val->realLargo = (double) num; break;
			}
			break;
		}
		case T_BOOL: {
			int num = *((int *) numPtr);
			switch(tipoVal){
				case T_INT: val->entero = num; break;
				case T_FLOAT: val->real = (float) num; break;
				case T_BOOL: val->boleano = num != 0; break;
				case T_LONG: val->enteroLargo = (long) num ; break;
				case T_DOUBLE: val->realLargo = (double) num; break;
			}
			break;
		}
		case T_LONG: {
			long num = *((long *) numPtr);
			switch(tipoVal){
				case T_INT: val->entero = (int) num; break;
				case T_FLOAT: val->real = (float) num; break;
				case T_BOOL: val->boleano = num != 0; break;
				case T_LONG: val->enteroLargo = (long) num ; break;
				case T_DOUBLE: val->realLargo = num; break;
			}
			break;
		}
		case T_DOUBLE: {
			double num = *((double *) numPtr);
			switch(tipoVal){
				case T_INT: val->entero = (int) num; break;
				case T_FLOAT: val->real = (float) num; break;
				case T_BOOL: val->boleano = num != 0; break;
				case T_LONG: val->enteroLargo =  num ; break;
				case T_DOUBLE: val->realLargo = (double) num; break;
			}
			break;
		}
	}
}


void * getVal(TipoValor val, int tipo){
	void * res = NULL;
	switch(tipo){
		case T_INT: res = (void *) &(val.entero); break;
		case T_FLOAT: res = (void *) &(val.real); break;
		case T_BOOL: res = (void *) &(val.boleano); break;
		case T_LONG: res = (void *) &(val.enteroLargo); break;
		case T_DOUBLE: res = (void *) &(val.realLargo); break;	
	}
	return res;
}


void opBin(int destino, int val1, int val2, int op){
	int tipoDestino = TS[destino].a1;
	int tipoVal1 = TS[val1].a1;
	int tipoVal2 = TS[val2].a1;
	
	//*((int *) getVal(TS[val1].a3, tipoVal1));
	//*((int *) getVal(TS[val2].a3, tipoVal2));
	
	switch(op){
		case SUMAR: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 + num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case RESTAR: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 - num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case MULTIPLICAR: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 * num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case DIVIDIR: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 / num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case ELEVAR: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							float res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							double res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							double res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = pow(num1,num2);
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case OP_MODULO: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 % (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 %  (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = (int) num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = (int) num1 % (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							float res = (int) num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							float res = (int) num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = (int) num1 % (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 % (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 % (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							float res = num1 % (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_FLOAT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							long res = num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_LONG);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = num1 %  (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = (int) num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							double res = (int) num1 % (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							double res = (int) num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							double res = (int) num1 % num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							double res = (int) num1 % (int) num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_DOUBLE);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case OP_IGUALDAD: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 == num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case OP_MAYOR: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 > num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case OP_MAYOR_IGUAL: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 >= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case OP_MENOR: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 < num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case OP_MENOR_IGUAL: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 <= num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case OP_DIFERENTE: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 != num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case OP_OR_LOGICO: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 || num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case OP_AND_LOGICO: {
			switch(tipoVal1){
				case T_INT: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_FLOAT: {
					float num1 = *((float *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_BOOL: {
					int num1 = *((int *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_LONG: {
					long num1 = *((long *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
				case T_DOUBLE: {
					double num1 = *((double *) getVal(TS[val1].a3, tipoVal1));
					switch(tipoVal2){
						case T_INT: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_FLOAT: {
							float num2 = *((float *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);	
							break;
						}
						case T_BOOL: {
							int num2 = *((int *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_LONG: {
							long num2 = *((long *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
						case T_DOUBLE: {
							double num2 = *((double *) getVal(TS[val2].a3, tipoVal2));
							int res = num1 && num2;
							setVal(&(TS[destino].a3), (void *) &res, tipoDestino, T_INT);
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}
}

void opMover(int destino, int source){
	int tipoDestino = TS[destino].a1;
	int tipoSource = TS[source].a1;
	switch(tipoSource){
		case T_INT: {
			int num = *((int *) getVal(TS[source].a3, tipoSource));
			setVal(&(TS[destino].a3), (void *) &num, tipoDestino, tipoSource);
			break;
		}
		case T_FLOAT:{
			float num = *((float *) getVal(TS[source].a3, tipoSource));
			setVal(&(TS[destino].a3), (void *) &num, tipoDestino, tipoSource);
			break;
		}
		case T_BOOL:{
			int num = *((int *) getVal(TS[source].a3, tipoSource));
			setVal(&(TS[destino].a3), (void *) &num, tipoDestino, tipoSource);
			break;
		}
		case T_LONG:{
			long num = *((long *) getVal(TS[source].a3, tipoSource));
			setVal(&(TS[destino].a3), (void *) &num, tipoDestino, tipoSource);
			break;
		}
		case T_DOUBLE:{
			double num = *((double *) getVal(TS[source].a3, tipoSource));
			setVal(&(TS[destino].a3), (void *) &num, tipoDestino, tipoSource);
			break;
		}
	}
}

void opNegacion(int destino, int source){
	int tipo = TS[source].a1;
	switch(tipo){
		case T_INT: TS[destino].a3.boleano = !TS[source].a3.entero; break;
		case T_FLOAT: TS[destino].a3.boleano = !TS[source].a3.real; break;
		case T_BOOL: TS[destino].a3.boleano = !TS[source].a3.boleano; break;
		case T_LONG: TS[destino].a3.boleano = !TS[source].a3.enteroLargo; break;
		case T_DOUBLE: TS[destino].a3.boleano = !TS[source].a3.realLargo; break;
	}
}

void printSimbolo(int id){
	int tipo = TS[id].a1;
	switch(tipo){
		case T_INT: printf("%d\n", TS[id].a3.entero); break;
		case T_FLOAT: printf("%f\n", TS[id].a3.real); break;
		case T_BOOL: printf("%d\n", TS[id].a3.boleano); break;
		case T_LONG: printf("%ld\n", TS[id].a3.enteroLargo); break;
		case T_DOUBLE: printf("%lf\n", TS[id].a3.realLargo); break;
	}
}

void inSimbolo(int id){
	int tipo = TS[id].a1;

	switch(tipo){
		case T_INT: scanf("%d", &(TS[id].a3.entero)); break;
		case T_FLOAT: scanf("%f", &(TS[id].a3.real)); break;
		case T_BOOL: scanf("%d", &(TS[id].a3.boleano)); break;
		case T_LONG: scanf("%ld", &(TS[id].a3.enteroLargo)); break;
		case T_DOUBLE: scanf("%lf", &(TS[id].a3.realLargo)); break;
	}	
}

void runCode(char * funcion){
	int id = buscarFuncion(funcion);
	if(id < 0) yyerror("error al correr el codigo");
	TipoCodigo * iter;
	int i = 0;
	int op, a1, a2, a3;
	int funGen[64];
	int actualfunGen = -1;
	for(i = 0, iter = Funciones[id].cod; i < Funciones[id].tamCod; i++, iter++){
		op = iter->op;
		a1 = iter->a1;
		a2 = iter->a2;
		a3 = iter->a3;
		if(actualfunGen != -1){
			if(op == FUNC){
				Funciones[tamFunciones].nombre = TF[a1].nombre;
				actualfunGen++;
				funGen[actualfunGen] = tamFunciones;
				tamFunciones++;
			}
			else if(op == END) actualfunGen--;
			else Funciones[funGen[actualfunGen]].cod[Funciones[funGen[actualfunGen]].tamCod++] = Funciones[id].cod[i];
		}
		else{
			if(op == SUMAR || op == RESTAR || op == MULTIPLICAR || op == DIVIDIR || op == ELEVAR
				 || op == OP_MODULO || op == OP_IGUALDAD || op == OP_MAYOR
				 || op == OP_MAYOR_IGUAL || op == OP_MENOR || op == OP_MENOR_IGUAL
				 || op == OP_DIFERENTE || op == OP_OR_LOGICO || op == OP_AND_LOGICO) opBin(a1, a2, a3, op);
			else{
				switch(op){
					case FUNC: {
						Funciones[tamFunciones].nombre = TF[a1].nombre;
						actualfunGen++;
						funGen[actualfunGen] = tamFunciones;
						tamFunciones++;
						break;
					}
					case MOVER: opMover(a1, a2); break;
					case OP_NEGACION: opNegacion(a1, a2); break;
					case OP_BREAK: return;
					case INST_RETURN:{
						opMover(varReturn[actualVarReturn], a1);
						actualVarReturn--;
						return;
						break;
					}
					case IMPRIMIR: printSimbolo(a1); break;
					case OP_IN: inSimbolo(a1); break;
					case SALTAR: {
						if(a2 != DIR_NULL){
							actualVarReturn++;
							varReturn[actualVarReturn] = a2;
						}
						if(a3 != DIR_NULL){
							for(int i = 0; i < params[a3].tamVars; i++){
								opMover(TF[a1].a2[i], params[a3].vars[i]);
							}
						}
						runCode(TF[a1].nombre);
						break;
					}
					case SALTARV: {
						switch(TS[a2].a1){
							case T_INT: {if(TS[a2].a3.entero){runCode(TF[a1].nombre);}  break;} 
							case T_FLOAT: {if(TS[a2].a3.real){runCode(TF[a1].nombre);}  break;} 
							case T_BOOL: {if(TS[a2].a3.boleano){runCode(TF[a1].nombre);}  break;} 
							case T_LONG: {if(TS[a2].a3.enteroLargo){runCode(TF[a1].nombre);}  break;} 
							case T_DOUBLE: {if(TS[a2].a3.realLargo){runCode(TF[a1].nombre);}  break;} 
						}
						break;
					}
					case SALTARF: {
						switch(TS[a2].a1){
							case T_INT: {if(!TS[a2].a3.entero){runCode(TF[a1].nombre);}  break;} 
							case T_FLOAT: {if(!TS[a2].a3.real){runCode(TF[a1].nombre);}  break;} 
							case T_BOOL: {if(!TS[a2].a3.boleano){runCode(TF[a1].nombre);}  break;} 
							case T_LONG: {if(!TS[a2].a3.enteroLargo){runCode(TF[a1].nombre);}  break;} 
							case T_DOUBLE: {if(!TS[a2].a3.realLargo){runCode(TF[a1].nombre);}  break;} 
						}
						break;
					}
				}	
			}
			
		}
	}
}

void interprete(){
	Funciones[0].nombre = "global";
	for(int i = 0; i < tamTC; i++){
		Funciones[0].cod[Funciones[0].tamCod++] = TC[i];
	}
	tamFunciones++;
	runCode("global");
}



int yyerror(char *s) {
	//printf("%s\n",s);
	fprintf(stderr,"At line %d %s: %s \n ", yylineno, s, yytext);	
	aceptado = 0;
}

int main(int argc, char ** argv) {
	if(argc != 2){
		printf("%s\n", "Faltan argumentos <file>");
		return 0;
	}
	FILE * fileIn;
	if(fileIn = fopen(argv[1], "r")){
		yyin = fileIn;
	}
	else{
		printf("%s\n", "El archivo no existe");
		return 0;
	}
	aceptado = 1;
	int tipo = 0;	
	insertarFuncion("global", 0);
	yyparse();
	int m = getFuncion("main");
	if(m < 0) yyerror("Se debe declarar la funcion main");
	if(TF[m].a1 != T_INT) yyerror("La funcion main debe retornar un entero");
	int varReturnMain = genVarTemp();
	TS[varReturnMain].a1 = T_INT;
	genCodigo(SALTAR, m, varReturnMain, DIR_NULL);
	if(aceptado){
		muestraSimbolo();
		printf("\n");
		muestraFuncion();
		printf("\n\n");
		muestraCodigo();
		printf("\n\n");
		interprete();
	}
}
