#include <stdio.h>
#include <ctype.h>
/* Global declarations */
/* Variables */

/*Ignorar Comentarios*/

#define NUM_RESERVED 8
#define MAX_LEN_RESERVED 6

#define COMMENTARY_OFF 0
#define COMMENTARY_ON 1
#define COMMENTARY_INI 2
#define COMMENTARY_FIN_1 3
#define COMMENTARY_FIN_2 4

int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
int commentaryState;
FILE *in_fp, *fopen();
int reservedIndex = 0;
int indexOfActualReserved = 0;
int actualReserved[NUM_RESERVED];

char reserved[NUM_RESERVED][MAX_LEN_RESERVED] = {	{'f','o','r','$','$','$'},
													{'i','f','$','$','$','$'},
													{'e','l','s','e','$','$'},
													{'w','h','i','l','e','$'},
													{'d','o','$','$','$','$'},
													{'i','n','t','$','$','$'},
													{'f','l','o','a','t','$'},
													{'s','w','i','t','c','h'}};



void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

#define RESERVED_CODE 30
#define FOR_CODE 30
#define IF_CODE 31
#define ELSE_CODE 32
#define WHILE_CODE 33
#define DO_CODE 34
#define INT_CODE 35
#define FLOAT_CODE 36 
#define SWITCH_CODE 37

/******************************************************/
/* main driver */
int main() {
/* Open the input data file and process its contents */
	if ((in_fp = fopen("front.in", "r")) == NULL)
		printf("ERROR - cannot open front.in \n");
	else {
		for(int i = 0; i < NUM_RESERVED; i++){
			actualReserved[i] = -1;
		}
		commentaryState = COMMENTARY_OFF;
		getChar();
		do {
			lex();
		} while (nextToken != EOF);
	}
	return 0;
}


/*****************************************************/
/* lookup - a function to lookup operators and parentheses
and return the token */
int lookup(char ch) {
	switch (ch) {
		case '(':
			addChar();
			nextToken = LEFT_PAREN;
			break;
		case ')':
			addChar();
			nextToken = RIGHT_PAREN;
			break;
		case '+':
			addChar();
			nextToken = ADD_OP;
			break;
		case '-':
			addChar();
			nextToken = SUB_OP;
			break;
		case '*':
			if(commentaryState == COMMENTARY_ON){
				commentaryState = COMMENTARY_FIN_1;
			}
			else if(commentaryState == COMMENTARY_INI){
				commentaryState = COMMENTARY_ON;
			}
			addChar();
			nextToken = MULT_OP;
			break;
		case '/':
			if(commentaryState == COMMENTARY_OFF){
				commentaryState = COMMENTARY_INI;
			}
			else if(commentaryState == COMMENTARY_FIN_1){
				commentaryState = COMMENTARY_FIN_2;
			}
			addChar();
			nextToken = DIV_OP;
			break;
		default:
			addChar();
			nextToken = EOF;
			break;
	}
	return nextToken;
}


/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else printf("Error - lexeme is too long \n");
}

/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar() {
	if ((nextChar = getc(in_fp)) != EOF) {
		if (isalpha(nextChar)) charClass = LETTER;
		else if (isdigit(nextChar)) charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else charClass = EOF;
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}

int verificarReserved(){
	for(int i = 0; i < indexOfActualReserved; i++){
		if(actualReserved[i] != -1){
			int temp = actualReserved[i];
			actualReserved[i] = -1;
			indexOfActualReserved = 0;
			reservedIndex = 0;
			return temp;
		}
	}
	return -1;
}

/* lex - a simple lexical analyzer for arithmetic
expressions */
int lex() {
	lexLen = 0;
	getNonBlank();	
	int flag = -1;
	switch (charClass) {
	/* Parse identifiers */
		case LETTER:
			if(commentaryState == COMMENTARY_ON){
				getChar();
				break;
			}
			else if(commentaryState == COMMENTARY_INI){
				printf("Next token is: %d, Next lexeme is %s\n",
				DIV_OP, "/");
				commentaryState = COMMENTARY_OFF;
			}
			else if(commentaryState == COMMENTARY_FIN_1){
				commentaryState = COMMENTARY_ON;
			}
			for(int i = 0; i < NUM_RESERVED; i++){
				if(reserved[i][reservedIndex] == nextChar){
					actualReserved[indexOfActualReserved] = i;
					indexOfActualReserved++;
				}
			}
			reservedIndex++;
			addChar();
			getChar();
			while (charClass == LETTER || charClass == DIGIT) {
				for(int i = 0; i < indexOfActualReserved; i++){
					if(actualReserved[i] != -1 && reserved[actualReserved[i]][reservedIndex] != nextChar){
						actualReserved[i] = -1;
					}
				}
				reservedIndex++;
				addChar();
				getChar();
			}
			flag = verificarReserved();
			if(flag == -1) nextToken = IDENT;
			else {
				nextToken = RESERVED_CODE + flag;
			}
			break;

		/* Parse integer literals */
		case DIGIT:
			if(commentaryState == COMMENTARY_ON){
				getChar();
				break;
			}
			else if(commentaryState == COMMENTARY_INI){
				printf("Next token is: %d, Next lexeme is %s\n",
				MULT_OP, "*");
				commentaryState = COMMENTARY_OFF;
			}
			else if(commentaryState == COMMENTARY_FIN_1){
				commentaryState = COMMENTARY_ON;
			}
			addChar();
			getChar();
			while (charClass == DIGIT) {
				addChar();
				getChar();
			}
			nextToken = INT_LIT;
			break;
		/* Parentheses and operators */
		case UNKNOWN:
			lookup(nextChar);
			getChar();
			break;
	/* EOF */
		case EOF:
			nextToken = EOF;
			lexeme[0] = 'E';
			lexeme[1] = 'O';
			lexeme[2] = 'F';
			lexeme[3] = 0;
			break;
	} /* End of switch */
	if(commentaryState == COMMENTARY_OFF){
		printf("Next token is: %d, Next lexeme is %s\n",
		nextToken, lexeme);	
	}
	else if(commentaryState == COMMENTARY_FIN_2){
		commentaryState = COMMENTARY_OFF;
	}
	return nextToken;
} /* End of function lex */






