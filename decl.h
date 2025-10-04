
// Function prototypes for all compiler files
// Copyright (c) 2019 Warren Toomey, GPL3
// scan.c
#ifndef DECL_H
#define DECL_H
#include "defs.h"

// scan.c
int scan(struct token *t);
void reject_token(struct token *t);

// tree.c
struct ASTnode *mkastnode(int op, int type, struct ASTnode *left,
                          struct ASTnode *mid, struct ASTnode *right,
                          int intvalue);
struct ASTnode *mkastleaf(int op, int type, int intvalue);
struct ASTnode *mkastunary(int op, int type, struct ASTnode *left,
                           int intvalue);

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
int cgloadglob(int id);
int cgstorglob(int r, int id);
void cgglobsym(int id);
void genglobsym(int id);
int cgloadint(int value);
int cgequal(int r1, int r2);
int cgnotequal(int r1, int r2);
int cglesshan(int r1, int r2);
int cggreaterthan(int r1, int r2);
int cglessequal(int r1, int r2);
int cggreaterequal(int r1, int r2);
int cgcompare(int r1, int r2, char *how);
void cgfuncpreamble(char *name);
void cgfuncpostamble(int id);
int cgwiden(int r, int oldtype, int newtype);

int cgcompare_and_set(int AstOp, int r1, int r2);
int cgcompare_and_jump(int AstOp, int r1, int r2, int l);
void cglabel(int l);
void cgjump(int l);
int cgprimsize(int type);
int cgcall(int r, int id);
void cgreturn(int reg, int id);
int cgderef(int reg, int type);
int cgaddress(int id);

// expr.c
struct ASTnode *binexpr(int ptp);
struct ASTnode *primary();
struct ASTnode *func_call();
struct ASTnode *prefix();

// stmt.c
struct ASTnode *statements();
struct ASTnode *print_statement();
struct ASTnode *assignment_statement();
struct ASTnode *compound_statement();
struct ASTnode *if_statement();
struct ASTnode *while_statement();
struct ASTnode *single_statement();
struct ASTnode *for_statement();
struct ASTnode *return_statement();

// decl.c
void var_declaration();
struct ASTnode *function_declaration();
int parse_type(int type);

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
int addglob(char *name, int type, int stype, int endlabel);

// types.c
int type_compatiable(int *left, int *right, int onlyright);
int pointer_to(int type);
int valaue_at(int ptrtype);

// gen.c
int label();
#endif
