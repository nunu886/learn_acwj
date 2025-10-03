#include "data.h"
#include "decl.h"
#include "defs.h"
#include <stdio.h>
// Generic code generator
// Copyright (c) 2019 Warren Toomey, GPL3

int label() {
  static int id = 1;
  return id++;
}

static int genIFAST(struct ASTnode *n) {
  int Lfalse = 0;
  int Lend = 0;

  Lfalse = label();
  if (n->right) {
    Lend = label();
  }
  // condition
  genAST(n->left, Lfalse, n->op);
  genfreeregs();

  // true branch
  genAST(n->mid, -1, n->op);
  genfreeregs();

  if (n->right) {
    cgjump(Lend);
  }
  cglabel(Lfalse);

  // false branch
  if (n->right) {
    genAST(n->right, -1, n->op);
    genfreeregs();
    cglabel(Lend);
  }

  return -1;
}

int genWhile(struct ASTnode *n) {
  int Lstart = label();
  int Lend = label();
  cglabel(Lstart);

  // gen
  genAST(n->left, Lend, n->op);
  genfreeregs();

  genAST(n->right, -1, n->op);
  genfreeregs();

  cgjump(Lstart);
  cglabel(Lend);
  return -1;
}

// Given an AST, generate
// assembly code recursively
int genAST(struct ASTnode *n, int reg, int parentASTop) {
  // printf("reg:  %d\n", reg);
  int leftreg = 0;
  int rightreg = 0;

  switch (n->op) {
  case A_IF:
    return genIFAST(n);
  case A_WHILE:
    return genWhile(n);
  case A_GLUE:
    genAST(n->left, -1, n->op);
    genAST(n->right, -1, n->op);
    genfreeregs();
    return -1;
  case A_FUNCTION:
    cgfuncpreamble(Gsym[n->v.id].name);
    genAST(n->left, -1, n->op);
    cgfuncpostamble(n->v.id);
    return -1;
  }

  // Get the left and right sub-tree values
  if (n->left) {
    leftreg = genAST(n->left, -1, n->op);
  }
  if (n->right) {
    rightreg = genAST(n->right, leftreg, n->op);
  }

  switch (n->op) {
  case A_ADD:
    return (cgadd(leftreg, rightreg));
  case A_SUBTRACT:
    return (cgsub(leftreg, rightreg));
  case A_MULTIPLY:
    return (cgmul(leftreg, rightreg));
  case A_DIVIDE:
    return (cgdiv(leftreg, rightreg));
  case A_INTLIT:
    // printf("INTLIT------------- %d\n", n->v.intvalue);
    return (cgloadint(n->v.intvalue));
  case A_IDENT:
    return (cgloadglob(n->v.id));
  case A_LVIDENT:
    // printf("------------- %d\n", rightreg);
    return cgstorglob(reg, n->v.id);
  case A_ASSIGN:
    // this work has already done, return the result.
    return rightreg;
  case A_EQ:
  case A_NE:
  case A_GT:
  case A_LT:
  case A_LE:
  case A_GE:
    if (parentASTop == A_IF || parentASTop == A_WHILE) {
      return cgcompare_and_jump(n->op, leftreg, rightreg, reg);
    } else {
      return (cgcompare_and_set(n->op, leftreg, rightreg));
    }
  case A_PRINT:
    genprintint(leftreg);
    genfreeregs();
    return (-1);
  case A_WIDEN:
    return cgwiden(leftreg, n->left->type, n->type);
  case A_RETURN:
    cgreturn(leftreg, FunctionId);
    return -1;
  case A_FUNCCALL:
    return cgcall(leftreg, n->v.id);
  default:
    fprintf(stderr, "Unknown AST operator %d\n", n->op);
    exit(1);
  }
}

void genpreamble() { cgpreamble(); }
void genpostamble() { cgpostamble(); }
void genfreeregs() { freeall_registers(); }
void genprintint(int reg) { cgprintint(reg); }
