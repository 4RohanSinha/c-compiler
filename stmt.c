#include "decl.h"
#include "compiler_info.h"
#include "defs.h"


void var_declaration() {
	match(T_INT, "int");
	ident();
	addglob(Text);
	genglobsym(Text);
	semi();
}

void assignment_statement() {
	struct ASTnode* left, *right, *tree;
	int id;

	//check it's an identifier
	ident();

	//check if it's been defined
	if ((id = findglob(Text)) == -1) fatals("Undeclared variable", Text);	

	//construct tree
	
	//first add the identifier as the right node of the AST
	right = mkastleaf(A_LVIDENT, id);

	//match an equals sign
	match(T_EQUALS, "=");
	
	//parse left side expression
	left = binexpr(0);

	//create AST tree
	tree = mkastnode(A_ASSIGN, left, right, 0);
	
	//generate assembly code for assignment
	int reg = genAST(tree, -1);
	genfreeregs();
	
	//match semicolon
	semi();
	
}

void print_statement() {
	struct ASTnode* tree;
	int reg;
	match(T_PRINT, "print");
	tree = binexpr(0);
	reg = genAST(tree, -1);
	genprintint(reg);
	genfreeregs();
	semi();
}

void statements() {
	struct ASTnode* tree;
	int reg;

	while (1) {
		switch (cur_token.token) {
			case T_PRINT:
				print_statement();
				break;
			case T_INT:
				var_declaration();
				break;
			case T_IDENT:
				assignment_statement();
				break;
			case T_EOF:
				return;
			default:
				fatald("Syntax error, token", cur_token.token);
		}
		/*match(T_PRINT, "print");

		tree = binexpr(0);
		reg = genAST(tree);
		genprintint(reg);
		genfreeregs();

		semi();
		if (cur_token.token == T_EOF) return;*/
	}
}
