%{
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdbool.h>
    void push1();
    char* top1();
    void prefunc(char a);
char* pop1();
int pop2();
void push2();
int top2();
void ADD();
void SUB();
void MUL();
void DIV();
void printstack();
void exitfunc();
void reset();
void newfunc();
FILE *yyin, *yyout;
bool isvalid = true;
%}

%name parse
%token ID ID2 Newline khatm
%left '+'
%left '-'
%left '*'
%left '/'

%%

S    :  E {newfunc();}
    ;
E   : E E '+' {prefunc('+'); ADD();}
    | E E '-' {prefunc('-'); SUB();}
    | E E '*' {prefunc('*'); MUL();}
    | E E '/' {prefunc('/'); DIV();}
    | ID    {push1(); push2();}
    | ID2 {exitfunc();}
    | Newline {newfunc();}
    | khatm {exit(1);}
	;
%%
#include"lex.yy.c"

char st[100][10];
int res[100];
int indx=0;
int indx2=0;

void exitfunc()
{
	isvalid = false;
}

void newfunc()
{
	char* s1 = top1();
	int s2 = top2();
	if(isvalid)
		fprintf(yyout,"%s %d",s1, s2 );
	else
		fprintf(yyout, "invalid_input");	
	reset();
}

void reset()
{
fprintf(yyout,"\n");
	indx = 0;
	indx2 = 0;
	isvalid = true;
}

void push1()
{
   strcpy(st[indx++],yytext);
}

void printstack()
{	fprintf(yyout,"PRinting st : ");
	for(int j=0; j<indx; j++)
	{
		fprintf(yyout,"%s ,", st[j]);
	}
	fprintf(yyout,"\n printing res : ");
	for(int j=0; j<indx2; j++)
	{
		fprintf(yyout,"%d ,", res[j]);
	}
	fprintf(yyout,"\n");
}


char* pop1()
{
if(indx==0)
 {
	isvalid = false;
	return "1";
}
    return st[--indx];
}

char* top1()
{
if(indx!= 1)
 {
	isvalid = false;
	return "0";
}
    return st[indx-1];
}

void push2()
{
	res[indx2++] = yylval;
   //strcpy(res[indx2++],yylval);
}

int pop2()
{
if(indx2==0)
 {
	isvalid = false;
	return 0;
}
    return res[--indx2];
}

int top2()
{
if(indx2!=1)
 {
	isvalid = false;
	return 0;
}
    return res[indx2-1];
}

void ADD()
{
	int a = pop2();
	int b = pop2();
	int c = a+b;
	res[indx2++]=c;

}

void SUB()
{
	int a = pop2();
	int b = pop2();
	int c = b-a;
	res[indx2++]=c;
}

void MUL()
{
	int a = pop2();
	int b = pop2();
	int c = a*b;
	res[indx2++]=c;
}

void DIV()
{
	int a = pop2();
	int b = pop2();
	if(a==0)
	{
	isvalid = false;
	res[indx2++]=0;
	}
	else
	{
	int c = b/a;
	res[indx2++]=c;}
}

void prefunc(char a)
{
    char* c1=pop1();
    char* c2=pop1();
    char buffer[100];
    bzero(buffer,100);
	buffer[0] = a;
	buffer[1] = ' ';
	int j=0;
	for(j=0; j<strlen(c2); j++)
	{
		buffer[2+j] = c2[j];
	}

	buffer[2+j] = ' ';
	int k=0;
	for(k=0; k<strlen(c1); k++)
	{
		buffer[2+j+k+1] = c1[k];	
	}
    strcpy(st[indx++], buffer);
	//fprintf(yyout,"st[index++] is : %s\n", buffer);
//fprintf(yyout,"LOL1\n");
}

int main(int argc, char** argv)
{
		extern FILE *yyin, *yyout; 
		/* open the source file 
		in read mode */
	yyin=fopen("postfix.txt","r"); 

		
		/* open the output file 
		in write mode */
	yyout=fopen("result.txt","w"); 
	
    yyparse();
	return 0;
}

