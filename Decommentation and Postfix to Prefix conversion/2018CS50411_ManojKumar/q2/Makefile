all : lex.l yacc.y
	lex lex.l
	yacc yacc.y
	gcc -o conversion y.tab.c -ll -ly

clean:
	rm -rf conversion
	rm -rf lex.yy.c
	rm -rf y.tab.c
	
