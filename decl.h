
// Function prototypes for all compiler files
// Copyright (c) 2019 Warren Toomey, GPL3
// scan.c
#ifndef DECL_H
#define DECL_H
#include "defs.h"

int scan(struct token *t);

// tree.c
struct ASTnode *mkastnode(int op, struct ASTnode *left, struct ASTnode *mid,
                          struct ASTnode *right, int intvalue);
struct ASTnode *mkastleaf(int op, int intvalue);
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);

// gen.c
int genAST(struct ASTnode *n, int reg, int parentASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
int genWhile(struct ASTnode *n);

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
int cgloadglob(char *identifier);
int cgstorglob(int r, char *identifier);
void cgglobsym(char *sym);
void genglobsym(char *s);
int cgloadint(int value);
int cgequal(int r1, int r2);
int cgnotequal(int r1, int r2);
int cglesshan(int r1, int r2);
int cggreaterthan(int r1, int r2);
int cglessequal(int r1, int r2);
int cggreaterequal(int r1, int r2);
int cgcompare(int r1, int r2, char *how);

int cgcompare_and_set(int AstOp,int r1, int r2);
int cgcompare_and_jump(int AstOp,int r1, int r2, int l);
void cglabel(int l);
void cgjump(int l);

// expr.c
struct ASTnode *binexpr(int ptp);
struct ASTnode *primary();

// stmt.c
struct ASTnode *  statements();
struct ASTnode * print_statement();
void var_declaration();
struct ASTnode * assignment_statement();
struct ASTnode *compound_statement();
struct ASTnode *if_statement();
struct ASTnode *while_statement();
struct ASTnode *single_statement();
struct ASTnode *for_statement();

// misc.c
void match(int t, char *what);
void semi(void);
void fatald(char *s, int t);
void ident();
void fatal(char *s);
void fatals(char *s1, char *s2);
void fatald(char *s, int t);
void fatalc(char *s, int c);
void lparen();
void rparen();
void lbrace();
void rbrace();

// sym.c
int findglob(char *s);
static int newglob();
int addglob(char *name);

#endif
