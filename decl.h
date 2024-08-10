#include "defs.h"

int scan(struct token* t);
struct ASTnode *mkastnode(int op, struct ASTnode *left,
			  struct ASTnode *right, int intvalue);
struct ASTnode *mkastleaf(int op, int intvalue);
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);
struct ASTnode *constructBinExp();
int interpretAST(struct ASTnode *n);
int genAST(struct ASTnode* n);
int arithop(int);
struct ASTnode* binexpr(int);
struct ASTnode* constructLiteralNode();
void freeall_registers();
void generatecode(struct ASTnode* n);
void cgpreamble();
void cgpostamble();
int cgload(int value);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
void match(int, char*);
void semi();
void genpreamble();
void genpostamble();
void statements();
