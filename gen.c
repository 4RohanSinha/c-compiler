#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "decl.h"
#include "compiler_info.h"

int genAST(struct ASTnode *n) {
	int leftreg, rightreg;

	if (n->left) leftreg = genAST(n->left);
	if (n->right) rightreg = genAST(n->right);

	switch(n->op) {
		case A_ADD:
			return cgadd(leftreg, rightreg);
		case A_SUBTRACT:
			return cgsub(leftreg, rightreg);
		case A_MULTIPLY:
			return cgmul(leftreg, rightreg);
		case A_DIVIDE:
			return cgdiv(leftreg, rightreg);
		case A_INTLIT:
			return cgload(n->intvalue);
		default:
			fprintf(stderr, "Unknown operator on line %d\n", lineNo);
			exit(1);
	}

}

void generatecode(struct ASTnode* n) {
	int reg;
	cgpreamble();
	reg = genAST(n);
	cgprintint(reg);
	cgpostamble();
}
