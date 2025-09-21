#include "data.h"
#include "decl.h"
#include "defs.h"
#include <stdio.h>

// Code generator for x86-64
// Copyright (c) 2019 Warren Toomey, GPL3

// List of available registers
// and their names
static int freereg[4];
static char *reglist[4] = {"%r8", "%r9", "%r10", "%r11"};
static char *breglist[4] = {"%r8b", "%r9b", "%r10b", "%r11b"};

// Set all registers as available
void freeall_registers(void) {
  freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

// Allocate a free register. Return the number of
// the register. Die if no available registers.
static int alloc_register(void) {
  for (int i = 0; i < 4; i++) {
    if (freereg[i]) {
      freereg[i] = 0;
      return (i);
    }
  }
  fprintf(stderr, "Out of registers!\n");
  exit(1);
}

// Return a register to the list of available registers.
// Check to see if it's not already there.
static void free_register(int reg) {
  if (freereg[reg] != 0) {
    fprintf(stderr, "Error trying to free register %d\n", reg);
    exit(1);
  }
  freereg[reg] = 1;
}

// Print out the assembly preamble
void cgpreamble() {
  freeall_registers();
  fputs("\t.text\n"
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
        Outfile);
}

// Print out the assembly postamble
void cgpostamble() {
  fputs("\tmovl	$0, %eax\n"
        "\tpopq	%rbp\n"
        "\tret\n",
        Outfile);
}

// Load an integer literal value into a register.
// Return the number of the register
int cgload(int value) {

  // Get a new register
  int r = alloc_register();

  // Print out the code to initialise it
  fprintf(Outfile, "\tmovq\t$%d, %s\n", value, reglist[r]);
  return (r);
}

// Add two registers together and return
// the number of the register with the result
int cgadd(int r1, int r2) {
  fprintf(Outfile, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return (r2);
}

// Subtract the second register from the first and
// return the number of the register with the result
int cgsub(int r1, int r2) {
  fprintf(Outfile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
  free_register(r2);
  return (r1);
}

// Multiply two registers together and return
// the number of the register with the result
int cgmul(int r1, int r2) {
  fprintf(Outfile, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return (r2);
}

// Divide the first register by the second and
// return the number of the register with the result
int cgdiv(int r1, int r2) {
  fprintf(Outfile, "\tmovq\t%s,%%rax\n", reglist[r1]);
  fprintf(Outfile, "\tcqo\n");
  fprintf(Outfile, "\tidivq\t%s\n", reglist[r2]);
  fprintf(Outfile, "\tmovq\t%%rax,%s\n", reglist[r1]);
  free_register(r2);
  return (r1);
}

// Call printint() with the given register
void cgprintint(int r) {
  fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
  fprintf(Outfile, "\tcall\tprintint\n");
  free_register(r);
}

int cgloadglob(char *identifier) {
  int r = alloc_register();
  fprintf(Outfile, "\tmovq\t%s(\%%rip), %s\n", identifier, reglist[r]);
  return r;
}

int cgstorglob(int r, char *identifier) {
  fprintf(Outfile, "\tmovq\t%s, %s(\%%rip)\n", reglist[r], identifier);
  return r;
}

void cgglobsym(char *sym) { fprintf(Outfile, "\t.comm\t%s,8,8\n", sym); }

void genglobsym(char *s) { cgglobsym(s); }

// load an integer literal value into a register.
// return a register
int cgloadint(int value) {
  int reg = alloc_register();
  fprintf(Outfile, "\tmovq\t$%d, %s\n", value, reglist[reg]);

  return reg;
}

// compare two registers
int cgcompare(int r1, int r2, char *how) {
  fprintf(Outfile, "\tcmpq\t%s, %s\n", reglist[r2], reglist[r1]);
  fprintf(Outfile, "\t%s\t%s\n", how, breglist[r2]);
  fprintf(Outfile, "\tandq\t$255, %s\n", reglist[r2]);
  free_register(r1);
  // printf("=========== %d \n",r2);
  return r2;
}

//==
int cgequal(int r1, int r2) { return cgcompare(r1, r2, "sete"); }
int cgnotequal(int r1, int r2) { return cgcompare(r1, r2, "setne"); }
// <
int cglesshan(int r1, int r2) { return cgcompare(r1, r2, "setl"); }
int cglessequal(int r1, int r2) { return cgcompare(r1, r2, "setle"); }
// >
int cggreaterthan(int r1, int r2) { return cgcompare(r1, r2, "setg"); }
int cggreaterequal(int r1, int r2) { return cgcompare(r1, r2, "setge"); }

int cgcompare_and_set(int AstOp, int r1, int r2) {
  fprintf(Outfile, "\tcmpq\t%s,%s\n", reglist[r1], reglist[r2]);
  switch (AstOp) {
  case A_EQ:
    fprintf(Outfile, "\t%s,%s\n", "sete", breglist[r2]);
    break;
  case A_NE:
    fprintf(Outfile, "\t%s,%s\n", "setne", breglist[r2]);
    break;
  case A_LT:
    fprintf(Outfile, "\t%s,%s\n", "setl", breglist[r2]);
    break;

  case A_GT:
    fprintf(Outfile, "\t%s,%s\n", "setg", breglist[r2]);
    break;

  case A_LE:
    fprintf(Outfile, "\t%s,%s\n", "setle", breglist[r2]);
    break;

  case A_GE:
    fprintf(Outfile, "\t%s,%s\n", "setge", breglist[r2]);
    break;

  default:
    fatal("bad ast op in cgcompare_and_set()");
  }
  fprintf(Outfile, "\tmovzbq\t%s, %s\n", breglist[r2], reglist[r2]);

  free_register(r1);
  return r2;
}

int cgcompare_and_jump(int AstOp, int r1, int r2, int l) {
  fprintf(Outfile, "\tcmpq\t%s,%s\n", reglist[r1], reglist[r2]);
  switch (AstOp) {
  case A_EQ:
    fprintf(Outfile, "\t%s\tL%d\n", "jne", l);
    break;
  case A_NE:
    fprintf(Outfile, "\t%s\tL%d\n", "je", l);
    break;
  case A_LT:
    fprintf(Outfile, "\t%s\tL%d\n", "jle", l);
    break;
  case A_GT:
    fprintf(Outfile, "\t%s\tL%d\n", "jge", l);
    break;
  case A_LE:
    fprintf(Outfile, "\t%s\tL%d\n", "jl", l);
    break;
  case A_GE:
    fprintf(Outfile, "\t%s\tL%d\n", "jg", l);
    break;
  default:
    fatal("bad ast op in cgcompare_and_jump()");
    break;
  }
  freeall_registers();
  return -1;
}

void cglabel(int l) { fprintf(Outfile, "L%d:\n", l); }

void cgjump(int l) { fprintf(Outfile, "\tjmp\tL%d\n", l); }
