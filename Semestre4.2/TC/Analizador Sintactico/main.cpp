#include <iostream>
#include <stdio.h>
#include <ctype.h>

using namespace std;


char preanalisis;
void parea(char);
void error();
void resto();
void term();
void exp();
void exp(){
    if(isdigit(preanalisis)){
        term();
        resto();
    }
    else error();
}

void resto(){
    if(preanalisis == '+'){
        parea('+');
        term();
        cout<<"+";
        resto();
    }
    else if(preanalisis == '-'){
        parea('-');
        term();
        cout<<"-";
        resto();
    }
    else if(preanalisis != '\n'){
        error();
    }
    ///cadena vacia;
}

void term(){
    if(isdigit(preanalisis)){
        cout<<preanalisis;
        parea(preanalisis);
    }
    else{
        error();
    }
}

void error(){
    cout<<endl;
    cout<<"Error"<<endl;
}

void parea(char t){
    if(preanalisis == t){
        preanalisis = getwchar();
    }
    else error();
}

int main(){
    preanalisis = getwchar();
    exp();
}

/*
char preanalisis;
void parea(char);
void A();
void B();
void S();
void error();


void parea(char t){
    if(preanalisis == t){
        preanalisis = getwchar();
    }
    else error();
}
void S(){
    if (preanalisis == 'x'){
        parea('x');
        S();
    }
    else if (preanalisis == 'a'){
        A();
        B();
        parea('c');
    }
    //else if(preanalisis == '\n') return;
    else error();
}

void A(){
    if(preanalisis == 'a')
        parea('a');
    else{
        error();
    }
}
void B(){
    if(preanalisis == 'b')
        parea('b');
    else{
        error();
    }
}
void error(){
	cout<<"Error de sintaxis"<<endl;
}

int main()
{
    preanalisis = getwchar();
    S();
}
*/

