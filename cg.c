#include <stdlib.h>
#include <stdio.h>
#include "defs.h"
#include "compiler_info.h"
#include "decl.h"

static int freereg[4];
static char* reglist[4] = {"%r8", "%r9", "%r10", "%r11"};

void freeall_registers() {
	for (int i = 0; i < 4; i++) freereg[i] = 1;
}

int alloc_register() {
	for (int i = 0; i < 4; i++) {
		if (freereg[i]) {
			freereg[i] = 0;
			return i;
		}
	}

	fprintf(stderr, "No more registers!\n");
	exit(1);
}

void free_register(int reg) {
	if (freereg[reg] != 0) {
		fprintf(stderr, "Error freeing register %d\n", reg);
		exit(1);
	}

	freereg[reg] = 1;
}

void cgpreamble() {
	freeall_registers();
	fputs(
		"\t.text\n"
		".LC0:\n"
		"\t.string\t\"%d\\n\"\n"
		"printint:\n"
		"\tpushq\t%rbp\n"
		"\tmovq\t%rsp, %rbp\n"
		"\tsubq\t$16, %rsp\n"
		"\tmovl\t%edi, -4(%rbp)\n"
		"\tmovl\t-4(%rbp), %eax\n"
		"\tmovl\t%eax, %esi\n"
		"\tleaq	.LC0(%rip), %rdi\n"
		"\tmovl	$0, %eax\n"
		"\tcall	printf@PLT\n"
		"\tnop\n"
		"\tleave\n"
		"\tret\n"
		"\n"
		"\t.globl\tmain\n"
		"\t.type\tmain, @function\n"
		"main:\n"
		"\tpushq\t%rbp\n"
		"\tmovq	%rsp, %rbp\n",
	  out_file);
}

void cgpostamble() {
	fputs(
		"\tmovl\t$0, %eax\n"
		"\tpopq\t%rbp\n"
		"\tret\n",
	out_file);
}

int cgload(int value) {
	int r = alloc_register();
	fprintf(out_file, "\tmovq\t$%d, %s\n", value, reglist[r]);
	return r;
}

int cgadd(int r1, int r2) {
	fprintf(out_file, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
	free_register(r1);
	return r2;
}

int cgsub(int r1, int r2) {
	fprintf(out_file, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
	free_register(r2);
	return r1;

}

int cgmul(int r1, int r2) {
	fprintf(out_file, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
	free_register(r1);
	return r2;

}

int cgdiv(int r1, int r2) {
	fprintf(out_file, "\tmovq\t%s, %%rax\n", reglist[r1]);
	fprintf(out_file, "\tcqo\n");
	fprintf(out_file, "\tidivq\t%s\n", reglist[r2]);
	fprintf(out_file, "\tmovq\t%%rax, %s\n", reglist[r1]);
	free_register(r2);
	return r1;
}


void cgprintint(int r) {
	fprintf(out_file, "\tmovq\t%s, %%rdi\n", reglist[r]);
	fprintf(out_file, "\tcall\tprintint\n");
	free_register(r);
}

