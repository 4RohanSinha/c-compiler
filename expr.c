#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "decl.h"
#include "compiler_info.h"


struct ASTnode *mkastnode(int op, struct ASTnode* left, struct ASTnode* right, int intvalue) {
	struct ASTnode* n;
	
	n = (struct ASTnode*)malloc(sizeof(struct ASTnode));

	if (n == NULL) {
		fprintf(stderr, "malloc failure\n");
		exit(1);
	}

	n->op = op;
	n->left = left;
	n->right = right;
	n->intvalue = intvalue;
	return n;
}

struct ASTnode *mkastleaf(int op, int intvalue) {
	return (mkastnode(op, NULL, NULL, intvalue));
}

struct ASTnode *mkastunary(int op, struct ASTnode* left, int intvalue) {
	return (mkastnode(op, left, NULL, intvalue));
}

int arithop(int tok) {
	switch (tok) {
		case T_PLUS:
			return A_ADD;
		case T_MINUS:
			return A_SUBTRACT;
		case T_STAR:
			return A_MULTIPLY;
		case T_SLASH:
			return A_DIVIDE;
		default:
			fprintf(stderr, "Unknown arithop error\n");
			exit(1);
	}
}

struct ASTnode *constructLiteralNode() {
	struct ASTnode* n;
	
	switch (cur_token.token) {
		case T_INTLIT:
			n = mkastleaf(A_INTLIT, cur_token.intvalue);
			scan(&cur_token);
			return n;
		default:
			fprintf(stderr, "syntax error on line %d\n", lineNo);
			exit(1);
	}
}

struct ASTnode* constructBinExp() {
	struct ASTnode* n, *left, *right;
	int nodetype;

	left = constructLiteralNode();

	if (cur_token.token == T_EOF) return left;

	//next token is for sure not a number - since the integer conversion process stopped before
	nodetype = arithop(cur_token.token);
	//get next token (an operator)
	scan(&cur_token);

	//continue recursively for other side
	right = constructBinExp();

	//add both left and right subtrees to main tree
	n = mkastnode(nodetype, left, right, 0);
	return n;
}
