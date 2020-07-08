%{
	#include<stdio.h> 
#include<string.h>
#include <stdbool.h>
int total=1;
int startcom=1;
int state=0;
%}
%Start comment0 comment1 printable1 transition printable2
whitespace [ \t\n]
comm [^\*]|\*[^\/\n]
digit [0-9]
intcon {digit}+
floatcon {digit}+"."{digit}+
start_comment "/""*"
end_comment "*""/"
comm1 {comm}
newline "\n"
%% 
<INITIAL>\n			{ state=0; fprintf(yyout, "%s", yytext); total++;}
<INITIAL>[^\"\/\'\n]*		{state=1; fprintf(yyout, "%s", yytext); BEGIN transition;}
<INITIAL>[\"]			{state = 4; fprintf(yyout, "\""); BEGIN printable1;}
<INITIAL>[\/][\*]		{state = 2; fprintf(yyout, " "); startcom = total; BEGIN comment0;}
<INITIAL>[\']			{state = 5; fprintf(yyout, "\'"); BEGIN printable2;}
<transition>{start_comment}	{state = 2; fprintf(yyout, " "); startcom = total; BEGIN comment0;}
<transition>[\"]		{state = 4; fprintf(yyout, "\""); BEGIN printable1;}
<transition>[\']		{state = 5; fprintf(yyout, "\'"); BEGIN printable2;}
<transition>\n			{state = 0; total++; fprintf(yyout, "\n"); BEGIN INITIAL;}//<transition>.			{fprintf(yyout, "%s", yytext); BEGIN INITIAL;}
<comment0>\n			{fprintf(yyout, "\n"); total++;}
<comment0>{comm1}		;
<comment0>[\*]			{state = 3; BEGIN comment1;}
<comment0>(\*\/)		{state = 0; BEGIN INITIAL;}
<comment1>\n			{state = 2; fprintf(yyout, "\n"); total++; BEGIN comment0;}
<comment1>[^\*\/\n]		{state = 2; BEGIN comment0;}
<comment1>[\*]			;
<comment1>[\/]			{state = 0; BEGIN INITIAL;}
<printable1>\n			{fprintf(yyout, "\n"); total++;}
<printable1>[^\"\\]		fprintf(yyout, "%s", yytext);
<printable1>(\\(\'|\"|\\))	fprintf(yyout, "%s", yytext);
<printable1>[\"]		{ state = 0; fprintf(yyout, "\""); BEGIN INITIAL;} 
<printable2>\n			{fprintf(yyout, "\n"); total++;}
<printable2>[^\'\\]		fprintf(yyout, "%s", yytext);
<printable2>(\\(\'|\"|\\))	fprintf(yyout, "%s", yytext);
<printable2>[\']		{state = 0; fprintf(yyout, "\'"); BEGIN INITIAL;} 
%%

int yywrap() 
{ 
	return 1; 
} 

/* code section */
int main() 
{ 
		//extern FILE *yyin, *yyout; 
		/* open the source file 
		in read mode */
	//yyin=fopen("input.c","r"); 

		
		/* open the output file 
		in write mode */
	//yyout=fopen("output.c","w"); 
	//yyerror=fopen("error.txt", "w");
	total=1;
	yylex(); 
	if(state == 2 || state == 3)
	{
		fprintf(stderr, "Error: line %d: unterminated comment\n", startcom);
	} 
}
