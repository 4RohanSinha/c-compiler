#ifndef DEFS_H
#define DEFS_H

enum {
	T_EOF, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_INTLIT, T_SEMI, T_PRINT, T_IDENT, T_EQUALS, T_INT
};

enum {
	A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, A_INTLIT, A_IDENT, A_LVIDENT, A_ASSIGN
};

struct token {
	int token;
	int intvalue;
};

struct ASTnode {
	int op;
	struct ASTnode* left;
	struct ASTnode* right;
	union {
		int intvalue;
		int id;
	} v;
};

struct symtable {
	char* name;
};

#endif
