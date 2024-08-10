#include "defs.h"
#include "decl.h"
#include "compiler_info.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// HELPER METHODS

// returns the position of the first instance of a character c in s
static int chrpos(char* s, int c) {
	char* p = strchr(s, c);

	return (p ? p - s : -1);
}

// if we want to rewind while reading characters (going back a character for instance), use this method
static void putback(int c) {
	putback_ = c;
}

// next() proceeds to the next character in the file
static int next() {
	int c;

	if (putback_) {
		c = putback_;
		putback_ = 0;
		return c;
	}

	c = fgetc(input_file);
	if ('\n' == c)
		lineNo++;
	return c;
}

// read the next integer by examining the next character until a non-numeric character is reached
static int scanint(int c) {
	int k, val = 0;

	while ((k = chrpos("0123456789", c)) >= 0) {
		val = val * 10 + k;
		c = next();
	}

	putback(c);
	return val;
}

// read identifier into buf
static int scanident(int c, char* buf, int lim) {
	int len = 0;
	while (isalpha(c) || isdigit(c) || c == '_') {
		if (len == lim - 1) {
			printf("identifier error (too long) on line %d\n", lineNo);
			exit(1);
		}
		else if (len < lim - 1) buf[len++] = c;
		c = next();
	}

	putback(c);
	buf[len] = '\0';
	return len;
}

//get next nonwhitespace character
static int skip() {
	int c;

	c = next();
	while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c)
		c = next();
	return c;
}

//get next character (not whitespace) and process as a token
//if a digit is encountered, scan the integer (will process multiple tokens)
int scan(struct token* t) {
	int c;
	c = skip();
	switch (c) {
		case EOF:
			t->token = T_EOF;
			return 0;
		case '+':
			t->token = T_PLUS;
			break;
		case '-':
			t->token = T_MINUS;
			break;
		case '*':
			t->token = T_STAR;
			break;
		case '/':
			t->token = T_SLASH;
			break;
		//case ';':
		//	t->token = T_SEMI;
		//	break;
		default:
			if (isdigit(c)) {
				t->intvalue = scanint(c);
				t->token = T_INTLIT;
				break;
			}

			printf("Unrecognized character %c on line %d\n", c, lineNo);
			exit(1);
	}

	return 1;
}
