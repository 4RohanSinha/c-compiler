#ifndef COMPILER_INFO_H
#define COMPILER_INFO_H

#include <stdio.h>

#ifndef extern_
#define extern_ extern
#endif

#include "defs.h"
#define TEXTLEN 512

extern_ int lineNo;
extern_ int putback_;
extern_ FILE* input_file;
extern_ FILE* out_file;
extern_ struct token cur_token;
extern_ char Text[TEXTLEN+1];


#endif
