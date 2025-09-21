#include "decl.h"
#include "data.h"
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
  tree = mkastunary(A_PRINT, tree, 0);
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
  right = mkastleaf(A_LVIDENT, id);
  match(T_EQUALS, "=");

  left = binexpr(0);
  tree = mkastnode(A_ASSIGN, left, NULL, right, 0);

  semi();

  return tree;
}

struct ASTnode *compound_statement() {
  struct ASTnode *left = NULL;
  struct ASTnode *tree = NULL;

  lbrace();

  while (1) {
    switch (Token.token) {
    case T_PRINT:
      tree = print_statement();
      break;
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
    case T_RABRCE:
      rbrace();
      return left;
    default:
      fatald("Syntax error, token", Token.token);
    }

    if (tree) {
      if (left == NULL) {
        left = tree;
      } else {
        left = mkastnode(A_GLUE, left, NULL, tree, 0);
      }
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

  return mkastnode(A_IF, condAST, trueAST, falseAST, 0);
}
