#include "data.h"
#include "decl.h"
#include <stdio.h>
// Generic code generator
// Copyright (c) 2019 Warren Toomey, GPL3

// Given an AST, generate
// assembly code recursively
int genAST(struct ASTnode *n, int tag) {
  int leftreg = 0;
  int rightreg = 0;

  // Get the left and right sub-tree values
  if (n->left)
    leftreg = genAST(n->left, -1);
  if (n->right)
    rightreg = genAST(n->right, leftreg);

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
    return (cgloadglob(Gsym[n->v.id].name));
  case A_LVIDENT:
    // printf("------------- %d\n", rightreg);
    return (cgstorglob(tag, Gsym[n->v.id].name));
  case A_ASSIGN:
    // this work has already done, return the result.
    return rightreg;
  case A_EQ:
    return (cgequal(leftreg, rightreg));
  case A_NE:
    return cgnotequal(leftreg, rightreg);
  case A_GT:
    return cggreaterequal(leftreg, rightreg);
  case A_LT:
    return (cglesshan(leftreg, rightreg));
  case A_LE:
    return (cglessequal(leftreg, rightreg));
  case A_GE:
    return (cggreaterequal(leftreg, rightreg));
  default:
    fprintf(stderr, "Unknown AST operator %d\n", n->op);
    exit(1);
  }
}

void genpreamble() { cgpreamble(); }
void genpostamble() { cgpostamble(); }
void genfreeregs() { freeall_registers(); }
void genprintint(int reg) { cgprintint(reg); }
