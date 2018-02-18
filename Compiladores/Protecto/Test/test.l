%{
#include "y.tab.h"
#include <stdlib.h>
#include <stdio.h>
%}

white           [ \t]+
digit           [0-9]
integer         {digit}+

%%

{white}         { /* Ignoramos espacios en blanco */ }
"exit"|"quit"|"bye"	{printf("Terminando programa\n");exit(0);}
{integer}	{
                  yylval.dval=atof(yytext);
                  return(NUMBER);
                }

"+"           return(PLUS);
"-"           return(MINUS);
"*"           return(TIMES);
"/"           return(DIVIDE);
"^"           return(POWER);
"("           return(LEFT_PARENTHESIS);
")"           return(RIGHT_PARENTHESIS);
"\n"  return(END);

%%