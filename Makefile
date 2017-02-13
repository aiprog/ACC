all:
	lex lexer.c
	yacc -d parser.c
	gcc -Wall -g -o acc acc.c table.c symbol.c type.c lex.yy.c y.tab.c error.c	\
	gen.c ast.c block.c const.c opcode.c asm.c