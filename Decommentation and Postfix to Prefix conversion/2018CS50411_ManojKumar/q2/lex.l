%{ 
/* Definition section */
#include <stdbool.h>
%}  
DIGIT [0-9] 
bool ispos = true;

/* Rule Section */
%%  
(\-)?{DIGIT}+		 {yylval = atoi(yytext); return ID;} 
[\+\-\*\/]			return yytext[0];
[\ ]				;
<<EOF>>				{ return khatm;}
\n				{ return Newline;}
.				{ return ID2;}
%% 
 //[\n]				return Newline;

