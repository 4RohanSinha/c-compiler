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
