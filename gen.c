#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "decl.h"
#include "compiler_info.h"

void genpreamble() { cgpreamble(); }
void genpostamble() { cgpostamble(); }
void genfreeregs() { freeall_registers(); }
void genprintint(int reg) { cgprintint(reg); }
void genglobsym(char *s) { cgglobsym(s); }

/*
void statements() {
	struct ASTnode* tree;
	int reg;

	while (1) {
		match(T_PRINT, "print");

		tree = binexpr(0);
		reg = genAST(tree);
		genprintint(reg);
		genfreeregs();

		semi();
		if (cur_token.token == T_EOF) return;
	}
}*/

int genAST(struct ASTnode *n, int reg) {
	int leftreg, rightreg;

	if (n->left) leftreg = genAST(n->left, -1);
	if (n->right) rightreg = genAST(n->right, leftreg);

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
			return cgloadint(n->v.intvalue);
		case A_LVIDENT:
			return cgstorglob(reg, Gsym[n->v.id].name);
		case A_IDENT:
			return cgloadglob(Gsym[n->v.id].name);
		case A_ASSIGN:
			return rightreg;
		default:
			fprintf(stderr, "Unknown operator on line %d\n", lineNo);
			exit(1);
	}

}

/*
void generatecode(struct ASTnode* n) {
	int reg;
	cgpreamble();
	reg = genAST(n);
	cgprintint(reg);
	cgpostamble();
}*/
