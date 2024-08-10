#include "compiler_info.h"
#include "defs.h"
#include "decl.h"
#include <stdlib.h>

void match(int t, char* what) {
	if (cur_token.token == t) scan(&cur_token);
	else {
		printf("%s expected on line %d\n", what, lineNo);
		exit(1);
	}
}

void semi() {
	match(T_SEMI, ";");
}

void ident() { match(T_IDENT, "identifier"); }


void fatal(char *s) {
  fprintf(stderr, "%s on line %d\n", s, lineNo); exit(1);
}

void fatals(char *s1, char *s2) {
  fprintf(stderr, "%s:%s on line %d\n", s1, s2, lineNo); exit(1);
}

void fatald(char *s, int d) {
  fprintf(stderr, "%s:%d on line %d\n", s, d, lineNo); exit(1);
}

void fatalc(char *s, int c) {
  fprintf(stderr, "%s:%c on line %d\n", s, c, lineNo); exit(1);
}
