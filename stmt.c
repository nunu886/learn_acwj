#include "data.h"
#include "decl.h"
#include "defs.h"
#include <string.h>
// Parsing of statements
// Copyright (c) 2019 Warren Toomey, GPL3

// statements: statement
//      | statement statements
//      ;
//
// statement: 'print' expression ';'
//            'int' identifier ';
//            identifier '=' expression ';'
//            | if_statement;
// identifier: T_IDENT
//      ;
// if_statement: 'if' '(' true_or_false_expression ')''{' (statement |
// statements)* '}'
//                ['else' '{' (statement | statements)* '}']
// while_statement: 'while' '(' true_or_false_expression ')''{' (statement |
// statements)* '}'

// Parse one or more statements
struct ASTnode *statements() { return compound_statement(); }

struct ASTnode *print_statement(void) {
  struct ASTnode *tree = NULL;
  int reg = 0;

  // Match a 'print' as the first token
  match(T_PRINT, "print");

  // Parse the following expression and
  // generate the assembly code
  tree = binexpr(0);
  int lefttype = P_INT;
  int righttype = tree->type;

  if (!type_compatiable(&lefttype, &righttype, 0)) {
    fatal("incompatible types");
  }

  if (righttype) {
    tree = mkastunary(righttype, P_INT, tree, 0);
  }

  tree = mkastunary(A_PRINT, P_INT, tree, 0);
  // printf("<<<< %d \n", reg)
  // Match the following semicolon
  semi();

  return tree;
}

// identifier '=' expression ';'
struct ASTnode *assignment_statement() {
  struct ASTnode *left = NULL;
  struct ASTnode *right = NULL;
  struct ASTnode *tree = NULL;

  ident();
  int id = findglob(Text);
  if (id == -1) {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(A_LVIDENT, Gsym[id].type, id);
  match(T_EQUALS, "=");

  left = binexpr(0);
  int lefttype = left->type;
  int righttype = right->type;

  if (!type_compatiable(&lefttype, &righttype, 0)) {
    fatal("incompatible types");
  }

  tree = mkastnode(A_ASSIGN, P_INT, left, NULL, right, 0);

  semi();

  return tree;
}

struct ASTnode *single_statement() {
  struct ASTnode *tree = NULL;
  switch (Token.token) {
  case T_PRINT:
    tree = print_statement();
    break;
  case T_CHAR:
  case T_INT:
    var_declaration();
    tree = NULL;
    break;
  case T_IDENT:
    tree = assignment_statement();
    break;
  case T_IF:
    tree = if_statement();
    break;
  case T_WHILE:
    tree = while_statement();
    break;
  case T_FOR:
    tree = for_statement();
    break;
  default:
    fatald("Syntax error, token", Token.token);
  }

  return tree;
}

struct ASTnode *compound_statement() {
  struct ASTnode *left = NULL;
  struct ASTnode *tree = NULL;

  lbrace();

  while (1) {
    tree = single_statement();
    // if (tree != NULL && tree->op == A_ASSIGN) {
    //   semi();
    // }

    if (tree) {
      if (left == NULL) {
        left = tree;
      } else {
        left = mkastnode(A_GLUE, P_NONE, left, NULL, tree, 0);
      }
    }

    if (Token.token == T_RABRCE) {
      rbrace();
      return left;
    }
  }
}

struct ASTnode *if_statement() {
  struct ASTnode *condAST = NULL;
  struct ASTnode *trueAST = NULL;
  struct ASTnode *falseAST = NULL;

  match(T_IF, "if");
  lparen();

  condAST = binexpr(0);

  // add a function if op is A_EQ -- A_GE

  rparen();

  trueAST = compound_statement();

  if (Token.token == T_ELSE) {
    match(T_ELSE, "else");
    falseAST = compound_statement();
  }

  return mkastnode(A_IF, P_NONE, condAST, trueAST, falseAST, 0);
}

struct ASTnode *while_statement() {
  struct ASTnode *condAST = NULL;
  struct ASTnode *bodyAST = NULL;

  match(T_WHILE, "while");
  lparen();

  condAST = binexpr(0);
  // add a function if op is A_EQ -- A_GE
  // switch(condAST->op)
  rparen();

  // {
  bodyAST = compound_statement();
  // }

  return mkastnode(A_WHILE, P_NONE, condAST, NULL, bodyAST, 0);
}

// for(i = 0; i < 10; i++){
//
//}
// while statement instance of for statement;
struct ASTnode *for_statement() {
  struct ASTnode *init = NULL;
  struct ASTnode *cond = NULL;
  struct ASTnode *incr = NULL;
  struct ASTnode *body = NULL;
  struct ASTnode *tree = NULL;

  match(T_FOR, "for");
  lparen();

  init = single_statement();

  cond = binexpr(0);
  semi();
  // cond expr syntax check
  //
  incr = single_statement();
  // skip ')'
  rparen();

  body = compound_statement();
  tree = mkastnode(A_GLUE, P_NONE, body, NULL, incr, 0);
  tree = mkastnode(A_WHILE, P_NONE, cond, NULL, tree, 0);

  return mkastnode(A_GLUE, P_NONE, init, NULL, tree, 0);
}
