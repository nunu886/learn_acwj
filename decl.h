
// Function prototypes for all compiler files
// Copyright (c) 2019 Warren Toomey, GPL3
// scan.c
#ifndef DECL_H
#define DECL_H
#include "defs.h"

int scan(struct token *t);

// tree.c
struct ASTnode *mkastnode(int op, struct ASTnode *left,
			  struct ASTnode *right, int intvalue);
struct ASTnode *mkastleaf(int op, int intvalue);
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);

// gen.c
int genAST(struct ASTnode *n, int reg);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);

// cg.c
void freeall_registers(void);
void cgpreamble();
void cgpostamble();
int cgload(int value);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgloadglob(char* identifier);
int cgstorglob(int r, char* identifier);
void cgglobsym(char *sym);
void genglobsym(char *s);
int cgloadint(int value);

// expr.c
struct ASTnode *binexpr(int ptp);
struct ASTnode *primary();

// stmt.c
void statements(void);
void print_statement();
void var_declaration();
void assignment_statement();

// misc.c
void match(int t, char *what);
void semi(void);
void fatald(char *s, int t);
void ident();
void fatal(char *s);
void fatals(char* s1, char* s2);
void fatald(char *s, int t);
void fatalc(char* s, int c);

//sym.c
int findglob(char *s);
static int newglob();
int addglob(char *name);

#endif
