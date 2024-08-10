#include "defs.h"
#include <stdlib.h>
#include "decl.h"
#include "compiler_info.h"

struct ASTnode* multiplicative_expr();

struct ASTnode* additive_expr() {
	struct ASTnode* left, *right;
	int tokentype;

	left = multiplicative_expr();

	tokentype = cur_token.token;
	if (tokentype == T_EOF)
		return left;

	while (1) {
		scan(&cur_token);
		right = multiplicative_expr();
		left = mkastnode(arithop(tokentype), left, right, 0);

		tokentype = cur_token.token;
		if (tokentype == T_EOF) break;
	}

	return left;
}

struct ASTnode* multiplicative_expr() {
	struct ASTnode* left, *right;
	int tokentype;

	left = constructLiteralNode();

	tokentype = cur_token.token;

	if (tokentype == T_EOF) return left;

	while (tokentype == T_STAR || tokentype == T_SLASH) {
		scan(&cur_token);
		right = constructLiteralNode();

		left = mkastnode(arithop(tokentype), left, right, 0);

		tokentype = cur_token.token;
		if (tokentype == T_EOF) break;
	}

	return left;
}

static int op_prec[] = {0, 10, 10, 20, 20, 0}; //EOF, +, -, *, /, INTLIT
static int op_precedence(int tokentype) {
	if (op_prec[tokentype] == 0) {
		fprintf(stderr, "syntax error on line %d token %d\n", lineNo, tokentype);
		exit(1);
	}

	return op_prec[tokentype];
}

struct ASTnode* binexpr(int ptp) {
	struct ASTnode* left, *right;
	int tokentype;

	//get integer literal on the left and get following token
	left = constructLiteralNode();
	tokentype = cur_token.token;

	//if following token is EOF, return just left
	if (tokentype == T_EOF) return left;

	//consider binexpr to be 5 * 3 + 2 * 6.
	//we'll continue the OG while loop until the end (input ptp = 0)
		//we'll have 5 be the first left node
		//next token is *, which IS greater than ptp
		//we scan the next token - it's a 3
		//we then call binexp recursively - and here, we want to evaluate anything with higher precedence than the *
		//the current token is a 3, we construct the literal node, but the next operator has lower precedence, so we just return the 3
		//3 is the right node, so 5 and 3 are merged as two children of the node pointed to by left at the time.
		//We now move to the next token - we encounter a +
		//the + is greater than the OG input ptp = 0
		//we fetch the next literal which is a 2
			//we call binexpr recursively again, this time with the operator precedence being a +.
			//the current (left) literal is a 2, we then scan the *.
			//This * has more precedence than a + so we continue and get the next literal.
				//call binexpr again, with a precedence of the *
				//we construct a literal with 6, but this time, we reach EOF so its precedence is obv lower than the *
				//so we return a node with 6
			//so now we got 6 as the right node.
			//we merge the 2 and 6 together under the * and return that tree
		//we then return the tree of 2 and 6 as the right subtree.
		//merge this subtree with the 5-3 subtree and then we return (cur_token is EOF!)

	//continue as long precedence of this token is more than previous token precedence
	while (op_precedence(tokentype) > ptp) {
		//fetch next integer literal
		scan(&cur_token);

		//call binexpr recursively with precedence of current token to build subtree
		right = binexpr(op_prec[tokentype]);


		//join created subtree with current one, convert token to AST
		left = mkastnode(arithop(tokentype), left, right, 0);

		//update current token - if it's EOF, break out
		tokentype = cur_token.token;
		if (tokentype == T_EOF) return left;
	}
	//return tree when precedence is same or lower
	return left;
}

