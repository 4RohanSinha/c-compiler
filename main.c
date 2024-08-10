#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "defs.h"
#include "decl.h"
#define extern_
#include "compiler_info.h"
#undef extern_

void init() {
	lineNo = 1;
	putback_ = '\n';
}

void usage(char* prog) {
	printf("Usage: %s infile\n", prog);
	exit(1);
}

char* tokstr[] = { "+", "-", "*", "/", "intlit" };

void scanfile() {
	struct token T;

	while (scan(&T)) {
		printf("Token %s", tokstr[T.token]);
		if (T.token == T_INTLIT)
			printf(", value %d", T.intvalue);
		printf("\n");
	}
}

int main(int argc, char **argv) {
	if (argc != 2)
		usage(argv[0]);
	init();
	if ((input_file = fopen(argv[1], "r")) == NULL) {
		printf("Unable to open %s: %s\n", argv[1], strerror(errno));
		return 1;
	}

	if ((out_file = fopen("out.s", "w")) == NULL) {
		printf("Unable to create out.s: %s\n", strerror(errno));
		return 1;
	}

//	scanfile();
	scan(&cur_token);			// Get the first token from the input
  	struct ASTnode* n = binexpr(0);		// Parse the expression in the file
  	printf("%d\n", interpretAST(n));	
	generatecode(n);
	fclose(out_file);
	return 0;
}
