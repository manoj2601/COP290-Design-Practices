all : lex.l
	lex lex.l
	gcc -o decomment lex.yy.c -ll

clean : 
	rm -rf decomment
	rm -rf lex.yy.c